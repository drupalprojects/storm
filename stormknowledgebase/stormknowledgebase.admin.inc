<?php
// $Id$

/**
 * @file
 */

function stormknowledgebase_list() {
  $o = drupal_get_form('stormknowledgebase_list_filter');

  $i = new stdClass();
  $i->type = 'stormknowledgebase';

  $params = $_GET;

  $header = array(
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.nid, n.title FROM {node} AS n WHERE n.status=1 AND n.type='stormknowledgebase' ";
  $s = stormknowledgebase_access_sql($s);
  $s = db_rewrite_sql($s);
  $tablesort = tablesort_sql($header);

  $itemsperpage = $_SESSION['stormknowledgebase_list_filter']['itemsperpage'];
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $notes = array();
  while ($item = db_fetch_object($r)) {
    $notes[] = $item;
  }

  $o .= theme('stormknowledgebase_list', $header, $notes);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormknowledgebase_list_filter() {
  $itemsperpage = $_SESSION['stormknowledgebase_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormknowledgebase_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormknowledgebase_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormknowledgebase_list_filter_reset'),
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

function stormknowledgebase_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormknowledgebase_list_filter']);
}

function stormknowledgebase_list_filter_filter($form, &$form_state) {
  $_SESSION['stormknowledgebase_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}


