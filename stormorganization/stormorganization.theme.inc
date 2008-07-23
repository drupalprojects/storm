<?php
// $Id$

/**
 * @file
 */

function theme_stormorganization_list($header, $organizations) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  $countries = stormattribute_attributes_bydomain('Country');
  foreach ($organizations as $key => $organization) {
    $n = new stdClass();
    $n->nid = $organization->nid;
    $n->uid = $organization->uid;
    $n->type = 'stormorganization';

    $rows[] = array(
      l($organization->title, 'node/'. $organization->nid),
      $countries[$organization->country],
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormorganization_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);
  unset($node->content['body']);

  $w = 0;
  $node->content['stormorganization'] = array (
    '#prefix' => '<div id="stormorganization">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fullname'] = array(
    '#prefix' => '<div class="fullname">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Fullname', $node->fullname),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['country'] = array(
    '#prefix' => '<div class="country">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Country', $node->country),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['provstate'] = array(
    '#prefix' => '<div class="provstate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Province / State', $node->provstate),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['city'] = array(
    '#prefix' => '<div class="city">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'City', $node->city),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['zip'] = array(
    '#prefix' => '<div class="zip">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Zip', $node->zip),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['address'] = array(
    '#prefix' => '<div class="address">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Address', $node->address),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['www'] = array(
    '#prefix' => '<div class="www">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'WWW', l($node->www, 'http://'. $node->www, array('absolute' => TRUE))),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['email'] = array(
    '#prefix' => '<div class="email">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Email', l($node->email, 'mailto:'. $node->email, array('absolute' => TRUE))),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['currency'] = array(
    '#prefix' => '<div class="currency">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Currency', $node->currency),
    '#weight' => $w++,
  );

  $languages = language_list('language', TRUE);
  $languages_options = array();
  foreach ($languages as $language_code => $language) {
    $languages_options[$language_code] = $language->name;
  }

  $node->content['stormorganization']['language'] = array(
    '#prefix' => '<div class="language">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Language', $languages_options[$node->orglanguage]),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['vatid'] = array(
    '#prefix' => '<div class="vatid">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'VAT id', $node->vatid),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['note'] = array(
    '#prefix' => '<div class="note">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', 'Note', $node->body),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  if (module_exists('stormperson')) {
    $node->content['stormorganization']['links']['people'] = array(
      '#prefix' => '<div class="people">',
      '#suffix' => '</div>',
      '#value' => l(t('People'), 'storm/people/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormproject')) {
    $node->content['stormorganization']['links']['projects'] = array(
      '#prefix' => '<div class="projects">',
      '#suffix' => '</div>',
      '#value' => l(t('Projects'), 'storm/projects/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormnote')) {
    $node->content['stormorganization']['links']['notes'] = array(
      '#prefix' => '<div class="notes">',
      '#suffix' => '</div>',
      '#value' => l(t('Notes'), 'storm/notes/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormticket')) {
    $node->content['stormorganization']['links']['tickets'] = array(
      '#prefix' => '<div class="tickets">',
      '#suffix' => '</div>',
      '#value' => l(t('Tickets'), 'storm/tickets/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormtimetracking')) {
    $node->content['stormorganization']['links']['timetrackings'] = array(
      '#prefix' => '<div class="timetrackings">',
      '#suffix' => '</div>',
      '#value' => l(t('Timetrackings'), 'storm/timetrackings/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  return $node;
}
