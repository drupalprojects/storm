<?php
// $Id$

/**
 * @file
 */

function stormtask_tasks($project_nid) {
  $project = node_load($project_nid);
  $i = new stdClass();
  $i->type = 'stormtask';

  $o = drupal_get_form('stormtask_tasks_filter');

  $params = $_GET;
  $params['organization_nid'] = $project->organization_nid;
  $params['project_nid'] = $project->nid;

  $header = array(
    array(
      'data' => t('Description'),
    ),
    array(
      'data' => t('Status'),
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  if ($_SESSION['stormtask_tasks_filter']['taskstatus'] && $_SESSION['stormtask_tasks_filter']['taskstatus'] != '-') {
    $v = $_SESSION['stormtask_tasks_filter']['taskstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskstatus IN ('". $v ."')";
  }

  $tree = _stormtask_get_tree($project->nid, 0, -1, NULL, $where);
  $taskstree = _stormtask_plain_tree($tree);

  $tasks = array();
  if ($taskstree) {
    foreach ($taskstree as $nid => $indentstepno) {
      $task = node_load($nid);
      $task->indentstepno = $indentstepno;
      $tasks[] = $task;
    }
  }
  $o .= theme('stormtask_tasks', $header, $tasks);
  print theme('page', $o);
}

function stormtask_tasks_filter() {
  $status_list = stormattribute_attributes_bydomain('task status search');
  $taskstatus = $_SESSION['stormtask_tasks_filter']['taskstatus'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['filter']['group1']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

function stormtask_tasks_filter_submit($form_id, $form_values) {
  if ($form_values['op'] == t('Filter')) {
    stormtask_tasks_filter_filter($form_id, $form_values);
  }
  else if ($form_values['op'] == t('Reset')) {
    stormtask_tasks_filter_reset($form_id, $form_values);
  }
}

function stormtask_tasks_filter_filter($form_id, $form_values) {
  $_SESSION['stormtask_tasks_filter']['taskstatus'] = $form_values['taskstatus'];
}

function stormtask_tasks_filter_reset($form_id, $form_values) {
  unset($_SESSION['stormtask_tasks_filter']);
}

function _stormtask_project_tasks_js($project_nid) {
  $tree = _stormtask_get_tree($project_nid);
  $tasks = _stormtask_plain_tree($tree);
  print drupal_to_js($tasks);
  exit();
}

function stormtask_list() {
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormtask_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormtask_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormtask_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormtask_list_filter']['organization_nid'] = $p->organization_nid;

  }

  $o = drupal_get_form('stormtask_list_filter');

  $i = new stdClass();
  $i->type = 'stormtask';

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sta.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sta.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Status'),
      'field' => 'sta.taskstatus',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, sta.* FROM {node} AS n INNER JOIN {stormtask} AS sta ON n.vid=sta.vid WHERE n.status=1 AND n.type='stormtask' ";
  if ($_SESSION['stormtask_list_filter']['organization_nid']) {
    $where[] = 'sta.organization_nid='. $_SESSION['stormtask_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormtask_list_filter']['project_nid']) {
    $where[] = 'sta.project_nid='. $_SESSION['stormtask_list_filter']['project_nid'];
  }
  if ($_SESSION['stormtask_list_filter']['taskstatus'] && $_SESSION['stormtask_list_filter']['taskstatus'] != '-') {
    $v = $_SESSION['stormtask_list_filter']['taskstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskstatus IN ('". $v ."')";
  }
  $itemsperpage = $_SESSION['stormtask_list_filter']['itemsperpage'];

  $s = db_rewrite_sql($s);
  $s = stormtask_access_sql($s, $where);
  $s = db_rewrite_sql($s, 'stormtask', 'nid');

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $tasks = array();
  while ($item = db_fetch_object($r)) {
    $tasks[] = $item;
  }

  $o .= theme('stormtask_list', $header, $tasks);
  $o .= theme('pager', NULL, $itemperpage, 0);
  print theme('page', $o);
}

function stormtask_list_filter() {
  $status_list = stormattribute_attributes_bydomain('task status search');

  $organization_nid = $_SESSION['stormtask_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormtask_list_filter']['project_nid'];
  $taskstatus = $_SESSION['stormtask_list_filter']['taskstatus'];

  if (!$taskstatus) {
    $rstatus_list = array_flip($status_list);
    $taskstatus = $rstatus_list['open'];
    $_SESSION['stormtask_list_filter']['taskstatus'] = $taskstatus;
  }

  $itemsperpage = $_SESSION['stormtask_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormtask_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
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
  $r = db_query(stormproject_access_sql($s), $organization_nid);
  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[$project->nid] = $project->title;
  }
  $projects = array(0 => t('All')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process' => array()),
  );

  $form['filter']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
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

function stormtask_list_filter_submit($form_id, $form_values) {
  if ($form_values['op'] == t('Filter')) {
    stormtask_list_filter_filter($form_id, $form_values);
  }
  else if ($form_values['op'] == t('Reset')) {
    stormtask_list_filter_reset($form_id, $form_values);
  }
}

function stormtask_list_filter_filter($form_id, $form_values) {
  $_SESSION['stormtask_list_filter']['organization_nid'] = $form_values['organization_nid'];
  $_SESSION['stormtask_list_filter']['project_nid'] = $form_values['project_nid'];
  $_SESSION['stormtask_list_filter']['taskstatus'] = $form_values['taskstatus'];
  $_SESSION['stormtask_list_filter']['itemsperpage'] = $form_values['itemsperpage'];
}

function stormtask_list_filter_reset($form_id, $form_values) {
  unset($_SESSION['stormtask_list_filter']);
}

