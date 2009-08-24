<?php
// $Id$

/**
 * @file
 * Implementations of hooks (help, menu, permission, theme)
 */

// TODO: move sql hooks from .module

/**
 * Implementation of hook_init
 */
// Note #370120. It is intended to move these calls to pages which specifically need them rather than on hook_init.
function storm_init() {
  //drupal_add_js(drupal_get_path('module', 'storm') .'/storm.js', 'module', 'header', FALSE);
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');
}

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
    'storm_link' => array(
      'arguments' => array(),
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
  );
}
