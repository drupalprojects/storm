<?php
// $Id$

/**
 * @file
 * Provides a theme function to create the Storm Dashboard
 */

/**
 * @function
 * Theme function to create the Storm Dashboard
 */
// See 498108: Proposed to refactor creation of Storm Dashboard
function theme_stormdashboard_dashboard() {
  $o = '<div id="stormdashboard">';
  $o .= '<dl class="stormdashboard">';
  
  $o .= '<div class="stormdashboard">';
  $o .= theme('storm_link', '', 'stormorganization');
  $o .= theme('storm_link', '', 'stormperson');
  $o .= theme('storm_link', '', 'stormteammember');
  $o .= theme('storm_link', '', 'stormproject');
  $o .= theme('storm_link', '', 'stormtask');
  $o .= theme('storm_link', '', 'stormticket');
  $o .= theme('storm_link', '', 'stormtimetracking');
  $o .= '</div>';

  $o .= '<div class="stormdashboard">';
  $o .= theme('storm_link', '', 'stormnote');
  $o .= theme('storm_link', '', 'storminvoice');
  $o .= theme('storm_link', '', 'stormexpense');
  $o .= theme('storm_link', '', 'stormknowledgebase');
  $o .= theme('storm_link', '', 'stormattribute');
  $o .= theme('storm_link', '', 'stormconfiguration');
  
  $o .= '</div>';
  $o .= '</dl>';
  $o .= '</div>';
  return $o;
}
