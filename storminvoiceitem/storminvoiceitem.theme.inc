<?php
// $Id$

/**
 * @file
 */

// FUNCTION MOVED TO STORM INVOICE AS PART OF #436922
/*function theme_storminvoiceitem_invoiceitems($form) {
  drupal_add_tabledrag('invoiceitems', 'order', 'sibling', 'invoiceitem-weight'); 

  $header = array();
  $row = array();

  $header = $form['header']['#value'];
  unset($form['header']);

  foreach (element_children($form['invoiceitems']) as $key) {
    $form['invoiceitems'][$key]['invoiceitem_weight_'. $key]['#attributes']['class'] = 'invoiceitem-weight';
    $data = array (
      drupal_render($form['invoiceitems'][$key]['invoiceitem_title_'. $key]),
      array('data' => sprintf('%.2f', drupal_render($form['invoiceitems'][$key]['invoiceitem_amount_'. $key])), 'align' => 'right'),
      array('data' => sprintf('%.2f', drupal_render($form['invoiceitems'][$key]['invoiceitem_tax_'. $key])), 'align' => 'right'),
      array('data' => sprintf('%.2f', drupal_render($form['invoiceitems'][$key]['invoiceitem_total_'. $key])), 'align' => 'right'),
      drupal_render($form['invoiceitems'][$key]['invoiceitem_weight_'. $key]),
      array (
        'data' => drupal_render($form['invoiceitems'][$key]['invoiceitem_operations_'. $key]),
        'class' => 'storm_list_operations',
      ),
    );
    $row['data'] = $data;
    $row['class'] = empty($row['class']) ? 'draggable' : $row['class'] .' draggable';
    $rows[] = $row;
  }
  $o = theme('table', $header, $rows, array('id' => 'invoiceitems'));
  return $o;
}*/

