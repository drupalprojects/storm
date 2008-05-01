<?php
// $Id$

function theme_stormorganization_list($header, $organizations) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  $countries = stormattribute_attributes_bydomain('Country');
  foreach ($organizations as $key => $organization) {
    $n = new stdClass();
    $n->nid = $organization->nid;
    $n->uid = $organization->uid;
    $n->type = 'stormorganization';

    $rows[] = array(
      l($organization->title, 'node/'. $organization->nid),
      $countries[$organization->country],
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}
