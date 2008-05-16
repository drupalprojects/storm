<?php
// $Id$

/**
 * @file
 */

function theme_stormperson_people($header, $people) {
  $rows = array();
  foreach ($people as $person) {
    $n = new stdClass();
    $n->nid = $person->nid;
    $n->uid = $person->uid;
    $n->type = 'stormperson';

    $rows[] = array(
      l($person->title, 'node/'. $person->nid),
      l($person->email, 'mailto:'. $person->email),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormperson_list($header, $people) {
  $rows = array();
  foreach ($people as $person) {
    $n = new stdClass();
    $n->nid = $person->nid;
    $n->uid = $person->uid;
    $n->organization_nid = $person->organization_nid;
    $n->type = 'stormperson';

    $rows[] = array(
      l($person->organization_title, 'node/'. $person->organization_nid),
      l($person->title, 'node/'. $person->nid) . theme('mark', node_mark($person->nid, $person->changed)),
      l($person->email, 'mailto:'. $person->email),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

