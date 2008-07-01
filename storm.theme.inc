<?php
// $Id$

/**
 * @file
 */

function theme_storm_form_group($form) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $row = array();
  foreach (element_children($form) as $key) {
    array_push($row, drupal_render($form[$key]));
  }
  $rows[] = array('data' => $row, 'class' => 'formgroup');
  $output .= theme('table', array(), $rows, array('class' => 'formgroup'));
  return $output;
}

function theme_datetime($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_dateext($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_storm_list_report($header, $rows, $title, $footer) {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_list_report_all.css';
  $css_print_file = drupal_get_path('module', 'storm') .'/storm_list_report_print.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '<link type="text/css" rel="stylesheet" media="print" href="'. base_path() . $css_print_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $o .= '<td class="storm_report_logo"><img src="'. theme_get_setting('logo') .'" /></td>';
  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_list_report_title">'. $title .'</div>';
  $o .= '<div class="storm_list_report_data">';
  $o .= theme('table', $header, $rows);
  $o .= '</div>';
  $o .= '<div class="storm_list_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

function theme_storm_report($header, $content, $title, $footer) {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_report_all.css';
  $css_print_file = drupal_get_path('module', 'storm') .'/storm_report_print.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '<link type="text/css" rel="stylesheet" media="print" href="'. base_path() . $css_print_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $o .= '<td class="storm_report_logo"><img src="'. theme_get_setting('logo') .'" /></td>';
  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_report_title">'. $title .'</div>';
  $o .= '<div id="storm_report_content">';
  $o .= $content;
  $o .= '</div>';
  $o .= '<div class="storm_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

