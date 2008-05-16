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
//     l($task->indenttitle, 'node/'. $nid . theme('mark', node_mark($nid, $task->changed))),
      l($task->indentstepno, 'node/'. $task->nid, array('html' => TRUE)),
      storm_icon('status_'. $task->taskstatus, $task->taskstatus),
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
      storm_icon('status_'. $task->taskstatus, $task->taskstatus),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

