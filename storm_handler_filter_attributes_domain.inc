<?php
/**
 * @file
 * Views handler to filter by attribute domain
 */
class storm_handler_filter_attributes_domain extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = storm_attributes_bydomain($this->definition['domain']);
      $this->value_options = $this->value_options['values'];
    }
  }
}
