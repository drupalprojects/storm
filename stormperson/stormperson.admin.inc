<?php
// $Id$

/**
 * @file
 */

function stormperson_people($organization) {
  $i = new stdClass();
  $i->type = 'stormperson';

  $o = drupal_get_form('stormperson_people_filter');

  $params = $_GET;
  $params['organization_nid'] = $organization->nid;

  $header = array(
    array(
      'data' => t('Name'),
    ),
    array(
      'data' => t('Email'),
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, spe.* FROM {stormperson} spe INNER JOIN {node} n ON n.nid=spe.nid WHERE n.status=1 AND n.type='stormperson' ";
  $where[] = 'spe.organization_nid='. $organization->nid;
  if ($_SESSION['stormperson_people_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%%". $_SESSION['stormperson_people_filter']['name'] ."%%')";
  }

  $s = stormperson_access_sql($s, $where);
  $s = db_rewrite_sql($s, 'stormperson', 'nid');

  $tablesort = tablesort_sql($header);
  $r = db_query($s . $tablesort);

  $people = array();
  while ($person = db_fetch_object($r)) {
    $people[] = $person;
  }

  $o .= theme('stormperson_people', $header, $people);
  print theme('page', $o);
}

function stormperson_people_filter() {
  $name = $_SESSION['stormperson_people_filter']['name'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $name,
    '#size' => 30,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormperson_people_filter_filter'),
  );

  $form['filter']['group1']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormperson_people_filter_reset'),
  );

  return $form;
}

function stormperson_people_filter_filter($form, &$form_state) {
  $_SESSION['stormperson_people_filter']['name'] = $form_state['values']['name'];
}

function stormperson_people_filter_reset($form, &$form_state) {
  unset($_SESSION['stormperson_people_filter']);
}

function stormperson_list() {
  $i = new stdClass();
  $i->type = 'stormperson';

  $o = drupal_get_form('stormperson_list_filter');

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'spe.organization_title',
    ),
    array(
      'data' => t('Name'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Email'),
      'field' => 'spe.email',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, spe.* FROM {stormperson} spe INNER JOIN {node} n ON n.nid=spe.nid WHERE n.status=1 AND n.type='stormperson' ";
  if ($_SESSION['stormperson_list_filter']['organization_nid']) {
    $where[] = 'spe.organization_nid='. $_SESSION['stormperson_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormperson_list_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%%". $_SESSION['stormperson_list_filter']['name'] ."%%')";
  }
  $itemsperpage = $_SESSION['stormperson_list_filter']['itemsperpage'];

  $s = stormperson_access_sql($s, $where);
  $s = db_rewrite_sql($s, 'stormperson', 'nid');

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $people = array();
  while ($person = db_fetch_object($r)) {
    $people[] = $person;
  }

  $o .= theme('stormperson_list', $header, $people);
  $o .= theme('pager', NULL, $itemperpage, 0);
  print theme('page', $o);
}

function stormperson_list_filter() {
  $organization_nid = $_SESSION['stormperson_list_filter']['organization_nid'];
  $name = $_SESSION['stormperson_list_filter']['name'];

  $itemsperpage = $_SESSION['stormperson_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormperson_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {stormorganization} sor INNER JOIN {node} n ON sor.nid=n.nid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $r = db_query(stormorganization_access_sql($s));
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => t('All')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
  );

  $form['filter']['group1']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $name,
    '#size' => 30,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormperson_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormperson_list_filter_reset'),
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

function stormperson_list_filter_filter($form, &$form_state) {
  $_SESSION['stormperson_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormperson_list_filter']['name'] = $form_state['values']['name'];
  $_SESSION['stormperson_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormperson_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormperson_list_filter']);
}

