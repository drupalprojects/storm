<?php
// $Id$

/**
 * @file
 */

function theme_stormknowledgebase_list($header, $notes) {
  $rows = array();

  foreach ($notes as $note) {
    $n = new stdClass();
    $n->nid = $note->nid;
    $n->uid = $note->uid;
    $n->type = 'stormknowledgebase';
    $rows[] = array(
      l($note->title, 'node/'. $note->nid),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}


