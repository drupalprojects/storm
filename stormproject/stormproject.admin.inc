<?php
// $Id$

function stormproject_list() {
  $i = new stdClass();
  $i->type = 'stormproject';

  $o = drupal_get_form('stormproject_list_filter');

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
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.nid, n.title, n.changed, spr.* FROM {stormproject} spr INNER JOIN {node} n ON n.nid=spr.nid WHERE n.status=1 AND n.type='stormproject'";

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
  $itemsperpage = $_SESSION['stormproject_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);
  $sql = stormproject_access_sql($s, $where) . $tablesort;
  $r = pager_query($sql, $itemsperpage, 0, NULL);

  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[] = $project;
  }

  $o .= theme('stormproject_list', $header, $projects);
  $o .= theme('pager', NULL, 10, 0);
  print theme('page', $o);
}

function stormproject_list_filter() {
  $status_list = stormattribute_attributes_bydomain('project status search');
  $organization_nid = $_SESSION['stormproject_list_filter']['organization_nid'];
  $projectcategory = $_SESSION['stormproject_list_filter']['projectcategory'];
  $projectstatus = $_SESSION['stormproject_list_filter']['projectstatus'];
  if (!$projectstatus) {
    $rstatus_list = array_flip($status_list);
    $projectstatus = $rstatus_list['open'];
    $_SESSION['stormproject_list_filter']['projectstatus'] = $projectstatus;
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

  $s = "SELECT nid, title FROM {node} n WHERE status=1 AND type='stormorganization' ORDER BY title ";
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

  $form['filter']['projectcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $projectcategory,
    '#options' => array('-' => t('all')) + stormattribute_attributes_bydomain('Project category'),
  );

  $form['filter']['projectstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $projectstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per page'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
  );

  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter')
  );

  return $form;
}

function stormproject_list_filter_submit($form, &$form_state) {
  $_SESSION['stormproject_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormproject_list_filter']['projectcategory'] = $form_state['values']['projectcategory'];
  $_SESSION['stormproject_list_filter']['projectstatus'] = $form_state['values']['projectstatus'];
  $_SESSION['stormproject_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function _stormproject_organization_projects_js($organization_nid=0) {
  $projects = array();

  if ($organization_nid) {
    $r = db_query(stormproject_access_sql("SELECT n.nid, n.title FROM {stormproject} spr INNER JOIN {node} n ON n.nid=spr.nid WHERE n.status=1 AND n.type='stormproject' AND spr.organization_nid=%d ORDER BY n.title"), $organization_nid);

    while ($item = db_fetch_object($r)) {
      $nid = $item->nid;
      $projects[$nid] = $item->title;
    }
  }
  print drupal_to_js($projects);
  exit();
}
