<?php

/**
 * @file
 * Callbacks for datastore pages.
 */

/**
 * Copy of feeds_access.
 */
function dkan_datastore_feeds_access($action, $param) {
  if (!in_array($action, array('import', 'clear', 'unlock'))) {
    // If $action is not one of the supported actions, we return access denied.
    return FALSE;
  }

  if (is_string($param)) {
    $importer_id = $param;
  }
  elseif ($param->type) {
    $importer_id = feeds_get_importer_id($param->type);
  }

  // Check for permissions if feed id is present, otherwise return FALSE.
  if ($importer_id) {
    if (user_access('administer feeds') || user_access("{$action} {$importer_id} feeds")) {
      return TRUE;
    }
  }
  return FALSE;
}
/**
 * Access callback for Data API instructions.
 */
function dkan_datastore_datastore_api_access($node) {
  if ($node->type != 'resource' || !isset($node->field_upload[$node->language][0])) {
    return FALSE;
  }
  else {
    return node_access('view', $node);
  }
}

/**
 * Access callback for download tab.
 */
function dkan_datastore_download_access($node) {
  if ($node->type != 'resource' || !isset($node->field_upload[$node->language][0])) {
    return FALSE;
  }
  else {
    return node_access('view', $node);
  }
}

/**
 * Access callback for back link.
 */
function dkan_datastore_back_access($node) {
  if ($node->type != 'resource') {
    return FALSE;
  }
  else {
    return node_access('view', $node);
  }
}

/**
 * Access callback for 'Add Resource' tab.
 */
function dkan_add_resource($node) {
  if ($node->type != 'dataset') {
    return FALSE;
  }
  else {
    return _node_add_access();
  }
}

/**
 * Callback for Data API instructions.
 */
function dkan_dataset_datastore_api($node) {
  $output = '<h2>' . t('DKAN Datastore API') . '</h2>';
  $output .= t('Access resource data via a web API with powerful query support.');
  $output .=  '<h3>' . t('Resource ID') . '</h3>';
  $output .= t('The Resource ID for this resource is ') . $node->uuid;
  $output .=  '<h3>' . t('Example Query') . '</h3>';
  GLOBAL $base_url;
  $url = $base_url . '/api/action/datastore/search.json?resource_id=' . $node->uuid . '&limit=5';
  $output .= l($url, $url);
  $output .= '<p>' . t('Query this datastore and return first five results') . '</p>';
  return $output;
}

/**
 * Callback for download tab.
 */
function dkan_dataset_download($node) {
  $uri = isset($node->field_upload[$node->language][0]['uri']) ? $node->field_upload[$node->language][0]['uri'] : $node->field_upload[0]['uri'];
  $url = file_create_url($uri);
  return drupal_goto($url);
}

/**
 * Callback for back link.
 */
function dkan_dataset_back($node) {
  if (isset($node->field_dataset_ref[$node->language][0]['target_id'])) {
    drupal_goto('node/' . $node->field_dataset_ref[$node->language][0]['target_id']);
  }
  return '';
}

/**
 * Callback for 'Add Resouce' tab.
 */
function dkan_dataset_add_resource($node) {
  drupal_goto('node/add/resource', array('query' => array('dataset' => $node->nid)));
}
