<?php

/**
 * @file
 */

function stormperson_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET) &&
    ($_SESSION['stormperson_list_filter']['organization_nid'] != $_GET['organization_nid'])) {
    $_SESSION['stormperson_list_filter']['organization_nid'] = $_GET['organization_nid'];
  }

  $i = new stdClass();
  $i->type = 'stormperson';

  $params = $_GET;
  $params['organization_nid'] = isset($_SESSION['stormperson_list_filter']['organization_nid']) ? $_SESSION['stormperson_list_filter']['organization_nid'] : NULL;

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
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();
  $filterfields = array();

  $query = db_select('node', 'n');
  $query->join('stormperson', 'spe', 'n.vid = spe.vid');
  $query
    ->fields('n')
    ->fields('spe', array('user_uid', 'organization_nid', 'organization_title', 'email'))
    ->condition('n.status', 1)
    ->condition('n.type', 'stormperson');

  if (isset($_SESSION['stormperson_list_filter']['organization_nid']) && $_SESSION['stormperson_list_filter']['organization_nid'] != 0) {
    $where[] = 'spe.organization_nid=%d';
    $args[] = $_SESSION['stormperson_list_filter']['organization_nid'];
    $filterfields[] = t('Organization');
  }
  if (isset($_SESSION['stormperson_list_filter']['name']) && $_SESSION['stormperson_list_filter']['name'] != '') {
    $where[] = "LOWER(n.title) LIKE LOWER('%%%s%%')";
    $args[] = $_SESSION['stormperson_list_filter']['name'];
    $filterfields[] = t('Name');
  }

  // Sets $itemsperpage for fieldset label, but this section does not control storing of session variable.
  $itemsperpage = isset($_SESSION['stormperson_list_filter']['itemsperpage']) ? $_SESSION['stormperson_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $stormperson_list_filter = drupal_get_form('stormperson_list_filter', $filterdesc);

  $o = drupal_render($stormperson_list_filter);

//  $s = stormperson_access_sql($s, $where);
//  $s = db_rewrite_sql($s);

//  $tablesort = tablesort_sql($header);
//  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $result = $query->execute();

  $people = array();
  foreach ($result as $person) {
    $people[] = $person;
  }

  $o .= theme('stormperson_list', array('header' => $header, 'people' => $people));
//  $o .= theme('pager', NULL, $itemsperpage, 0);
  return $o;
}

function stormperson_list_filter($form, &$form_state, $filterdesc = 'Filter') {
  $organization_nid = isset($_SESSION['stormperson_list_filter']['organization_nid']) ? $_SESSION['stormperson_list_filter']['organization_nid'] : 0;
  $name = isset($_SESSION['stormperson_list_filter']['name']) ? $_SESSION['stormperson_list_filter']['name'] : '';

  $itemsperpage = isset($_SESSION['stormperson_list_filter']['itemsperpage']) ? $_SESSION['stormperson_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormperson_list_filter']['itemsperpage'] = $itemsperpage;

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $filterdesc,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
//    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.status', 1)
    ->condition('n.type', 'stormorganization')
    ->orderBy('title', 'ASC');

//  $s = stormorganization_access_sql($s);
//  $s = db_rewrite_sql($s);

  $result = $query->execute();

  $organizations = array();
  foreach ($result as $organization) {
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
//    '#theme' => 'storm_form_group',
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

function _stormperson_organization_people_js($organization_nid=0) {
  $people = array();

  if ($organization_nid) {
      $query = db_select('node', 'n')
        ->fields('n', array('nid', 'title'))
        ->condition('n.status', 1)
        ->condition('n.type', 'organization')
        ->condition('spe.organization', $organization_nid)
        ->join('stormperson', 'spe', 'n.vid = spe.vid')
        ->addTag('node_access');

  $result = $query->execute();
    foreach($result as $row) {
      $nid = $row->name;
      $people[$nid] = $row->title;
    }
  }
  print drupal_json_encode($people);
  exit();
}

