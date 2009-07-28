<?php
// $Id$

/**
 * @file
 * Defines the administration settings form for the Storm module
 *//
function storm_admin_settings() {
  $form = array();
  $w = -5;

  $form['storm_icons_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Icons directory'),
    '#default_value' => variable_get('storm_icons_path', drupal_get_path('module', 'storm') .'/icons'),
    '#description' => t("The directory that contains the Storm's icons"),
    '#weight' => $w++,
  );

  $form['storm_report_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Report header'),
    '#default_value' => variable_get('storm_report_header', ''),
    '#description' => t('The header that will appear on Storm reports'),
    '#weight' => $w++,
  );
  
  $form['yearsrange'] = array(
    '#type' => 'fieldset',
    '#title' => t('Years range in dates'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#weight' => $w++,
  );
  
  $form['yearsrange']['group0'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => $w++,
  );

  $form['yearsrange']['group0']['storm_yearsrangebegin'] = array(
    '#type' => 'select',
    '#title' => t('Begin'),
    '#options' => drupal_map_assoc(range(1970, 2037)),
    '#default_value' => variable_get('storm_yearsrangebegin', 2001),
  );

  $form['yearsrange']['group0']['storm_yearsrangeend'] = array(
    '#type' => 'select',
    '#title' => t('End'),
    '#options' => drupal_map_assoc(range(1970, 2037)),
    '#default_value' => variable_get('storm_yearsrangeend', 2012),
  );

  $form['taxation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxation defaults'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#weight' => $w++,
  );
  
  $form['taxation']['tax1app'] = array(
    '#type' => 'select',
    '#title' => t('Tax 1: Application'),
    '#default_value' => variable_get('storm_tax1_app', 'item'),
    '#description' => t('The method of application to use for Tax 1'),
    '#options' => array(
      1 => t('Apply to item amount'),
      0 => t('Do not apply tax'),
    ),
    '#weight' => $w++,
  );
  
  $form['taxation']['tax1name'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax 1: Name'),
    '#default_value' => variable_get('storm_tax1_name', 'VAT'),
    '#description' => t('The name to use for Tax 1'),
    '#weight' => $w++,
  );
  
  $form['taxation']['tax1percent'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax 1: Default percentage'),
    '#default_value' => variable_get('storm_tax1_percent', 20),
    '#description' => t('Default percentage for Tax 1'),
    '#size' => 5,
    '#weight' => $w++
  );
  
  $form['taxation']['tax2app'] = array(
    '#type' => 'select',
    '#title' => t('Tax 2: Application'),
    '#default_value' => variable_get('storm_tax2_app', 'none'),
    '#description' => t('The method of application to use for Tax 2'),
    '#options' => array(
      2 => t('Apply to total of item amount plus previous tax'),
      1 => t('Apply to item amount'),
      0 => t('Do not apply tax'),
    ),
    '#weight' => $w++,
  );
  
  $form['taxation']['tax2name'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax 2: Name'),
    '#default_value' => variable_get('storm_tax2_name', 'VAT'),
    '#description' => t('The name to use for Tax 2'),
    '#weight' => $w++,
  );
  
  $form['taxation']['tax2percent'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax 2: Default percentage'),
    '#default_value' => variable_get('storm_tax2_percent', 20),
    '#description' => t('Default percentage for Tax 2'),
    '#size' => 5,
    '#weight' => $w++,
  );

  return system_settings_form($form);
}
