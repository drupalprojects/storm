<?php
// $Id$

/**
 * @file
 */

function stormtask_tasks($project) {
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
      'data' => t('Category'),
    ),
    array(
      'data' => t('Status'),
    ),
    array(
      'data' => t('Priority'),
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  if ($_SESSION['stormtask_tasks_filter']['taskcategory'] && $_SESSION['stormtask_tasks_filter']['taskcategory'] != '-') {
    $v = $_SESSION['stormtask_tasks_filter']['taskcategory'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskcategory IN ('". $v ."')";
  }
  if ($_SESSION['stormtask_tasks_filter']['taskstatus'] && $_SESSION['stormtask_tasks_filter']['taskstatus'] != '-') {
    $v = $_SESSION['stormtask_tasks_filter']['taskstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskstatus IN ('". $v ."')";
  }
  if ($_SESSION['stormtask_tasks_filter']['taskpriority'] && $_SESSION['stormtask_tasks_filter']['taskpriority'] != '-') {
    $v = $_SESSION['stormtask_tasks_filter']['taskpriority'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskpriority IN ('". $v ."')";
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
  $category_list = stormattribute_attributes_bydomain('Task category');
  $status_list = stormattribute_attributes_bydomain('Task status search');
  $priority_list = stormattribute_attributes_bydomain('Task priority search');

  $taskcategory = $_SESSION['stormtask_tasks_filter']['taskcategory'];
  $taskstatus = $_SESSION['stormtask_tasks_filter']['taskstatus'];
  $taskpriority = $_SESSION['stormtask_tasks_filter']['taskpriority'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $taskcategory,
    '#options' => array('-' => t('all')) + $category_list,
  );

  $form['filter']['group1']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group1']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $taskpriority,
    '#options' => array('-' => t('all')) + $priority_list,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormtask_tasks_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormtask_tasks_filter_reset'),
  );

  return $form;
}

function stormtask_tasks_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_tasks_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_tasks_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_tasks_filter']['taskpriority'] = $form_state['values']['taskpriority'];
}

function stormtask_tasks_filter_reset($form, &$form_state) {
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
      'data' => t('Category'),
      'field' => 'sta.taskcategory',
    ),
    array(
      'data' => t('Status'),
      'field' => 'sta.taskstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'sta.taskpriority',
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
  if ($_SESSION['stormtask_list_filter']['taskcategory'] && $_SESSION['stormtask_list_filter']['taskcategory'] != '-') {
    $v = $_SESSION['stormtask_list_filter']['taskcategory'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskcategory IN ('". $v ."')";
  }
  if ($_SESSION['stormtask_list_filter']['taskstatus'] && $_SESSION['stormtask_list_filter']['taskstatus'] != '-') {
    $v = $_SESSION['stormtask_list_filter']['taskstatus'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskstatus IN ('". $v ."')";
  }
  if ($_SESSION['stormtask_list_filter']['taskpriority'] && $_SESSION['stormtask_list_filter']['taskpriority'] != '-') {
    $v = $_SESSION['stormtask_list_filter']['taskpriority'];
    $v = str_replace(',', "','", $v);
    $where[] = "sta.taskpriority IN ('". $v ."')";
  }
  $itemsperpage = $_SESSION['stormtask_list_filter']['itemsperpage'];

  $s = stormtask_access_sql($s, $where);
  $s = db_rewrite_sql($s);

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
  $category_list = stormattribute_attributes_bydomain('Task category');
  $status_list = stormattribute_attributes_bydomain('Task status search');
  $priority_list = stormattribute_attributes_bydomain('Task priority search');

  $organization_nid = $_SESSION['stormtask_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormtask_list_filter']['project_nid'];
  $taskcategory = $_SESSION['stormtask_list_filter']['taskcategory'];
  $taskstatus = $_SESSION['stormtask_list_filter']['taskstatus'];
  $taskpriority = $_SESSION['stormtask_list_filter']['taskpriority'];

  if (!$taskcategory) {
    $rcategory_list = array_flip($category_list);
    $taskcategory = $rcategory_list['open'];
    $_SESSION['stormtask_list_filter']['taskcategory'] = $taskcategory;
  }

  if (!$taskstatus) {
    $rstatus_list = array_flip($status_list);
    $taskstatus = $rstatus_list['open'];
    $_SESSION['stormtask_list_filter']['taskstatus'] = $taskstatus;
  }

  if (!$taskpriority) {
    $rpriority_list = array_flip($priority_list);
    $taskpriority = $priority_list['open'];
    $_SESSION['stormtask_list_filter']['taskpriority'] = $taskpriority;
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
    '#process' => array('storm_dependent_select_process'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $taskcategory,
    '#options' => array('-' => t('all')) + $category_list,
  );

  $form['filter']['group2']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => t('all')) + $status_list,
  );

  $form['filter']['group2']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $taskpriority,
    '#options' => array('-' => t('all')) + $priority_list,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormtask_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormtask_list_filter_reset'),
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

function stormtask_list_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormtask_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormtask_list_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_list_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_list_filter']['taskpriority'] = $form_state['values']['taskpriority'];
  $_SESSION['stormtask_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormtask_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormtask_list_filter']);
}

