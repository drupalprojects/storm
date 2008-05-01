<?php
// $Id$

function theme_stormproject_list($header, $projects) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  foreach ($projects as $key => $project) {
    $n = new stdClass();
    $n->nid = $project->nid;
    $n->uid = $project->uid;
    $n->organization_nid = $project->organization_nid;
    $n->type = 'stormproject';

    $rows[] = array(
      storm_icon('projectcategory_'. $project->projectcategory, $project->projectcategory),
      l($project->organization_title, 'node/'. $project->organization_nid),
      l($project->title, 'node/'. $project->nid) . theme('mark', node_mark($project->nid, $project->changed)),
      storm_icon('status_'. $project->projectstatus, $project->projectstatus),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}


function theme_stormproject_list_find($form) {
  $o = '<table class="formgroup"><tbody style="border: none;"><tr>';
  $o .= '<td valign="top">';
  $o .= drupal_render($form['organization_nid']);
  $o .= '</td>';
  $o .= '<td valign="top">';
  $o .= drupal_render($form['projectcategory']);
  $o .= '</td>';
  $o .= '<td valign="top">';
  $o .= drupal_render($form['projectstatus']);
  $o .= '</td>';
  $o .= '</tr></tbody></table>';
  $o .= drupal_render($form);
  return $o;
}
