<?php
// $Id$

/**
 * @file
 */

function theme_stormtask_tasks($header, $tasks) {
  $rows = array();
  foreach ($tasks as $task) {
    $n = new stdClass();
    $n->nid = $task->nid;
    $n->uid = $task->uid;
    $n->type = 'stormtask';

    $rows[] = array(
      l($task->indentstepno, 'node/'. $task->nid, array('html' => TRUE)),
      storm_icon('category_'. $task->taskcategory, $task->taskcategory),
      storm_icon('status_'. $task->taskstatus, $task->taskstatus),
      storm_icon('priority_'. $task->taskpriority, $task->taskpriority),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormtask_list($header, $tasks) {
  $rows = array();
  foreach ($tasks as $task) {
    $n = new stdClass();
    $n->nid = $task->nid;
    $n->uid = $task->uid;
    $n->organization_nid = $task->organization_nid;
    $n->type = 'stormtask';

    $rows[] = array(
      l($task->organization_title, 'node/'. $task->organization_nid),
      l($task->project_title, 'node/'. $task->project_nid),
      l($task->title, 'node/'. $task->nid) . theme('mark', node_mark($task->nid, $task->changed)),
      storm_icon('category_'. $task->taskcategory, $task->taskcategory),
      storm_icon('status_'. $task->taskstatus, $task->taskstatus),
      storm_icon('priority_'. $task->taskpriority, $task->taskpriority),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormtask_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormtask'] = array(
    '#prefix' => '<div id="stormtask">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormtask']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'task'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormtask']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'task'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormtask']['stepno'] = array(
    '#prefix' => '<div class="stepno">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Step no.', 'task'), $node->stepno),
    '#weight' => $w++,
  );

  return $node;
}
