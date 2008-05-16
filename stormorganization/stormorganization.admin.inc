<?php
// $Id$

/**
 * @file
 */

function stormorganization_list() {
  $i = new stdClass();
  $i->type = 'stormorganization';
  $o = drupal_get_form('stormorganization_list_filter');

  $header = array(
    array(
      'data' => t('Name'),
      'field' => 'n.title',
      'sort' => 'ASC',
    ),
    array(
      'data' => t('Country'),
      'field' => 'sor.country',
      'sort' => '',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT n.*, sor.* FROM {stormorganization} sor INNER JOIN {node} n ON n.nid=sor.nid WHERE n.status=1 AND n.type='stormorganization'";
  $where = array();
  if ($_SESSION['stormorganization_list_filter']['country']) {
    $where[] = "sor.country='". $_SESSION['stormorganization_list_filter']['country'] ."'";
  }
  if ($_SESSION['stormorganization_list_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%%". $_SESSION['stormorganization_list_filter']['name'] ."%%')";
  }
  $itemsperpage = $_SESSION['stormorganization_list_filter']['itemsperpage'];

  $s = db_rewrite_sql($s, 'stormorganization', 'nid');
  $s = stormorganization_access_sql($s, $where);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[] = $organization;
  }

  $o .= theme('stormorganization_list', $header, $organizations);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormorganization_list_filter() {
  $country = $_SESSION['stormorganization_list_filter']['country'];
  $name = $_SESSION['stormorganization_list_filter']['name'];
  $itemsperpage = $_SESSION['stormorganization_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormorganization_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => -20,
  );

  $form['filter']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => $country,
    '#options' => array(0 => t('All')) + stormattribute_attributes_bydomain('Country'),
  );

  $form['filter']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $name,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormorganization_list_filter_filter'),
  );

  $form['filter']['group1']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormorganization_list_filter_reset'),
  );

  $form['filter']['group1']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormorganization_list_filter_filter($form, &$form_state) {
  $_SESSION['stormorganization_list_filter']['country'] = $form_state['values']['country'];
  $_SESSION['stormorganization_list_filter']['name'] = $form_state['values']['name'];
  $_SESSION['stormorganization_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormorganization_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormorganization_list_filter']);
}

