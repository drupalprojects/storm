<?php
// $Id$

/**
 * @file
 */

function theme_storminvoice_list($header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals) {
  $rows = array();
  foreach ($invoices as $invoice) {
    $n = new stdClass();
    $n->nid = $invoice->nid;
    $n->uid = $invoice->uid;
    $n->organization_nid = $invoice->organization_nid;
    $n->project_nid = $invoice->project_nid;
    $n->type = 'storminvoice';

    $invoice->status = 'open';
    if ($invoice->paymentdate) {
      $invoice->status = 'paid';
    }
    else if ($invoice->duedate < time()) {
      $invoice->status = 'overdue';
    }

    $rows[] = array(
      array(
        'data' => storm_icon('invoice_status_'. $invoice->status, $invoice->status),
        'style' => 'text-align: center',
      ),
      array(
        'data' => l($invoice->number, 'node/'. $invoice->nid),
        'style' => 'text-align: right',
      ),
      l($invoice->organization_title, 'node/'. $invoice->organization_nid),
      l($invoice->project_title, 'node/'. $invoice->project_nid),
      l($invoice->title, 'node/'. $invoice->nid),
      format_date($invoice->requestdate, 'custom', 'Y-m-d'),
      array('data' => sprintf('%.2f', $invoice->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  $o .= theme('pager', NULL, $itemsperpage, 0);

  $header = array(
    array(
      'data' => ' ',
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
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => t('Total to pay'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->vat),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => t('Total paid'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->vat),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => t('Total'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->vat),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->total),
      'style' => 'text-align: right;',
    ),
  );
  $o .= theme('table', $header, $rows);

  return $o;
}

function theme_storminvoice_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['storminvoice'] = array(
    '#prefix' => '<div id="storminvoice">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['storminvoice']['number'] = array(
    '#prefix' => '<div class="number">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Number'), $node->number),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['description'] = array(
    '#prefix' => '<div class="invoicedescription">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->title),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Organization'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['reference'] = array(
    '#prefix' => '<div class="reference">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Reference'), $node->reference),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Amount'), sprintf('%.2f', $node->amount)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['vat'] = array(
    '#prefix' => '<div class="vat">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('VAT'), sprintf('%.2f', $node->vat)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['total'] = array(
    '#prefix' => '<div class="total">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Total'), sprintf('%.2f', $node->total)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['requestdate'] = array(
    '#prefix' => '<div class="requestdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Request date'), format_date($node->requestdate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['duedate'] = array(
    '#prefix' => '<div class="duedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Due date'), format_date($node->duedate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['paymentdate'] = array(
    '#prefix' => '<div class="paymentdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Payment date'), $node->paymentdate? format_date($node->paymentdate, 'custom', 'Y-m-d') : ''),
    '#weight' => $w++,
  );

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }
  $node->content['storminvoice']['status'] = array(
    '#prefix' => '<div class="invoicestatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Status'), $status),
    '#weight' => $w++,
  );

  $organization = node_load($node->organization_nid);
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  if ($myorg->orglanguage != $organization->orglanguage) {
    $language = $organization->orglanguage .','. $myorg->orglanguage;
  }
  else {
    $language = $myorg->orglanguage;
  }

  $node->content['storminvoice']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['storminvoice']['links']['print'] = array(
    '#prefix' => '<div class="print">',
    '#suffix' => '</div>',
    '#value' => l(t('Print'), 'storm/invoice/report/'. $node->nid .'/std/'. $language),
    '#weight' => $w++,
  );

  return $node;
}

function theme_storminvoice_report($node, $report, $language) {
  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = stormattribute_attributes_bydomain('Country');
  $currencies = stormattribute_attributes_bydomain('Currency');
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = t('Invoice', array(), $language);
  if ($language1) $title .= '<br />'. t('Invoice', array(), $language1);

  $o .= '<div>';
  $o .= '<div id="storminvoice_billto"><table>';
  $o .= '<tr><td class="storminvoice_billto_title">';
  $o .= t('Bill To :', array(), $language);
  if ($language1) $o .= '<br />'. t('Bill To :', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= '<strong>'. $organization->fullname .'</strong>';
  $o .= '<br />';
  $o .= $organization->address;
  $o .= '<br />';
  $o .= $organization->city;
  $o .= '<br />';
  $o .= $organization->provstate .' '. $organization->zip .' '. $countries[$organization->country];
  $o .= '<br />';
  if ($organization->vatid) {
    $o .= t('VAT ID', array(), $language);
    if ($language1) $o .= ' / '. t('VAT ID', array(), $language1);
    $o .= ' : '. $organization->vatid;
  }
  $o .= '</td></tr>';
  $o .= '</table></div>';

  $o .= '<div id="storminvoice_details"><table>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Invoice#', array(), $language);
  if ($language1) $o .= '<br />'. t('Invoice#', array(), $language1);
  $o .='</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Currency', array(), $language);
  if ($language1) $o .= '<br />'. t('Currency', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Date', array(), $language);
  if ($language1) $o .= '<br />'. t('Date', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Reference', array(), $language);
  if ($language1) $o .= '<br />'. t('Riferimento', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td>'. $node->number .'</td>';
  $o .= '<td>'. t($mycurrency, array(), $language) .'</td>';
  $o .= '<td>'. format_date($node->requestdate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. $node->reference .'</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title" colspan="2">';
  $o .= t('Due total', array(), $language);
  if ($language1) $o .= '<br />'. t('Due total', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Due date', array(), $language);
  if ($language1) $o .= '<br />'. t('Due date', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Terms', array(), $language);
  if ($language1) $o .= '<br />'. t('Terms', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td style="text-align: center; font-weight: bold;" colspan="2">';
  $o .= sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= '<br />';
    $o .= '('. $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr) .')';
  }
  $o .= '</td>';
  $o .= '<td>'. format_date($node->duedate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. t(variable_get('storminvoice_payment_terms', ''), array(), $language) .'</td>';
  $o .= '</tr>';
  $o .= '</table></div>';
  $o .= '</div>';

  $description = t('Description', array(), $language);
  if ($language1) $description .= '<br />'. t('Description', array(), $language1);

  $amount = t('Amount', array(), $language);
  if ($language1) $amount .= '<br />'. t('Amount', array(), $language1);

  $vatpercent = t('VAT %', array(), $language);
  if ($language1) $vatpercent .= '<br />'. t('VAT %', array(), $language1);

  $vat = t('VAT', array(), $language);
  if ($language1) $vat .= '<br />'. t('VAT', array(), $language1);

  $total = t('Total', array(), $language);
  if ($language1) $total .= '<br />'. t('Total', array(), $language1);

  $header = array(
    array(
      'data' => $description,
    ),
    array(
      'data' => $amount,
    ),
    array(
      'data' => $vatpercent,
    ),
    array(
      'data' => $vat,
    ),
    array(
      'data' => $total,
    ),
  );
  $items = storminvoiceitem_getitems($node->nid);
  $rows = array();
  foreach ($items as $i) {
    $rows[] = array(
      array(
        'data' => $i->title,
      ),
      array(
        'data' => sprintf('%.2f', $i->amount),
        'style' => 'text-align: right;',
      ),
      array(
        'data' => $i->vatpercent,
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->vat),
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->total),
        'style' => 'text-align: right;',
      ),
    );
  }

  $total = t('Total', array(), $language);
  if ($language1) $total .= ' / '. t('Total', array(), $language1);

  $rows[] = array(
    array(
      'data' => $total,
      'style' => 'font-weight: bold; text-align: center; background-color: #e9e9e9;',
    ),
    array(
      'data' => sprintf('%.2f', $node->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => '-',
      'style' => 'text-align: center;',
    ),
    array(
      'data' => sprintf('%.2f', $node->vat),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $node->total),
      'style' => 'text-align: right;',
    ),
  );

  $o .= '<div id="storminvoice_items">'. theme('table', $header, $rows) .'</div>';

  if ($node->vatexempt) {
    $o .= '<div id="storminvoice_vat_exempt">';
    $o .= t('Vat exempt art. 7', array(), $language);
    if ($language1) $o .= '<br />'. t('Vat exempt art. 7', array(), $language1);
    $o .= '</div>';
  }

  $o .= '<div id="storminvoice_payment_modes"><table>';
  $o .= '<tr><td class="storminvoice_payment_title">';
  $o .= t('Payment', array(), $language);
  if ($language1) $o .= '<br />'. t('Payment', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= t(variable_get('storminvoice_payment_modes', ''), array(), $language);
  $o .= '</td></tr>';
  $o .= '</table></div>';

  if ($status=='paid') {
    $o .= '<div id="paid">';
    $o .= '<h2>'. t('Paid in full', array(), $language) .'</h2>';
    if ($language1) $o .= '<h2>'. t('Paid in full', array(), $language1) .'</h2>';
    $o .= '</div>';
  }

  print theme('storm_report', $header, $o, $title, $footer);
}
