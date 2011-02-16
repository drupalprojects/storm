<?php
// $Id$

function stormteam_views_data() {
  $data['stormteam']['table']['group']  = 'Storm Team';
  $data['stormteam']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );
  
  $data['stormteam']['mnid'] = array(
    'title' => t('Team Member Nid'),
    'help' => 'Storm Team Members',
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'parent' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_nid',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Member -> Node'),
    ),
  );
  
  return $data;
}