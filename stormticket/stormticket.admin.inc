<?php
// $Id$
function stormticket_list() {
  $i = new stdClass();
  $i->type = 'stormticket';

  $o = drupal_get_form('stormticket_list_filter');

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
      'data' => t('Task'),
      'field' => 'sti.task_title',
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
      'data' => storm_icon('user_comment', t('Comments')),
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.nid, n.title, n.uid, n.changed, n.created, sti.organization_title, sti.organization_nid, sti.project_title, sti.project_nid, sti.task_title, sti.task_nid, sti.ticketcategory, sti.ticketstatus, sti.ticketpriority FROM {stormticket} sti INNER JOIN {node} n ON n.nid=sti.nid WHERE n.status=1 AND n.type='stormticket' ";
  if ($_SESSION['stormticket_list_filter']['organization_nid']) {
    $where[] = 'sti.organization_nid='. $_SESSION['stormticket_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['project_nid']) {
    $where[] = 'sti.project_nid='. $_SESSION['stormticket_list_filter']['project_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['task_nid']) {
    $where[] = 'sti.task_nid='. $_SESSION['stormticket_list_filter']['task_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['ticketstatus'] && $_SESSION['stormticket_list_filter']['ticketstatus'] != '-') {
    $v = $_SESSION['stormticket_list_filter']['ticketstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "sti.ticketstatus IN ('". $v ."')";
  }
  if ($_SESSION['stormticket_list_filter']['ticketpriority'] && $_SESSION['stormticket_list_filter']['ticketpriority'] != '-') {
    $v = $_SESSION['stormticket_list_filter']['ticketpriority'];
    $v = str_replace(',', "','", $v);
    $where[] = "sti.ticketpriority IN ('". $v ."')";
  }
  $itemsperpage = $_SESSION['stormticket_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);
  $r = pager_query(stormticket_access_sql($s, $where) . $tablesort, $itemsperpage, 0, NULL);

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

  $s = "SELECT n.nid, n.title FROM {stormorganization} sor INNER JOIN {node} n ON sor.nid=n.nid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
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
    '#attributes' => array('onchange' => "stormtask_organization_project_tasks(this, 'edit-project-nid', 'edit-task-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {stormproject} spr INNER JOIN {node} n ON spr.nid=n.nid 
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
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormtask_project_tasks(this, 'edit-task-nid', true, 'All')"),
  );

  $r = db_query(stormtask_access_sql("SELECT n.nid, n.title FROM {stormtask} sta INNER JOIN {node} n ON sta.nid=n.nid WHERE status=1 AND type='stormtask' AND sta.project_nid=%d ORDER BY title "), $project_nid);
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

function stormticket_list_filter_submit($form, &$form_state) {
  $_SESSION['stormticket_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormticket_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormticket_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormticket_list_filter']['ticketstatus'] = $form_state['values']['ticketstatus'];
  $_SESSION['stormticket_list_filter']['ticketpriority'] = $form_state['values']['ticketpriority'];
  $_SESSION['stormticket_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function _stormticket_task_tickets_js($organization_nid=0, $project_nid=0, $task_nid=0) {
  $tickets = array();

  $r = db_query(stormticket_access_sql("SELECT n.nid, n.title FROM {stormticket} sti INNER JOIN {node} n 
  ON n.nid=sti.nid WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d 
  AND sti.task_nid=%d ORDER BY n.title"), $organization_nid, $project_nid, $task_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $tickets[$nid] = $item->title;
  }
  print drupal_to_js($tickets);
  exit();
}
