<?php

/**
 * @file
 */

function theme_stormnote_list($header, $notes) {
  $rows = array();

  foreach ($notes as $note) {
    $n = new stdClass();
    $n->nid = $note->nid;
    $n->uid = $note->uid;
    $n->organization_nid = $note->organization_nid;
    $n->project_nid = $note->project_nid;
    $n->task_nid = $note->task_nid;
    $n->type = 'stormnote';
    $rows[] = array(
      l($note->organization_title, 'node/'. $note->organization_nid),
      l($note->project_title, 'node/'. $note->project_nid),
      l($note->task_title, 'node/'. $note->task_nid),
      l($note->title, 'node/'. $note->nid),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormnote_view($node, $teaser = FALSE, $page = FALSE) {
  $o  = '<div class="stormnote">';
  $o .= '<dl>';
  $o .= '<dt>'. t('Title') .' : '.  check_plain($node->title) .'</dt>';
  $o .= '<dt>'.  t('Organization') .' : '.  l($node->organization_title, 'node/'. $node->organization_nid) .'</dt>';
  $o .= '<dt>'.  t('Project') .' : '.  l($node->project_title, 'node/'. $node->project_nid) .'</dt>';
  $o .= '<dt>'.  t('Task') .' : '.  l($node->task_title, 'node/'. $node->task_nid) .'</dt>';
  $o .= '</dl>';
  $o .= '</div>';
  return $o;
}

