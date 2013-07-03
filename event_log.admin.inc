<?php

/**
 * @file
 * The 'Event log' module admin pages.
 *
 * @author Bas van Meurs
 * @copyright Cipix Internet
 */

/**
 * Page callback that shows all logged events.
 */
function event_log_overview_page() {
  $table = array(
    '#theme' => 'table',
    '#header' => array(
      array('data' => 'created', 'field' => 'created', 'sort' => 'desc'),
      array('data' => 'type', 'field' => 'type'),
      array('data' => 'operation', 'field' => 'operation'),
      array('data' => 'path', 'field' => 'path'),
      array('data' => 'form id', 'field' => 'form_id'),
      array('data' => 'description', 'field' => 'description'),
      array('data' => 'user', 'field' => 'uid'),
      array('data' => 'ip', 'field' => 'ip'),
      array('data' => 'ID', 'field' => 'ref_numeric'),
      array('data' => 'Name', 'field' => 'ref_char'),
      array('data' => 'info'),
    ),
    '#rows' => array(),
  );

  $query = db_select('event_log', 'c')
    ->fields('c')
    ->extend('TableSort')
    ->orderByHeader($table['#header'])
    ->extend('PagerDefault')
    ->limit(50);
  $result = $query->execute();

  foreach ($result as $record) {
    if (!empty($record->uid)) {
      $account = user_load($record->uid);
    }
    else {
      $account = NULL;
    }
    $table['#rows'][] = array(
      array('data' => check_plain(date("Y-m-d H:i:s", $record->created))),
      array('data' => check_plain($record->type)),
      array('data' => check_plain($record->operation)),
      array('data' => check_plain($record->path)),
      array('data' => check_plain($record->form_id)),
      array('data' => $record->description),
      array('data' => (empty($account) ? '' : l($account->name, 'user/' . $account->uid))),
      array('data' => check_plain($record->ip)),
      array('data' => check_plain($record->ref_numeric)),
      array('data' => check_plain($record->ref_char)),
      array('data' => (empty($record->info) ? '' : '<div class="event-log-info"><pre>' . check_plain($record->info) . '</pre></div>')),
    );
  }

  $render = array(
    'table' => $table,
    'pager' => array('#theme' => 'pager'),
  );

  drupal_add_js(drupal_get_path('module', 'event_log') . '/js/event_log.admin.js');

  return $render;
}
