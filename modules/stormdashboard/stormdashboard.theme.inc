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
  $o .= theme_storm_link('', 'stormorganization');
  $o .= theme_storm_link('', 'stormperson');
  $o .= theme_storm_link('', 'stormteammember');
  $o .= theme_storm_link('', 'stormproject');
  $o .= theme_storm_link('', 'stormtask');
  $o .= theme_storm_link('', 'stormticket');
  $o .= theme_storm_link('', 'stormtimetracking');
  $o .= '</div>';

  $o .= '<div class="stormdashboard">';
  $o .= theme_storm_link('', 'stormnote');
  $o .= theme_storm_link('', 'storminvoice');
  $o .= theme_storm_link('', 'stormexpense');
  $o .= theme_storm_link('', 'stormknowledgebase');
  $o .= theme_storm_link('', 'stormattribute');
  $o .= theme_storm_link('', 'stormconfiguration');
  
  $o .= '</div>';
  $o .= '</dl>';
  $o .= '</div>';
  return $o;
}
