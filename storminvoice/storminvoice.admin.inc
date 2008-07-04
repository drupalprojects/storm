<?php
// $Id$

/**
 * @file
 */

function storminvoice_list() {
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['storminvoice_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['storminvoice_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['storminvoice_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['storminvoice_list_filter']['organization_nid'] = $p->organization_nid;
  }

  $o = drupal_get_form('storminvoice_list_filter');

  $i = new stdClass();
  $i->type = 'storminvoice';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Number'),
      'field' => 'sin.number',
      'sort' => 'asc',
      'nowrap' => 'nowrap',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'sin.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sin.project_title',
    ),
    array(
      'data' => t('Description'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Req. date'),
      'field' => 'sin.requestdate',
    ),
    array(
      'data' => t('Total'),
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.title, sin.* FROM {node} AS n INNER JOIN {storminvoice} AS sin ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' ";

  $s_totals_topay = "SELECT SUM(amount) amount, SUM(vat) vat, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate=0";

  $s_totals_paid = "SELECT SUM(amount) amount, SUM(vat) vat, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate<>0";

  $s_totals = "SELECT SUM(amount) amount, SUM(vat) vat, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice'";

  if ($_SESSION['storminvoice_list_filter']['organization_nid']) {
    $where[] = 'sin.organization_nid='. $_SESSION['storminvoice_list_filter']['organization_nid'];
  }
  if ($_SESSION['storminvoice_list_filter']['project_nid']) {
    $where[] = 'sin.project_nid='. $_SESSION['storminvoice_list_filter']['project_nid'];
  }
  if ($_SESSION['storminvoice_list_filter']['reqdatefrom']) {
    $reqdatefrom = $_SESSION['storminvoice_list_filter']['reqdatefrom'];
    $where[] = 'sin.requestdate>='. _storm_date_to_gmtimestamp($reqdatefrom);
  }
  if ($_SESSION['storminvoice_list_filter']['reqdateto']) {
    $reqdateto = $_SESSION['storminvoice_list_filter']['reqdateto'];
    $where[] = 'sin.requestdate<='. _storm_date_to_gmtimestamp($reqdateto);
  }

  $status = $_SESSION['storminvoice_list_filter']['status'];
  if ($status && $status !='-') {
    if ($status=='to pay') {
      $where[] = "sin.paymentdate=0";
    }
    if ($status=='paid') {
      $where[] = "sin.paymentdate<>0";
    }
    if ($status=='overdue') {
      $where[] = "sin.paymentdate=0 AND sin.duedate<". time();
    }

    $where[] = 'sin.requestdate<='. _storm_date_to_gmtimestamp($reqdateto);
  }

  $itemsperpage = $_SESSION['storminvoice_list_filter']['itemsperpage'];
  if (!$itemsperpage) $itemsperpage = 10;

  $tablesort = tablesort_sql($header);

  if ($tablesort == " ORDER BY  sin.number ASC") {
    $tablesort = ' ORDER BY LPAD(sin.number, 10, "0") ASC';
  }
  else if ($tablesort == " ORDER BY  sin.number DESC") {
    $tablesort = ' ORDER BY LPAD(sin.number, 10, "0") DESC';
  }

  $s = db_rewrite_sql($s);
  $s = storminvoice_access_sql($s, $where);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);
  $invoices = array();
  while ($invoice = db_fetch_object($r)) {
    $invoices[] = $invoice;
  }

  $r = db_query(storminvoice_access_sql($s_totals_topay, $where));
  if ($r) $totals_topay = db_fetch_object($r);

  $r = db_query(storminvoice_access_sql($s_totals_paid, $where));
  if ($r) $totals_paid = db_fetch_object($r);

  $r = db_query(storminvoice_access_sql($s_totals, $where));
  if ($r) $totals = db_fetch_object($r);

  $o .= theme('storminvoice_list', $header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals);
  print theme('page', $o);
}

function storminvoice_list_filter() {
  $organization_nid = $_SESSION['storminvoice_list_filter']['organization_nid'];
  $project_nid = $_SESSION['storminvoice_list_filter']['project_nid'];
  $status = $_SESSION['storminvoice_list_filter']['status'];
  $itemsperpage = $_SESSION['storminvoice_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['storminvoice_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $reqdatefrom = $_SESSION['storminvoice_list_filter']['reqdatefrom'];
  $today = time();
  if (!$reqdatefrom) {
    $reqdatefrom = array('day' => 1,
                         'month' => 1,
                         'year' => format_date($today, 'custom', 'Y'),
                         'hour' => 0,
                         'minute' => 0,
                        );
    $_SESSION['storminvoice_list_filter']['reqdatefrom'] = $reqdatefrom;
  }

  $reqdateto = $_SESSION['storminvoice_list_filter']['reqdateto'];
  if (!$reqdateto) {
    $reqdateto = array('day' => 31,
                       'month' => 12,
                       'year' => format_date($today, 'custom', 'Y'),
                       'hour' => 23,
                       'minute' => 59,
    );
    $_SESSION['storminvoice_list_filter']['reqdateto'] = $reqdateto;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Search'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = db_rewrite_sql($s);
  $r = db_query(stormorganization_access_sql($s));
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => t('All')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormproject_organization_projects(this, 'edit-project-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = db_rewrite_sql($s);
  $r = db_query(stormproject_access_sql($s), $organization_nid);
  $projects = array();
  while ($project = db_fetch_array($r)) {
    $projects[$project['nid']] = $project['title'];
  }
  $projects = array(0 => t('All')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -19,
  );

  $form['filter']['group2']['reqdatefrom'] = array(
    '#type' => 'date',
    '#title' => t('Req. date from'),
    '#default_value' => $reqdatefrom,
  );

  $form['filter']['group2']['reqdateto'] = array(
    '#type' => 'date',
    '#title' => t('Req. date to'),
    '#default_value' => $reqdateto,
  );

  $form['filter']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('-' => t('all'), 'to pay' => t('to pay'), 'paid' => t('paid'), 'overdue' => t('overdue')),
    '#default_value' => $status,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  $form['filter']['group3']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function storminvoice_list_filter_submit($form_id, $form_values) {
  if ($form_values['op'] == t('Filter')) {
    storminvoice_list_filter_filter($form_id, $form_values);
  }
  else if ($form_values['op'] == t('Reset')) {
    storminvoice_list_filter_reset($form_id, $form_values);
  }
}

function storminvoice_list_filter_filter($form_id, $form_values) {
  $_SESSION['storminvoice_list_filter']['organization_nid'] = $form_values['organization_nid'];
  $_SESSION['storminvoice_list_filter']['project_nid'] = $form_values['project_nid'];
  $_SESSION['storminvoice_list_filter']['reqdatefrom'] = $form_values['reqdatefrom'];
  $_SESSION['storminvoice_list_filter']['reqdateto'] = $form_values['reqdateto'];
  $_SESSION['storminvoice_list_filter']['status'] = $form_values['status'];
  $_SESSION['storminvoice_list_filter']['itemsperpage'] = $form_values['itemsperpage'];
}

function storminvoice_list_filter_reset($form_id, $form_values) {
  unset($_SESSION['storminvoice_list_filter']);
}

function _storminvoice_project_invoices_js($organization_nid, $project_nid) {
  $invoices = array();

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {storminvoice} AS sin
  ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice' AND sin.organization_nid=%d AND sin.project_nid=%d
  ORDER BY n.title";
  $s = db_rewrite_sql($s);

  $r = db_query(storminvoice_access_sql($s), $organization_nid, $project_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $invoices[$nid] = $item->title;
  }
  print drupal_to_js($invoices);
  exit();
}

function storminvoice_report($node, $report, $language) {
  print theme('storminvoice_report', $node, $report, $language);
}
