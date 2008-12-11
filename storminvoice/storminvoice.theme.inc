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
      'data' => storm_t('Amount', 'invoice'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Tax', 'invoice'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Total', 'invoice'),
      'style' => 'text-align: center;',
    ),
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => storm_t('Total to pay', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => storm_t('Total paid', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => storm_t('Total', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->tax),
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
    '#value' => theme('storm_view_item', storm_t('Number', 'invoice'), $node->number),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['description'] = array(
    '#prefix' => '<div class="invoicedescription">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'invoice'), $node->title),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'invoice'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'invoice'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['reference'] = array(
    '#prefix' => '<div class="reference">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Reference', 'invoice'), $node->reference),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Amount', 'invoice'), sprintf('%.2f', $node->amount)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['tax'] = array(
    '#prefix' => '<div class="tax">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Tax', 'invoice'), sprintf('%.2f', $node->tax)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['total'] = array(
    '#prefix' => '<div class="total">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Total', 'invoice'), sprintf('%.2f', $node->total)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['requestdate'] = array(
    '#prefix' => '<div class="requestdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Request date', 'invoice'), format_date($node->requestdate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['duedate'] = array(
    '#prefix' => '<div class="duedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Due date', 'invoice'), format_date($node->duedate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['paymentdate'] = array(
    '#prefix' => '<div class="paymentdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Payment date', 'invoice'), $node->paymentdate? format_date($node->paymentdate, 'custom', 'Y-m-d') : ''),
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
    '#value' => theme('storm_view_item', storm_t('Status', 'invoice'), $status),
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
  $node->content['storminvoice']['links']['print']['html'] = array(
    '#prefix' => '<div class="print_html">',
    '#suffix' => '</div>',
    '#value' => l(t('Print HTML'), 'storm/invoice/report/'. $node->nid .'/html/'. $language),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['links']['print']['pdf'] = array(
    '#prefix' => '<div class="print_pdf">',
    '#suffix' => '</div>',
    '#value' => l(t('Print PDF'), 'storm/invoice/report/'. $node->nid .'/pdf/'. $language),
    '#weight' => $w++,
  );

  return $node;
}

function theme_storminvoice_report($node, $report, $language) {
  if($report=='html') {
    print theme('storminvoice_report_html', $node, $language); 
  }
  else if ($report=='pdf') {
    theme('storminvoice_report_pdf', $node, $language); 
  }
}

function theme_storminvoice_report_pdf($node, $language) {
  $tcpdf_dir = drupal_get_path('module', 'storm') .'/libs/tcpdf';
  require_once($tcpdf_dir .'/config/lang/eng.php');
  require_once($tcpdf_dir .'/tcpdf.php');

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
  $title = storm_t('Invoice', 'invoice', array(), $language);
  if ($language1) $title .= '<br />'. storm_t('Invoice', 'invoice', array(), $language1);
  $complete_title = $title .' '. $myorg->title .' : '. $organization->title .' - '. $node->number;

  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true); 

  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("Storm");
  $pdf->SetTitle($complete_title);
  $pdf->SetSubject($title);
  $pdf->SetKeywords($title, $myorg->title, $organization->title, $node->number);

  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);

  $margins = $pdf->getMargins();
  $pageWidth = $pdf->getPageWidth() - $margins['left'] - $margins['right'];
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

  $pdf->AddPage();

  $pdf->SetFont("times", "B", 14);
  $headerleft = variable_get('site_name', '') .'<br />'. variable_get('site_slogan', '');
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), $headerleft, 0 ,0 ,0, false, 'L');

  $pdf->SetFont("times", "N", 10);
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), variable_get('storm_report_header', ''), 0 ,1 ,0, false, 'R');

  $pdf->SetFont("times", "B", 14);
  $pdf->setDrawColor(204, 204, 204);
  $pdf->Cell(0, 12, $title, 'B' ,1,'C');

  $filename = strtolower($title . '_' . $myorg->title . '_' . $organization->title . '_' . str_replace('/', '-', $node->number));

  //Close and output PDF document
  $pdf->Output($filename, "I");

}

function theme_storminvoice_report_html($node, $language) {
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
  $title = storm_t('Invoice', 'invoice', array(), $language);
  if ($language1) $title .= '<br />'. storm_t('Invoice', 'invoice', array(), $language1);
  $headtitle = $title . ' '. $myorg->title . ' : ' . $organization->title . ' - ' . $node->number;

  $o .= '<div>';
  $o .= '<div id="storminvoice_billto"><table>';
  $o .= '<tr><td class="storminvoice_billto_title">';
  $o .= storm_t('Bill To :', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Bill To :', 'invoice', array(), $language1);
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
  if ($organization->taxid) {
    $o .= storm_t('Tax ID', 'invoice', array(), $language);
    if ($language1) $o .= ' / '. storm_t('Tax ID', 'invoice', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $o .= '</td></tr>';
  $o .= '</table></div>';

  $o .= '<div id="storminvoice_details"><table>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Invoice#', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Invoice#', 'invoice', array(), $language1);
  $o .='</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Currency', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Currency', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Date', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Date', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Reference', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Reference', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td>'. $node->number .'</td>';
  $o .= '<td>'. storm_t($mycurrency, 'invoice', array(), $language) .'</td>';
  $o .= '<td>'. format_date($node->requestdate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. $node->reference .'</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title" colspan="2">';
  $o .= storm_t('Due total', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Due total', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Due date', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Due date', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Terms', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Terms', 'invoice', array(), $language1);
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

  $description = storm_t('Description', 'invoice', array(), $language);
  if ($language1) $description .= '<br />'. storm_t('Description', 'invoice', array(), $language1);

  $amount = storm_t('Amount', 'invoice', array(), $language);
  if ($language1) $amount .= '<br />'. storm_t('Amount', 'invoice', array(), $language1);

  $taxpercent = storm_t('Tax %', 'invoice', array(), $language);
  if ($language1) $taxpercent .= '<br />'. storm_t('Tax %', 'invoice', array(), $language1);

  $tax = storm_t('Tax', 'invoice', array(), $language);
  if ($language1) $tax .= '<br />'. storm_t('Tax', 'invoice', array(), $language1);

  $total = storm_t('Total', 'invoice', array(), $language);
  if ($language1) $total .= '<br />'. storm_t('Total', 'invoice', array(), $language1);

  $header = array(
    array(
      'data' => $description,
    ),
    array(
      'data' => $amount,
    ),
    array(
      'data' => $taxpercent,
    ),
    array(
      'data' => $tax,
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
        'data' => $i->taxpercent,
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->tax),
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->total),
        'style' => 'text-align: right;',
      ),
    );
  }

  $total = storm_t('Total', 'invoice', array(), $language);
  if ($language1) $total .= ' / '. storm_t('Total', 'invoice', array(), $language1);

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
      'data' => sprintf('%.2f', $node->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $node->total),
      'style' => 'text-align: right;',
    ),
  );

  $o .= '<div id="storminvoice_items">'. theme('table', $header, $rows) .'</div>';

  if ($node->taxexempt && $language1=='it') {
    $o .= '<div id="storminvoice_tax_exempt">';
    $o .= storm_t('Tax exempt art. 7', 'invoice', array(), $language);
    if ($language1) $o .= '<br />'. storm_t('Tax exempt art. 7', 'invoice', array(), $language1);
    $o .= '</div>';
  }

  $o .= '<div id="storminvoice_payment_modes"><table>';
  $o .= '<tr><td class="storminvoice_payment_title">';
  $o .= storm_t('Payment', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Payment', 'invoice', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= t(variable_get('storminvoice_payment_modes', ''), array(), $language);
  $o .= '</td></tr>';
  $o .= '</table></div>';

  if ($status=='paid') {
    $o .= '<div id="paid">';
    $o .= '<h2>'. storm_t('Paid in full', 'invoice', array(), $language) .'</h2>';
    if ($language1) $o .= '<h2>'. storm_t('Paid in full', 'invoice', array(), $language1) .'</h2>';
    $o .= '</div>';
  }

  return theme('storm_report', $header, $o, $title, $footer, $headtitle);
}
