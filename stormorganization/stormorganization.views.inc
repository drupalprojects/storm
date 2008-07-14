<?php
// $Id

function stormorganization_views_tables() {
  $tables['stormorganization'] = array(
    'name' => 'stormorganization',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'vid'
      ),
      'right' => array(
        'field' => 'vid'
      ),
    ),
    'fields' => array(
      'prefix' => array(
        'name' => t('Storm Organization : Prefix'),
        'sortable' => true,
      ),
      'fullname' => array(
        'name' => t('Storm Organization : Fullname'),
        'sortable' => true,
      ),
      'country' => array(
        'name' => t('Storm Organization : Country'),
        'sortable' => true,
      ),
    ),
    'sorts' => array(
      'prefix' => array('name' => t('Storm Organization : Prefix')),
      'fullname' => array('name' => t('Storm Organization : Fullname')),
      'country' => array('name' => t('Storm Organization : Country'))
    ),
    'filters' => array(
      'prefix' => array(
        'name' => t('Storm Organization : Prefix'),
        'operator' => 'views_handler_operator_like',
        'handler' => 'views_handler_filter_like',
      ),
      'fullname' => array(
        'name' => t('Storm Organization : Fullname'),
        'operator' => 'views_handler_operator_like',
        'handler' => 'views_handler_filter_like',
      ),
    ),
  );
  return $tables;
}

