<?php

/**
 * @file
 */

function storminvoice_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['storminvoice_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['storminvoice_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['storminvoice_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['storminvoice_list_filter']['organization_nid'] = $p->organization_nid;
  }

  $i = new stdClass();
  $i->type = 'storminvoice';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Number'),
      'field' => 'sin.number',
      'sort' => 'asc',
      'nowrap' => 'nowrap',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'sin.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sin.project_title',
    ),
    array(
      'data' => t('Description'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Req. date'),
      'field' => 'sin.requestdate',
    ),
    array(
      'data' => t('Total'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.title, n.type, n.uid, sin.*, nre.format FROM {node} AS n
    INNER JOIN {storminvoice} AS sin ON n.vid=sin.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='storminvoice' ";

  $s_totals_topay = "SELECT SUM(amount) amount, SUM(tax1) tax1, SUM(tax2) tax2, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate=0";

  $s_totals_paid = "SELECT SUM(amount) amount, SUM(tax1) tax1, SUM(tax2) tax2, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate<>0";

  $s_totals = "SELECT SUM(amount) amount, SUM(tax1) tax1, SUM(tax2) tax2, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice'";

  $where = array();
  $args = array();
  $filterfields = array();

  if (isset($_SESSION['storminvoice_list_filter']['organization_nid']) && $_SESSION['storminvoice_list_filter']['organization_nid'] != 0) {
    $where[] = 'sin.organization_nid=%d';
    $args[] = $_SESSION['storminvoice_list_filter']['organization_nid'];
    $filterfields[] = 'Organization';
  }
  if (isset($_SESSION['storminvoice_list_filter']['project_nid']) && $_SESSION['storminvoice_list_filter']['project_nid'] != 0) {
    $where[] = 'sin.project_nid=%d';
    $args[] = $_SESSION['storminvoice_list_filter']['project_nid'];
    $filterfields[] = 'Project';
  }
  if (isset($_SESSION['storminvoice_list_filter']['reqdatefrom'])) {
    $reqdatefrom = $_SESSION['storminvoice_list_filter']['reqdatefrom'];
    $where[] = 'sin.requestdate>=%d';
    $args[] = _storm_date_to_gmtimestamp($reqdatefrom);
    $filterfields[] = 'Request date';
  }
  if (isset($_SESSION['storminvoice_list_filter']['reqdateto']) && $_SESSION['storminvoice_list_filter']['reqdateto']['day'] != -1) {
    $reqdateto = $_SESSION['storminvoice_list_filter']['reqdateto'];
    $where[] = 'sin.requestdate<=%d';
    $args[] = _storm_date_to_gmtimestamp($reqdateto);
    $filterfields[] = 'Request date';
  }

  $status = isset($_SESSION['storminvoice_list_filter']['status']) ? $_SESSION['storminvoice_list_filter']['status'] : '-';
  if ($status != '-') {
    if ($status == 'to pay') {
      $where[] = "sin.paymentdate=0";
    }
    if ($status == 'paid') {
      $where[] = "sin.paymentdate<>0";
    }
    if ($status == 'overdue') {
      $where[] = "sin.paymentdate=0 AND sin.duedate<". time();
    }
    $filterfields[] = 'Status';
  }

  $itemsperpage = isset($_SESSION['storminvoice_list_filter']['itemsperpage']) ? $_SESSION['storminvoice_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['storminvoice_list_filter']['itemsperpage'] = $itemsperpage;

  $tablesort = tablesort_sql($header);

  if ($tablesort == " ORDER BY  sin.number ASC") {
    $tablesort = " ORDER BY LPAD(sin.number, 10, '0') ASC";
  }
  elseif ($tablesort == " ORDER BY  sin.number DESC") {
    $tablesort = " ORDER BY LPAD(sin.number, 10, '0') DESC";
  }

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('storminvoice_list_filter', $filterdesc);

  $s = storminvoice_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);
  $invoices = array();
  while ($invoice = db_fetch_object($r)) {
    $invoices[] = $invoice;
  }

  $s_totals_topay = storminvoice_access_sql($s_totals_topay, $where);
  $s_totals_topay = db_rewrite_sql($s_totals_topay);
  $r = db_query($s_totals_topay, $args);
  if ($r) $totals_topay = db_fetch_object($r);

  $s_totals_paid = storminvoice_access_sql($s_totals_paid, $where);
  $s_totals_paid = db_rewrite_sql($s_totals_paid);
  $r = db_query($s_totals_paid, $args);
  if ($r) $totals_paid = db_fetch_object($r);

  $s_totals = storminvoice_access_sql($s_totals, $where);
  $s_totals = db_rewrite_sql($s_totals);
  $r = db_query($s_totals, $args);
  if ($r) $totals = db_fetch_object($r);

  $o .= theme('storminvoice_list', $header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals);
  print theme('page', $o);
}

function storminvoice_list_filter(&$form_state, $filterdesc = 'Filter') {
  $organization_nid = isset($_SESSION['storminvoice_list_filter']['organization_nid']) ? $_SESSION['storminvoice_list_filter']['organization_nid'] : 0;
  $project_nid = isset($_SESSION['storminvoice_list_filter']['project_nid']) ? $_SESSION['storminvoice_list_filter']['project_nid'] : 0;
  $status = isset($_SESSION['storminvoice_list_filter']['status']) ? $_SESSION['storminvoice_list_filter']['status'] : '';
  $itemsperpage = isset($_SESSION['storminvoice_list_filter']['itemsperpage']) ? $_SESSION['storminvoice_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['storminvoice_list_filter']['itemsperpage'] = $itemsperpage;

  $reqdatefrom = isset($_SESSION['storminvoice_list_filter']['reqdatefrom']) ? $_SESSION['storminvoice_list_filter']['reqdatefrom'] : NULL;
  $reqdateto = isset($_SESSION['storminvoice_list_filter']['reqdateto']) ? $_SESSION['storminvoice_list_filter']['reqdateto'] : NULL;

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $filterdesc,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => t('All')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormproject_organization_projects(this, 'edit-project-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid);
  $projects = array();
  while ($project = db_fetch_array($r)) {
    $projects[$project['nid']] = $project['title'];
  }
  $projects = array(0 => t('All')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -19,
  );

  $form['filter']['group2']['reqdatefrom'] = array(
    '#type' => 'dateext',
    '#title' => t('Req. date from'),
    '#withnull' => TRUE,
    '#default_value' => $reqdatefrom,
  );

  $form['filter']['group2']['reqdateto'] = array(
    '#type' => 'dateext',
    '#title' => t('Req. date to'),
    '#withnull' => TRUE,
    '#default_value' => $reqdateto,
  );

  $form['filter']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('-' => t('all'), 'to pay' => t('to pay'), 'paid' => t('paid'), 'overdue' => t('overdue')),
    '#default_value' => $status,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('storminvoice_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('storminvoice_list_filter_reset'),
  );

  $form['filter']['group3']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function storminvoice_list_filter_filter($form, &$form_state) {
  $_SESSION['storminvoice_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['storminvoice_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['storminvoice_list_filter']['reqdatefrom'] = $form_state['values']['reqdatefrom'];
  $_SESSION['storminvoice_list_filter']['reqdateto'] = $form_state['values']['reqdateto'];
  $_SESSION['storminvoice_list_filter']['status'] = $form_state['values']['status'];
  $_SESSION['storminvoice_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function storminvoice_list_filter_reset($form, &$form_state) {
  unset($_SESSION['storminvoice_list_filter']);
}

function _storminvoice_project_invoices_js($organization_nid, $project_nid) {
  $invoices = array();

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {storminvoice} AS sin
  ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice' AND sin.organization_nid=%d AND sin.project_nid=%d
  ORDER BY n.title";

  $s = storminvoice_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $invoices[$nid] = $item->title;
  }
  print drupal_to_js($invoices);
  exit();
}

function storminvoice_report($node, $report, $language) {
  print theme('storminvoice_report', $node, $report, $language);
}

/**
 * Page
 */
function storminvoice_send_page($node, $language) {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Invoices'), 'storm/invoices');
  $breadcrumb[] = l($node->title, 'node/'. $node->nid);

  drupal_set_breadcrumb($breadcrumb);
  drupal_set_html_head('<meta name="robots" content="noindex, nofollow" />');

  $path =  'node/'. $node->nid;

  return drupal_get_form('storminvoice_send_form', $path, $node, $language);
}

/**
 * Form
 */
function storminvoice_send_form(&$form_state, $path = NULL, $node = NULL, $language) {
  global $base_url, $user;

  $pdffile=variable_get('storminvoice_email_pdf_file_path', '');

  $organization = node_load($node->organization_nid);
  $organization_email = $organization->email;

  $form = array();
  $cid = array();

  $emailtype = 'email';

  $form['message']['instructions'] = array(
    '#type' => 'item',
    '#value' => variable_get('storminvoice_email_pdf_instructions', t('<p>From here you can send PDF file of invoice as attachment to your client</p><p><b>NOTE:</b> You can change subject and cover note of email. If <i>Bcc</i> field is checked you will also get copy of email.</p>')),
  );
  $form['message']['yemail'] = array(
    '#type' => 'textfield',
    '#title' => t('Your e-mail'),
    '#size' => 58,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['message']['yname'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#size' => 58,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['message']['recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Send to'),
    '#default_value' => $organization_email,
    '#cols' => 50,
    '#rows' => 1,
    '#description' => t('Enter multiple addresses and separate them with commas.'),
    '#required' => TRUE,
  );

  $form['message']['bcc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send me a copy'),
    '#default_value' => 1,
    '#description' => t('Leave this box checked if you want to receive a copy as Bcc.'),
  );

  $form['message']['page'] = array(
      '#type' => 'item',
      '#title' => t('You are going to email the following invoice'),
      '#value' => l($node->title, 'storm/invoice/report/'. $node->nid .'/pdf/'. $language),
    );

  $form['message']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject'),
    '#value' => variable_get('storminvoice_cover_note_subject', ''),
    '#description' => t('Enter subject for email.'),
  );
  $form['message']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body'),
    '#value' => variable_get('storminvoice_cover_note', ''),
    '#description' => 'Cover note',
    '#cols' => 50,
    '#rows' => 10,
    '#required' => TRUE,
  );

  $form['message']['path'] = array(
    '#type' => 'hidden',
    '#value' => $path,
  );

  $form['message']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send invoice'),
  );

  if ($user->uid != 0) {
    $form['message']['yemail']['#default_value'] = $user->mail;
    $form['message']['yemail']['#disabled'] = TRUE;
    $form['message']['yemail']['#value'] = $user->mail;
    $form['message']['yname']['#default_value'] = $user->name;
  }

  return $form;
}

function storminvoice_send_form_submit($form, &$form_state) {
  $form_filled = $form_state['clicked_button']['#post'];

  $to = $form_filled['recipients'];
  $language = NULL;
  $from = $form_state['values']['yname'] .'<'. $form_state['values']['yemail'] .'>';

  $params = array();

  $params['subject'] = $form_filled['subject'];
  $params['bcc'] = $form_state['values']['bcc'];

  $trenner  = md5(uniqid(time()));
  variable_set('storminvoice_email_pdf_trenner', $trenner);

  $headers['Content-Type'] = "multipart/mixed; boundary=$trenner";
  $params['body']  = "\n--$trenner\n";
  $params['body'] .= "Content-Type: text/plain; charset=UTF-8; format=flowed;"."\n\n"; // sets the mime type
  $params['body'] .= $form_filled['body'] ."\n";
  $params['body'] .= "\n\n";
  $params['body'] .= "\n\n";

  $node =  $form['#parameters'][3];
  $language = $form['#parameters'][4];
  $filedata = theme('storminvoice_report_pdf', $node, $language, 'email');

  $pdffile = variable_get('storminvoice_email_pdf_file_path', '');
  $pdffile_name = variable_get('storminvoice_email_pdf_file_name', '');
  $pdffile_mime = file_get_mimetype($pdffile);
  $params['body']  .= "--$trenner"."\n";
  $params['body']  .= "Content-Type:$pdffile_mime; name='$pdffile_name'\n";
  $params['body']  .= "Content-Disposition: attachment; filename=$pdffile_name\n";
  $params['body']  .= "Content-Transfer-Encoding: base64\n\n";
  $params['body']  .= chunk_split(base64_encode($filedata));
  $params['body'] .= "--$trenner--";

  drupal_mail('storminvoice', 'invoice', $to, $language, $params, $from , True);

  drupal_set_message(variable_get('storminvoice_sent', t('You have sent invoice to @email.', array('@email' => $to ))));
}

/**
 * Implementation of hook_mail().
 */
function storminvoice_mail($key, &$message, $params) {

  $trenner = variable_get('storminvoice_email_pdf_trenner', '');
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];
  $message['headers']['MIME-Version'] = '1.0';
  $message['headers']['Content-Type'] = "multipart/mixed; boundary=$trenner";
  if ($params['bcc']) {
    $message['headers']['Bcc'] = $message['headers']['From'];
  }
}
