<?php
// $Id$

/**
 * @file
 * Contains functions relating to the Storm Invoice auto_add functionality
 * (creating invoices based on other Storm nodes)
 */
 
function storminvoice_auto_add($node, $invoice = NULL) {
  switch ($node->type) {
    case 'stormticket':
      $invoice_nid = stormticket_storminvoice_auto_add($node, $invoice);
      break;
    case 'stormtimetracking':
      $invoice_nid = stormtimetracking_storminvoice_auto_add($node, $invoice);
      break;
    default:
      drupal_set_message(t('This type of node cannot be automatically added to an invoice'), 'error');
      drupal_goto('node/'. $node->nid);
  }
  
  drupal_set_message('This invoice was created automatically from the selected '. $node->type .'.');
  drupal_goto('node/'. $invoice_nid);
}

function storminvoice_auto_add_select($node) {
  $form['invoice'] = array(
    '#title' => t('Invoice'),
    '#type' => 'select',
    '#options' => array(485, 489),
    '#description' => t('Please choose the invoice that you would like to add to'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Select'),
  );
  
  return $form;
}

function storminvoice_auto_add_select_submit($form_id, $form_state) {
  storminvoice_auto_add($node, $form_state['values']['invoice']);
}