<?php
// $Id$

/**
 * @file
 * Provides implementations of Drupal hooks: menu, permission, theme
 */

/**
 * @function
 * Implementation of hook_menu
 */
function stormattribute_menu() {
  $items = array();
  
  $items['storm/attributes'] = array(
    'title' => t('Attributes'),
    'description' => t('Storm Attributes'),
    'page callback' => 'stormattribute_list',
    'access arguments' => array('Storm attribute: access'),
    'type' => MENU_NORMAL_ITEM);
  
  $items['storm/attributes/add'] = array(
    'title' => t('Add a new attribute'),
    'description' => t('Storm Attributes'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('stormattribute_add'),
    'access arguments' => array('Storm attribute: add'),
    'type' => MENU_CALLBACK);
  
  $items['storm/attributes/edit/%'] = array(
    'title' => t('Edit an attribute'),
    'description' => t('Storm Attributes'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('stormattribute_edit', 3),
    'access arguments' => array('Storm attribute: edit'),
    'type' => MENU_CALLBACK);
  
  $items['storm/attributes/delete/%'] = array(
    'title' => t('Delete an attribute'),
    'description' => t('Storm Attributes'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('stormattribute_delete', 3),
    'access arguments' => array('Storm attribute: delete'),
    'type' => MENU_CALLBACK);
  
  $items['storm/attributes/domain/autocomplete'] = array(
    'title' => t('List of attributes'),
    'description' => t('Storm Attributes'),
    'page callback' => '_stormattribute_domain_autocomplete',
    'page arguments' => array(4),
    'access arguments' => array('Storm attribute: access'),
    'type' => MENU_CALLBACK);
  
  return $items;
}

/**
 * @function
 * Implementation of hook_permission
 */
function stormattribute_permission() {
  return array(
    'Storm attribute: access',
    'Storm attribute: add',
    'Storm attribute: edit',
    'Storm attribute: delete',
  );
}

/**
 * @function
 * Implementation of hook_theme
 */
function stormattribute_theme() {
  return array(
    'stormattribute_list' => array(
      'arguments' => array('form'),
    ),
  );
}
