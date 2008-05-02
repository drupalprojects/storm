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

function theme_stormtimetracking_list_report($format, $timetrackings) {
  if ($format=='pdf') {
  
    include_once(drupal_get_path('module', 'storm') . '/libs/fpdf/fpdf.php');
    $pdf=new FPDF('L');
    $pdf->AddPage();
    $pdf->SetFont('Arial','',10);

    $header = array (
                  t('Project') => 50,
                  t('Task') => 50,
                  t('Ticket') => 50,
                  t('Title') => 50,
                  t('Date') => 40,
                  t('Duration (h)') => 40,
                  );

    $pdf->SetFont('','B');
    foreach($header as $col=>$width) {
      $pdf->Cell($width,7,$col,1);
    }
    $pdf->Ln();

    $pdf->SetFont('','');
    foreach($timetrackings as $timetracking) {
      $pdf->Cell(50,6,$timetracking->project_title,1);
      $pdf->Cell(50,6,$timetracking->task_title,1);
      $pdf->Cell(50,6,$timetracking->ticket_title,1);
      $pdf->Cell(50,6,$timetracking->title,1);
      $pdf->Cell(40,6,format_date($timetracking->trackingdate, 'small'), 1);
      $pdf->Cell(40,6,sprintf('%.2f', $timetracking->duration),1, 0, 'R');
     $pdf->Ln();
    }

    //$pdf->Image(drupal_get_path('module', 'storm') . '/tmp/timetrackings.png', 40, 70);

    $pdf->Output('storm_timetrackings_report.pdf', 'I');
  }
}
