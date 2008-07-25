<?php
// $Id$

/**
 * @file
 */

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
      storm_icon('priority_'. $project->projectpriority, $project->projectpriority),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormproject_view($node, $teaser = FALSE, $page = FALSE, $links = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormproject'] = array(
    '#prefix' => '<div id="stormproject">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['stormproject']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Organization', l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );
  $node->content['stormproject']['projectcategory'] = array(
    '#prefix' => '<div class="projectcategory">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Category', stormattribute_value('Project category', $node->projectcategory)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['projectstatus'] = array(
    '#prefix' => '<div class="projectstatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Status', stormattribute_value('Project status', $node->projectstatus)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['projectpriority'] = array(
    '#prefix' => '<div class="projectpriority">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Priority', stormattribute_value('Project priority', $node->projectpriority)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;

  if (module_exists('stormnote')) {
    $node->content['stormproject']['links']['notes']  = array(
      '#prefix' => '<div class="notes">',
      '#suffix' => '</div>',
      '#value' => l(t('Notes'), 'storm/notes/', array(), 'project_nid='. $node->nid),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormticket')) {
    $node->content['stormproject']['links']['tickets'] = array(
      '#prefix' => '<div class="tickets">',
      '#suffix' => '</div>',
      '#value' => l(t('Tickets'), 'storm/tickets/', array(), 'project_nid='. $node->nid),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormtimetracking')) {
    $node->content['stormproject']['links']['timetrackings'] = array(
      '#prefix' => '<div class="timetrackings">',
      '#suffix' => '</div>',
      '#value' => l(t('Timetrackings'), 'storm/timetrackings/', array(), 'project_nid='. $node->nid),
      '#weight' => $w++,
    );
  }
  return $node;
}
