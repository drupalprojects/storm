<?php
// $Id$

/**
 * @file
 */

function stormorganization_list() {
  if (array_key_exists('name', $_GET)) {
    if ($_SESSION['stormorganization_list_filter']['name'] != $_GET['name']) {
      $_SESSION['stormorganization_list_filter']['name'] = $_GET['name'];
    }
  }

  $o = drupal_get_form('stormorganization_list_filter');

  $i = new stdClass();
  $i->type = 'stormorganization';

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

  $s = "SELECT n.*, sor.* FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization'";
  $where = array();
  if ($_SESSION['stormorganization_list_filter']['country']) {
    $where[] = "sor.country='". $_SESSION['stormorganization_list_filter']['country'] ."'";
  }
  if ($_SESSION['stormorganization_list_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('". $_SESSION['stormorganization_list_filter']['name'] ."')";
  }
  $itemsperpage = $_SESSION['stormorganization_list_filter']['itemsperpage'];

  $s = stormorganization_access_sql($s, $where);
  $s = db_rewrite_sql($s);

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
    '#autocomplete_path' => 'storm/organizations/autocomplete',
  );

  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range(0, 9) as $n) {
    $qs .= '<li>'. l($n, 'storm/organizations', array('query' => array('name' => $n .'%'))) .'</li>';
  }
  $form['filter']['name_quick_shortcuts_numbers'] = array(
    '#type' => 'markup',
    '#value' => $qs,
  );
  $qs .= '</ul></div>';
  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range('A', 'Z') as $l) {
    $qs .= '<li>'. l($l, 'storm/organizations', array('query' => array('name' => $l .'%'))) .'</li>';
  }
  $qs .= '</ul></div>';
  $form['filter']['name_quick_shortcuts_letters'] = array(
    '#type' => 'markup',
    '#value' => $qs,
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

function stormorganization_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $s = "SELECT title FROM {node} AS n WHERE n.type='stormorganization' AND LOWER(title) LIKE LOWER('%s%%')";
    $s = stormorganization_access_sql($s);
    $s = db_rewrite_sql($s);

    $result = db_query_range($s, $string, 0, 10);
    while ($org = db_fetch_object($result)) {
      $matches[$org->title] = check_plain($org->title);
    }
  }

  drupal_json($matches);
}

