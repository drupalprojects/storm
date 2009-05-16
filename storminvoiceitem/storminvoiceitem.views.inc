<?php
// $Id:

function storminvoiceitem_views_data() {
  $data['storminvoiceitem']['table']['group']  = 'Storm';
  $data['storminvoiceitem']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['storminvoiceitem']['invoice_nid'] = array(
    'title' => t('Invoiceitem Invoice Node ID'),
    'help' => 'Storm Invoiceitem Invoice Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['storminvoiceitem']['weight'] = array(
    'title' => t('Invoiceitem Weight'),
    'help' => 'Storm Invoiceitem Weight',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['storminvoiceitem']['amount'] = array(
    'title' => t('Invoiceitem Amount'),
    'help' => t('Storm Invoiceitem Amount'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['storminvoiceitem']['taxpercent'] = array(
    'title' => t('Invoiceitem Tax Percent'),
    'help' => t('Storm Invoiceitem Tax Percent'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['storminvoiceitem']['tax'] = array(
    'title' => t('Invoiceitem Tax'),
    'help' => t('Storm Invoiceitem Tax'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['storminvoiceitem']['total'] = array(
    'title' => t('Invoiceitem Total'),
    'help' => t('Storm Invoiceitem Total'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}
