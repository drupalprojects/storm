<?php
// $Id$

function _storm_timestamp_to_gm($timestamp, $timezone=NULL) {
  if (!isset($timezone)) {
    global $user;
    if (variable_get('configurable_timezones', 1) && $user->uid && strlen($user->timezone)) {
      $timezone = $user->timezone;
    }
    else {
      $timezone = variable_get('date_default_timezone', 0);
    }
  }
  $timestamp -= $timezone;
  return $timestamp;
}

function _storm_date_to_gmtimestamp($date, $timezone=NULL) {
  $gmttimestamp = gmmktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
  return _storm_timestamp_to_gm($gmttimestamp, $timezone);
}

function _storm_datetime_to_gmtimestamp($datetime, $timezone=NULL) {
  $gmttimestamp = gmmktime($datetime['hour'], $datetime['minute'], 0, $datetime['month'], $datetime['day'], $datetime['year']);
  return _storm_timestamp_to_gm($gmttimestamp, $timezone);
}

function _storm_gmtimestamp_to_datetime($timestamp, $timezone=NULL) {
  $datetime = array(
    'day' => format_date($timestamp, 'custom', 'j', $timezone),
    'month' => format_date($timestamp, 'custom', 'n', $timezone),
    'year' => format_date($timestamp, 'custom', 'Y', $timezone),
    'hour' => (int)format_date($timestamp, 'custom', 'H', $timezone),
    'minute' => (int)format_date($timestamp, 'custom', 'i', $timezone),
  );
  return $datetime;
}

function _storm_gmtimestamp_to_date($timestamp, $timezone=NULL) {
  $date = array(
    'day' => format_date($timestamp, 'custom', 'j', $timezone),
    'month' => format_date($timestamp, 'custom', 'n', $timezone),
    'year' => format_date($timestamp, 'custom', 'Y', $timezone),
  );
  return $date;
}

function _storm_gmtimestamp_without_time($timestamp, $timezone=NULL) {
  $date = _storm_gmtimestamp_to_date($timestamp, $timezone);
  $gmttimestamp = gmmktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
  return _storm_timestamp_to_gm($gmttimestamp, $timezone);
}

function _storm_strtotime($timestr='') {
  $timestr = substr($timestr, 0, 5);
  $time['hour'] = 0;
  $time['minute'] = 0;

  $ar = explode(':', $timestr);
  if (is_array($ar)) {
    if (array_key_exists(0, $ar)) $time['hour'] = $ar[0];
    if (array_key_exists(0, $ar)) $time['minute'] = $ar[1];
  }
  return $time;
}

function _storm_timetostr($time=array()) {
  $timestr = str_pad($time['hour'], 2, "0", STR_PAD_LEFT) .':'. str_pad($time['minute'], 2, "0", STR_PAD_RIGHT);
  return $timestr;
}

function storm_rewrite_sql($sql, $where=array(), $join=array()) {
  $where = empty($where) ? '' : '('. implode(') AND (', $where) .')';
  $join = empty($join) ? '' : implode(' ', $join);

  if (!empty($where) || !empty($join)) {
    if (!empty($where)) {
      $new = "WHERE $where ";
    }
    $new = " $join $new";
    if (strpos($sql, 'WHERE')) {
      $sql = str_replace('WHERE', $new .'AND (', $sql);
      $insert = ') ';
    }
    else {
      $insert = $new;
    }
    if (strpos($sql, 'GROUP')) {
      $replace = 'GROUP';
    }
    elseif (strpos($sql, 'HAVING')) {
      $replace = 'HAVING';
    }
    elseif (strpos($sql, 'ORDER')) {
      $replace = 'ORDER';
    }
    elseif (strpos($sql, 'LIMIT')) {
      $replace = 'LIMIT';
    }
    else {
      $sql .= $insert;
    }
    if (isset($replace)) {
      $sql = str_replace($replace, $insert . $replace, $sql);
    }
  }

  return $sql;
}
