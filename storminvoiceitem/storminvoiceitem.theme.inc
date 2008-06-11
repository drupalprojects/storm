<?php
// $Id$

/**
 * @file
 */

function theme_storminvoiceitem_invoiceitems($header, $invoiceitems) {
  $rows = array();
  foreach ($invoiceitems as $invoiceitem) {
    $n = new stdClass();
    $n->nid = $invoiceitem->nid;
    $n->uid = $invoiceitem->uid;
    $n->type = 'storminvoiceitem';

    $rows[] = array(
      $invoiceitem->title,
      array('data' => sprintf('%.2f', $invoiceitem->amount), 'align' => 'right'),
      array('data' => sprintf('%.2f', $invoiceitem->vat), 'align' => 'right'),
      array('data' => sprintf('%.2f', $invoiceitem->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

