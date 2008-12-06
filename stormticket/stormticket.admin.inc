<?php
// $Id$

/**
 * @file
 */

function stormticket_list() {
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormticket_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormticket_list_filter']['project_nid']);
    unset($_SESSION['stormticket_list_filter']['task_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormticket_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormticket_list_filter']['organization_nid'] = $p->organization_nid;

    unset($_SESSION['stormticket_list_filter']['task_nid']);
  }

  if (array_key_exists('task_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['task_nid'] != $_GET['task_nid']) {
      $_SESSION['stormticket_list_filter']['task_nid'] = $_GET['task_nid'];
    }

    $t = node_load($_GET['task_nid']);
    $_SESSION['stormticket_list_filter']['organization_nid'] = $t->organization_nid;
    $_SESSION['stormticket_list_filter']['project_nid'] = $t->project_nid;
  }

  $o = drupal_get_form('stormticket_list_filter');

  $i = new stdClass();
  $i->type = 'stormticket';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'sti.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sti.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Date'),
      'field' => 'n.created',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Status'),
      'field' => 'sti.ticketstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'sti.ticketpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  $s = "SELECT n.*, sti.* FROM  {node} AS n INNER JOIN {stormticket} AS sti ON n.vid=sti.vid WHERE n.status=1 AND n.type='stormticket' ";
  if ($_SESSION['stormticket_list_filter']['organization_nid']) {
    $where[] = 'sti.organization_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['project_nid']) {
    $where[] = 'sti.project_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['project_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['task_nid']) {
    $where[] = 'sti.task_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['task_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['ticketstatus'] && $_SESSION['stormticket_list_filter']['ticketstatus'] != '-') {
    $status = split(',', $_SESSION['stormticket_list_filter']['ticketstatus']);
    $v = array();
    foreach($status as $item) $v[] = '%s';
    $where[] = "sti.ticketstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormticket_list_filter']['ticketpriority'] && $_SESSION['stormticket_list_filter']['ticketpriority'] != '-') {
    $priority = split(',', $_SESSION['stormticket_list_filter']['ticketpriority']);
    $v = array();
    foreach($priority as $item) $v[] = '%s';
    $where[] = "sti.ticketpriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key=>$value) {
      $args[] = $value;
    }
  }
  $itemsperpage = $_SESSION['stormticket_list_filter']['itemsperpage'];

  $s = stormticket_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $tickets = array();
  while ($item = db_fetch_object($r)) {
    $tickets[] = $item;
  }

  $o .= theme('stormticket_list', $header, $tickets);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormticket_list_filter() {
  $status_list = stormattribute_attributes_bydomain('ticket status search');

  $organization_nid = $_SESSION['stormticket_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormticket_list_filter']['project_nid'];
  $task_nid = $_SESSION['stormticket_list_filter']['task_nid'];
  $ticketstatus = $_SESSION['stormticket_list_filter']['ticketstatus'];
  if (!$ticketstatus) {
    $rstatus_list = array_flip($status_list);
    $ticketstatus = $rstatus_list['open'];
    $_SESSION['stormticket_list_filter']['ticketstatus'] = $ticketstatus;
  }
  $ticketpriority = $_SESSION['stormticket_list_filter']['ticketpriority'];

  $itemsperpage = $_SESSION['stormticket_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormticket_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#theme' => 'stormticket_list_filter',
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
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
    '#attributes' => array('onchange' => "stormtask_organization_project_tasks(this, 'edit-project-nid', 'edit-task-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid);
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
    '#attributes' => array('onchange' => "stormtask_project_tasks(this, 'edit-task-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormtask} AS sta
  ON sta.vid=n.vid WHERE n.status=1 AND type='stormtask' AND sta.project_nid=%d ORDER BY title ";
  $s = stormtask_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $project_nid);

  $tasks = array();
  while ($task = db_fetch_object($r)) {
    $tasks[$task->nid] = $task->title;
  }
  $tasks = array(0 => t('All')) + $tasks;
  $form['filter']['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => t('Task'),
    '#default_value' => $task_nid,
    '#options' => $tasks,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['ticketstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $ticketstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group2']['ticketpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $ticketpriority,
    '#options' => array('-' => t('all')) + stormattribute_attributes_bydomain('ticket priority search'),
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormticket_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormticket_list_filter_reset'),
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

function stormticket_list_filter_filter($form, &$form_state) {
  $_SESSION['stormticket_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormticket_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormticket_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormticket_list_filter']['ticketstatus'] = $form_state['values']['ticketstatus'];
  $_SESSION['stormticket_list_filter']['ticketpriority'] = $form_state['values']['ticketpriority'];
  $_SESSION['stormticket_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormticket_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormticket_list_filter']);
}

function _stormticket_task_tickets_js($organization_nid=0, $project_nid=0, $task_nid=0) {
  $tickets = array();

  $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormticket} sti
  ON n.vid=sti.vid WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d
  AND sti.task_nid=%d ORDER BY n.title";

  $s = stormticket_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid, $task_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $tickets[$nid] = $item->title;
  }
  print drupal_to_js($tickets);
  exit();
}
