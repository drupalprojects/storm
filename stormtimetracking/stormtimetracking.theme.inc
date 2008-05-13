<?php
//Id

function theme_stormtimetracking_list($header, $timetrackings, $duration) {
  $rows = array();
  foreach($timetrackings as $timetracking) {
    $n = new stdClass();
    $n->nid = $timetracking->nid;
    $n->uid = $timetracking->uid;
    $n->organization_nid = $timetracking->organization_nid;
    $n->project_nid = $timetracking->project_nid;
    $n->task_nid = $timetracking->task_nid;
    $n->ticket_nid = $timetracking->ticket_nid;
    $n->type = 'stormtimetracking';
    $rows[] = array(
      l($timetracking->organization_title, 'node/' . $timetracking->organization_nid),
      l($timetracking->project_title, 'node/' . $timetracking->project_nid),
      l($timetracking->title, 'node/' . $timetracking->nid), 
      format_date($timetracking->trackingdate, 'small'),
      array('data' => sprintf('%.2f', $timetracking->duration), 'align'=>'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) . '&nbsp;' . storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  $o .= '<span style="font-weight: bold;">' . t('Total duration') . ' : ' . sprintf('%.2f', $duration) . '</span> (hours)'; 
  return $o;
}

function theme_stormtimetracking_view($node, $teaser = FALSE, $page = FALSE) {
  $o  = '<div class="stormtimetracking">';
  $o .= '<dl>';
  $o .= '<dt>' . t('Title') . ' : ' .  check_plain($node->title) . '</dt>';
  $o .= '<dt>' .  t('Organization') . ' : ' .  l($node->organization_title, 'node/' . $node->organization_nid) . '</dt>';
  $o .= '<dt>' .  t('Project') . ' : ' .  l($node->project_title, 'node/' . $node->project_nid) . '</dt>';
  $o .= '<dt>' .  t('Task') . ' : ' .  l($node->task_title, 'node/' . $node->task_nid) . '</dt>';
  $o .= '<dt>' .  t('Ticket') . ' : ' .  l($node->ticket_title, 'node/' . $node->ticket_nid) . '</dt>';
  $o .= '</dl>';
  $o .= '</div>';
  return $o;
}

function theme_stormtimetracking_list_report_html($header, $timetrackings) {
  $output = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $output .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $output .= '<head>';
  $output .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $output .= drupal_get_html_head();
  $output .= drupal_get_css();

  $output .= '</head>';
  $output .= '<body style="background-color: #fff; color: #000;">';
  $output .= '<table>';
  $output .= '<tr>';
  foreach ($header as $c) {
    $output .= '<td>'. $c .'</td>';
  }
  $output .= '</tr>';
  foreach ($timetrackings as $i) {
    $output .= '<tr>';
    $output .= '<td>'. $i->organization_title .'</td>';
    $output .= '<td>'. $i->project_title .'</td>';
    $output .= '<td>'. $i->duration .'</td>';
    $output .= '</tr>';
  }
  $output .= '</table>';
  $output .= '</body></html>';

  return $output;

}

function theme_stormtimetracking_list_report_html_wo_org($header, $timetrackings) {
  $output = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $output .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $output .= '<head>';
  $output .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $output .= drupal_get_html_head();
  $output .= drupal_get_css();

  $output .= '</head>';
  $output .= '<body style="background-color: #fff; color: #000;">';
  $output .= '<table>';
  $output .= '<tr>';
  foreach ($header as $c) {
    $output .= '<td>'. $c .'</td>';
  }
  $output .= '</tr>';
  foreach ($timetrackings as $i) {
    $output .= '<tr>';
    $output .= '<td>'. $i->project_title .'</td>';
    $output .= '<td>'. $i->title .'</td>';
    $output .= '<td>'. $i->duration .'</td>';
    $output .= '</tr>';
  }
  $output .= '</table>';
  $output .= '</body></html>';

  return $output;

}
