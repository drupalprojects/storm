<?php
// $Id$

function theme_storm_form_group($form) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');
  $o = '<table class="formgroup"><tr>';
  foreach (element_children($form) as $key) {
    $o .= '<td>'. drupal_render($form[$key]) .'</td>';
  }
  $o .= '</tr></table>';
  return $o;
}

function theme_datetime($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

