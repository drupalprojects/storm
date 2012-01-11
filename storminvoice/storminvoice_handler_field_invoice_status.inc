<?php

class storminvoice_handler_field_invoice_status extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['paymentdate'] = array('field' => 'paymentdate');
    $this->additional_fields['duedate'] = array('field' => 'duedate');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $paymentdate = $values->{$this->aliases['paymentdate']};
    $duedate = $values->{$this->aliases['duedate']};
    $status = 'open';
    if ($paymentdate) {
      $status = 'paid';
    }
    else if ($duedate < time()) {
      $status = 'overdue';
    }
    return storm_icon('invoice_status_'. check_plain($status), stormattribute_value('Invoice status', $status));
  }

}