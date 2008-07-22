<?php
// $Id$

/**
 * @file
 */

function stormproject_list() {
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormproject_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormproject_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
  }

  $o = drupal_get_form('stormproject_list_filter');

  $i = new stdClass();
  $i->type = 'stormproject';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'spr.organization_title',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Status'),
      'field' => 'spr.projectstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'spr.projectpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.*, spr.* FROM {node} AS n INNER JOIN {stormproject} AS spr ON n.vid=spr.vid WHERE n.status=1 AND n.type='stormproject'";

  $where = array();
  if ($_SESSION['stormproject_list_filter']['organization_nid']) {
    $where[] = 'spr.organization_nid='. $_SESSION['stormproject_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormproject_list_filter']['projectcategory'] && $_SESSION['stormproject_list_filter']['projectcategory'] != '-') {
    $where[] = "spr.projectcategory='". $_SESSION['stormproject_list_filter']['projectcategory'] ."'";
  }
  if ($_SESSION['stormproject_list_filter']['projectstatus'] && $_SESSION['stormproject_list_filter']['projectstatus'] != '-') {
    $v = $_SESSION['stormproject_list_filter']['projectstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "spr.projectstatus IN ('". $v ."')";
  }
  if ($_SESSION['stormproject_list_filter']['projectpriority'] && $_SESSION['stormproject_list_filter']['projectpriority'] != '-') {
    $where[] = "spr.projectpriority='". $_SESSION['stormproject_list_filter']['projectpriority'] ."'";
  }
  $itemsperpage = $_SESSION['stormproject_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);

  $s = stormproject_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[] = $project;
  }

  $o .= theme('stormproject_list', $header, $projects);
  $o .= theme('pager', NULL, 10, 0);
  print theme('page', $o);
}

function stormproject_list_filter() {
  $organization_nid = $_SESSION['stormproject_list_filter']['organization_nid'];
  $projectcategory = $_SESSION['stormproject_list_filter']['projectcategory'];
  $status_list = stormattribute_attributes_bydomain('project status search');
  $projectstatus = $_SESSION['stormproject_list_filter']['projectstatus'];
  if (!$projectstatus) {
    $rstatus_list = array_flip($status_list);
    $projectstatus = $rstatus_list['open'];
    $_SESSION['stormproject_list_filter']['projectstatus'] = $projectstatus;
  }
  $priority_list = stormattribute_attributes_bydomain('project priority search');
  $projectpriority = $_SESSION['stormproject_list_filter']['projectpriority'];
  if (!$projectpriority) {
    $rpriority_list = array_flip($priority_list);
    $projectpriority = $rpriority_list['-'];
    $_SESSION['stormproject_list_filter']['projectpriority'] = $projectpriority;
  }
  $itemsperpage = $_SESSION['stormproject_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormproject_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#theme' => 'stormproject_list_filter',
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title ";
  $s = db_rewrite_sql($s);
  $r = db_query(stormorganization_access_sql($s));

  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $form['filter']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => array(0 => t('All')) + $organizations,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['projectcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $projectcategory,
    '#options' => array('-' => t('all')) + stormattribute_attributes_bydomain('Project category'),
  );

  $form['filter']['group1']['projectstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $projectstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group1']['projectpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $projectpriority,
    '#options' => array('-' => t('all')) + $priority_list,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormproject_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormproject_list_filter_reset'),
  );

  $form['filter']['group2']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormproject_list_filter_submit($form_id, $form_values) {
  if ($form_values['op'] == t('Filter')) {
    stormproject_list_filter_filter($form_id, $form_values);
  }
  else if ($form_values['op'] == t('Reset')) {
    stormproject_list_filter_reset($form_id, $form_values);
  }
}

function stormproject_list_filter_filter($form_id, $form_values) {
  $_SESSION['stormproject_list_filter']['organization_nid'] = $form_values['organization_nid'];
  $_SESSION['stormproject_list_filter']['projectcategory'] = $form_values['projectcategory'];
  $_SESSION['stormproject_list_filter']['projectstatus'] = $form_values['projectstatus'];
  $_SESSION['stormproject_list_filter']['projectpriority'] = $form_values['projectpriority'];
  $_SESSION['stormproject_list_filter']['itemsperpage'] = $form_values['itemsperpage'];
}

function stormproject_list_filter_reset($form_id, $form_valuese) {
  unset($_SESSION['stormproject_list_filter']);
}

function _stormproject_organization_projects_js($organization_nid=0) {
  $projects = array();

  if ($organization_nid) {
    $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormproject} AS spr ON n.vid=spr.vid WHERE n.status=1 AND n.type='stormproject' AND spr.organization_nid=%d ORDER BY n.title";
    $s = stormproject_access_sql($s);
    $s = db_rewrite_sql($s);
    $r = db_query($s, $organization_nid);

    while ($item = db_fetch_object($r)) {
      $nid = $item->nid;
      $projects[$nid] = $item->title;
    }
  }
  print drupal_to_js($projects);
  exit();
}
