<?php

function theme_stormattribute_list($form) {
  drupal_add_tabledrag('attributes', 'order', 'sibling', 'attribute-weight'); 

  $header = array();
  $row = array();

  $header = $form['header']['#value'];
  unset($form['header']);

  foreach (element_children($form['attributes']) as $key) {
    $form['attributes'][$key]['attribute_weight_'. $key]['#attributes']['class'] = 'attribute-weight';
    $data = array (
      drupal_render($form['attributes'][$key]['attribute_domain_'. $key]),
      drupal_render($form['attributes'][$key]['attribute_akey_'. $key]),
      drupal_render($form['attributes'][$key]['attribute_avalue_'. $key]),
      drupal_render($form['attributes'][$key]['attribute_weight_'. $key]),
      array (
        'data' => drupal_render($form['attributes'][$key]['attribute_operations_'. $key]),
        'class' => 'storm_list_operations',
      ),
    );
    $row['data'] = $data;
    $row['class'] = empty($row['class']) ? 'draggable' : $row['class'] .' draggable';
    $rows[] = $row;
  }
  $o = theme('table', $header, $rows, array('id' => 'attributes'));
  return $o;
}

