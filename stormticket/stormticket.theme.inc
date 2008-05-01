<?php
// $Id$

function theme_stormticket_list($header, $tickets) {

  $rows = array();
  foreach ($tickets as $ticket) {
    $n = new stdClass();
    $n->nid = $ticket->nid;
    $n->uid = $ticket->uid;
    $n->organization_nid = $ticket->organization_nid;
    $n->type = 'stormticket';

    $comments_links = l(comment_num_all($ticket->nid), 'node/'. $ticket->nid, array(), NULL, 'comments');
    $num_comment_new = comment_num_new($ticket->nid);
    if ($num_comment_new >0) {
      $comments_links .= ' - '. l($num_comment_new .' new', 'node/'. $ticket->nid, array(), NULL, 'new');
    }
    $rows[] = array(
      storm_icon('category_'. $ticket->ticketcategory, $ticket->ticketcategory),
      l($ticket->organization_title, 'node/'. $ticket->organization_nid),
      l($ticket->project_title, 'node/'. $ticket->project_nid),
      l($ticket->task_title, 'node/'. $ticket->task_nid),
      l($ticket->title, 'node/'. $ticket->nid) . theme('mark', node_mark($ticket->nid, $ticket->changed)),
      storm_icon('status_'. $ticket->ticketstatus, $ticket->ticketstatus),
      storm_icon('priority_'. $ticket->ticketpriority, $ticket->ticketstatus),
      $comments_links,
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

