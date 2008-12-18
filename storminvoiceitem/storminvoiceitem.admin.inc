<?php
// $Id$

/**
 * @file
 */

function storminvoiceitem_invoiceitems($invoice) {
  $node_invoice = node_load($invoice->nid);
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Invoices'), 'storm/invoices');
  $breadcrumb[] = l(t('Invoice : '. $node_invoice->title), 'node/'. $node_invoice->nid);
  drupal_set_breadcrumb($breadcrumb);

  $i = new stdClass();
  $i->type = 'storminvoiceitem';

  $params = $_GET;
  $params['organization_nid'] = $invoice->organization_nid;
  $params['project_nid'] = $invoice->project_nid;
  $params['invoice_nid'] = $invoice->nid;

  $header = array(
    array(
      'data' => storm_t('Title', 'invoiceitem'),
    ),
    array(
      'data' => storm_t('Amount', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('VAT', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Total', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, sit.* FROM {node} AS n INNER JOIN {storminvoiceitem} sit ON n.vid=sit.vid WHERE n.status=1 AND n.type='storminvoiceitem' ORDER BY sit.weight";
  $where[] = 'sit.invoice_nid='. $invoice->nid;

  $s = storminvoiceitem_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = db_query($s . $tablesort);

  $items = array();
  while ($item = db_fetch_object($r)) {
    $items[] = $item;
  }

  $o .= theme('storminvoiceitem_invoiceitems', $header, $items);
  print theme('page', $o);
}

