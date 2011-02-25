<?php

/**
 * @file
 */

function storminvoiceitem_invoiceitems($invoice_nid) {
  $invoice = node_load($invoice_nid);

  $i = new stdClass();
  $i->type = 'storminvoiceitem';

  $params = $_GET;
  $params['organization_nid'] = $invoice->organization_nid;
  $params['project_nid'] = $invoice->project_nid;
  $params['invoice_nid'] = $invoice->nid;

  $header = array(
    array(
      'data' => t('Title'),
    ),
    array(
      'data' => t('Amount'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => t('VAT'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => t('Total'),
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

