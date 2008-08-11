<?php
// $Id$

/**
 * @file
 */

function stormattribute_list() {
  $o = drupal_get_form('stormattribute_list_filter');

  $i = new stdClass();
  $i->type = 'stormattribute';

  $header = array(
    array(
      'data' => t('Domain'),
      'field' => 'domain',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Key'),
      'field' => 'akey',
    ),
    array(
      'data' => t('Value'),
      'field' => 'avalue',
      'sort' => '',
    ),
    array(
      'data' => storm_icon_add('storm/attributes/add', $i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT * FROM {stormattribute}";
  $where = array();

  if ($_SESSION['stormattribute_list_filter']['domain']) {
    $where[] = "domain='". $_SESSION['stormattribute_list_filter']['domain'] ."'";
  }

  if ($_SESSION['stormattribute_list_filter']['akey']) {
    $where[] = "LOWER(akey) LIKE LOWER('". $_SESSION['stormattribute_list_filter']['akey'] ."')";
  }

  if ($_SESSION['stormattribute_list_filter']['avalue']) {
    $where[] = "LOWER(avalue) LIKE LOWER('". $_SESSION['stormattribute_list_filter']['avalue'] ."')";
  }

  $itemsperpage = $_SESSION['stormattribute_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);

  $s = db_rewrite_sql($s, 'stormattribute', 'aid');
  $s = storm_rewrite_sql($s, $where) . $tablesort;
  $r = pager_query($s, $itemsperpage, 0, NULL);

  $attributes = array();
  while ($attribute = db_fetch_object($r)) {
    $attributes[] = $attribute;
  }

  $o .= theme('stormattribute_list', $header, $attributes);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormattribute_list_filter() {
  $r = db_query("SELECT DISTINCT domain FROM {stormattribute} ORDER BY domain");
  $domains = array(0 => t('All'));
  while ($domain = db_fetch_object($r)) {
    $domains[$domain->domain] = $domain->domain;
  }

  $domain = $_SESSION['stormattribute_list_filter']['domain'];
  if (!$domain) {
    $domain = 0;
    $_SESSION['stormattribute_list_filter']['domain'] = $domain;
  }
  $akey = $_SESSION['stormattribute_list_filter']['akey'];
  $avalue = $_SESSION['stormattribute_list_filter']['avalue'];

  $itemsperpage = $_SESSION['stormattribute_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormattribute_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['domain'] = array(
    '#type' => 'select',
    '#title' => t('Domain'),
    '#default_value' => $domain,
    '#options' => array('0' => t('All')) + _stormattibute_domain_options(),
  );

  $form['filter']['group1']['akey'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => $akey,
    '#size' => 20,
  );

  $form['filter']['group1']['avalue'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#default_value' => $avalue,
    '#size' => 20,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormattribute_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormattribute_list_filter_reset'),
  );

  $form['filter']['group2']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormattribute_list_filter_filter($form, &$form_state) {
  $_SESSION['stormattribute_list_filter']['domain'] = $form_state['values']['domain'];
  $_SESSION['stormattribute_list_filter']['akey'] = $form_state['values']['akey'];
  $_SESSION['stormattribute_list_filter']['avalue'] = $form_state['values']['avalue'];
  $_SESSION['stormattribute_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormattribute_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormattribute_list_filter']);
}

function stormattribute_add() {
  $attribute = array(
    'domain' => '',
    'akey' => '',
    'avalue' => '',
    'weight' => 0,
  );
  return stormattribute_form($attribute);
}

function stormattribute_add_submit($form, &$form_state) {
  db_query("INSERT INTO {stormattribute} (domain, akey, avalue, weight) VALUES ('%s', '%s', '%s', %d)", $form_state['values']['domain'], $form_state['values']['akey'], $form_state['values']['avalue'], $form_state['values']['weight']);

  if ($_REQUEST['destination']) {
    $destination = urldecode($_REQUEST['destination']);
  }
  else {
    $destination = 'storm/attributes';
  }
  drupal_goto($destination);
}

function stormattribute_edit($form_state, $aid) {
  $attribute = array();
  if ($aid) {
    $r = db_query("SELECT * FROM {stormattribute} WHERE aid=%d", $aid);
    $attribute = db_fetch_object($r);
  }
  return stormattribute_form($attribute);
}

function stormattribute_edit_submit($form, &$form_state) {
  db_query("UPDATE {stormattribute} SET domain='%s', akey='%s', avalue='%s', weight=%d WHERE aid=%d", $form_state['values']['domain'], $form_state['values']['akey'], $form_state['values']['avalue'], $form_state['values']['weight'], $form_state['values']['aid']);

  if ($_REQUEST['destination']) {
    $destination = urldecode($_REQUEST['destination']);
  }
  else {
    $destination = 'storm/attributes';
  }
  drupal_goto($destination);
}

function stormattribute_form_delete($form, &$form_state) {
  drupal_goto('storm/attributes/delete/'. $form_state['values']['aid']);
}

function stormattribute_delete($form_state, $aid) {
  $form['aid'] = array('#type' => 'value', '#value' => $aid);
  $r = db_query("SELECT * FROM {stormattribute} WHERE aid=%d", $aid);
  $a = db_fetch_object($r);
  $title = $a->domain .' : '. $a->avalue;

  return confirm_form($form,
  t('Are you sure you want to delete the attribute %title?', array('%title' => $title)),
  'storm/attributes/edit/'. $aid,
  t('This action cannot be undone.'),
  t('Delete'), t('Cancel'));
}

function stormattribute_delete_submit($form, &$form_state) {
  if ($form_state['values']['aid']) {
    db_query('DELETE FROM {stormattribute} WHERE aid=%d', $form_state['values']['aid']);
    drupal_set_message('Storm attribute deleted');
    if ($_REQUEST['destination']) {
      $destination = urldecode($_REQUEST['destination']);
    }
    else {
      $destination = 'storm/attributes';
    }
    drupal_goto($destination);
  }
}

function stormattribute_form($attribute = NULL) {
  $form = array();
  if ($attribute->aid) {
    $form['aid'] = array(
      '#type' => 'value',
      '#value' => $attribute->aid,
    );
  }

  $form['domain'] = array(
    '#type' => 'select',
    '#title' => t('Domain'),
    '#required' => TRUE,
    '#default_value' => $attribute->domain,
    '#options' => _stormattibute_domain_options(),
    '#weight' => -10
  );

  $form['akey'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#required' => TRUE,
    '#default_value' => $attribute->akey,
    '#weight' => -9
  );

  $form['avalue'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#required' => TRUE,
    '#default_value' => $attribute->avalue,
    '#weight' => -8
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $attribute->weight,
    '#weight' => -7
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  if ($attribute->aid) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('stormattribute_form_delete'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('stormattribute_cancel_submit'),
  );

  return $form;
}

function _stormattibute_domain_options() {
  static $options;
  if (!$options) {
    $r = db_query("SELECT DISTINCT(domain) AS domain_name FROM {stormattribute} ORDER BY domain");
    $options = array();
    while ($d = db_fetch_object($r)) {
      $options[$d->domain_name] = $d->domain_name;
    }
  }
  return $options;
}
