<?php

/**
 * @file
 */

function theme_stormticket_list($variables) {
  $header = $variables['header'];
  $tickets = $variables['tickets'];

  $rows = array();
  foreach ($tickets as $ticket) {
    $n = new stdClass();
    $n->nid = $ticket->nid;
    $n->uid = $ticket->uid;
    $n->organization_nid = $ticket->organization_nid;
    $n->type = 'stormticket';

    $rows[] = array(
      storm_icon('category_'. $ticket->ticketcategory, storm_attribute_value('Ticket category', $ticket->ticketcategory)),
      l($ticket->organization_title, 'node/'. $ticket->organization_nid),
      l($ticket->project_title, 'node/'. $ticket->project_nid),
      l($ticket->title, 'node/'. $ticket->nid) . theme('mark', array('type' => node_mark($ticket->nid, $ticket->changed))),
      format_date($ticket->created, 'small'),
      storm_icon('status_'. $ticket->ticketstatus, storm_attribute_value('Ticket status', $ticket->ticketstatus)),
      storm_icon('priority_'. $ticket->ticketpriority, storm_attribute_value('Ticket priority', $ticket->ticketpriority)),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
//        'class' => 'storm_list_operations',
      ),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

function theme_stormticket_view($variables) {
  $node = $variables['node'];

  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );

//  $node->content['links']['expenses'] = theme('storm_link', 'stormticket', 'stormexpense', $node->nid, 1);
//  $node->content['links']['timetrackings'] = theme('storm_link', 'stormticket', 'stormtimetracking', $node->nid, 2);

  // Code to create invoice auto_add link
  if (module_exists('storminvoice')) {

    $node->content['links']['auto_invoice'] = array(
      '#prefix' => variable_get('storm_icons_display', TRUE) ? '<dt id="storminvoices" class="stormcomponent">' : '<dt class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => theme('storminvoice_autoadd_links', $node->nid, $node->billable, $node->billed),
      '#weight' => 3,
    );
  }

  $node->content['group1'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group1') : -20,
  );

  $node->content['group1']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Organization'), 'value' => l($node->organization_title, 'node/'. $node->organization_nid))),
    '#weight' => 1,
  );

  $node->content['group1']['project_nid'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Project'), 'value' => l($node->project_title, 'node/'. $node->project_nid))),
    '#weight' => 2,
  );

  $node->content['group1']['task_nid'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Task'), 'value' => l($node->task_title, 'node/'. $node->task_nid))),
    '#weight' => 2,
  );

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group2']['category'] = array(
    '#prefix' => '<div class="category">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Category'), 'value' => check_plain(storm_attribute_value('Ticket category', $node->ticketcategory)))),
    '#weight' => 1,
  );

  $node->content['group2']['status'] = array(
    '#prefix' => '<div class="storm_status">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Status'), 'value' => check_plain(storm_attribute_value('Ticket status', $node->ticketstatus)))),
    '#weight' => 2,
  );

  $node->content['group2']['priority'] = array(
    '#prefix' => '<div class="priority">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Priority'), 'value' => check_plain(storm_attribute_value('Ticket priority', $node->ticketpriority)))),
    '#weight' => 3,
  );

  $node->content['group5'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -16,
  );

  $node->content['group5']['assigned'] = array(
    '#prefix' => '<div class="assigned">',
    '#suffix' => '</div>',
    '#markup' => theme('storm_view_item', array('label' => t('Assigned to'), 'value' => l($node->assigned_title, 'node/'. $node->assigned_nid))),
  );

  $body = field_get_items('node', $node, 'body');
  if($body) {
    $node->content['body'] = array (
      '#prefix' => '<div class="stormbody">',
      '#suffix' => '</div>',
      '#markup' => theme('storm_view_item', array('label' => t('Description'), 'value' => check_plain($node->body['und'][0]['value']))),
      '#weight' => -15,
    );
  }

  return $node;
}
