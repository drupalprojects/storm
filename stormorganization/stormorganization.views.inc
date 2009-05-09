<?php
// $Id:

function stormorganization_views_data() {
  $data['stormorganization']['table']['group']  = 'Storm';
  $data['stormorganization']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormorganization']['prefix'] = array(
    'title' => t('Prefix'),
    'help' => 'Storm Organization Prefix',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormorganization']['fullname'] = array(
    'title' => t('Fullname'),
    'help' => 'Storm Organization Fullname',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

  function stormorganization_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'stormattribute'),
    ),
    'handlers' => array(
      'stormattribute_handler_filter_attributes_domain' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
