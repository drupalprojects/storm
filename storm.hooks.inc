<?php
// $Id$

/**
 * @file
 * Implementations of hooks (help, menu, permission, theme)
 */

// TODO: haven't added init as hope to rid of it
// TODO: move sql hooks from .module

/**
 * Implementation of hook_help
 */
function storm_help($path, $arg) {
  $output = '';

  switch ($path) {
    case 'admin/help#storm':
      $output = '<p>'.  t('Provides a complete project management environment') .'</p>';
      break;
  }
  return $output;
}

/**
 * Implementation of hook_menu
 */
function storm_menu() {
  $items = array();

  $items['storm'] = array(
    'title' => t('Storm'),
    'description' => t('Storm Dashboard'),
    'page callback' => 'storm_dashboard',
    'access arguments' => array('Storm: access dashboard'),
    'parent' => '',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['admin/storm'] = array(
    'title' => t('Storm'),
    'description' => t('Storm Administration Page'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('storm_admin_settings'),
    'access arguments' => array('access administration pages'),
    'type' => MENU_NORMAL_ITEM,
  );
  
  $items['admin/storm/storm'] = array(
    'title' => t('Storm'),
    'description' => t('Storm Administration Page'),
    'access arguments' => array('access administration pages'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -100,
  );
  
  return $items;
}

/**
 * Implementation of hook_permission
 */
function storm_permission() {
  return array(
    'Storm: access dashboard' => array(
      'title' => t('Storm: access dashboard'),
      'description' => t('View Storm dashboard'),
  );
}

/**
 * Implementation of hook_theme
 */
function storm_theme() {
  return array(
    'storm_form_group' => array(
      'arguments' => array('header', 'form'),
    ),
    'datetime' => array(
      'arguments' => array('element'),
    ),
    'dateext' => array(
      'arguments' => array('element'),
    ),
    'storm_list_report' => array(
      'arguments' => array('header', 'rows', 'title', 'footer', 'headtitle'),
    ),
    'storm_report' => array(
      'arguments' => array('header', 'content', 'title', 'footer'),
    ),
    'storm_view_item' => array(
      'arguments' => array('label', 'value'),
    ),
    'storm_dashboard' => array(
      'arguments' => array(),
    ),
  );
}