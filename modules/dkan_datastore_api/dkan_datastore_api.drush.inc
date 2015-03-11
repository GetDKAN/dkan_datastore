<?php

/**
 * Implements hook_drush_command().
 */
function dkan_datastore_api_drush_command() {

  // TODO: Add option to update from link.
  $items['datastore-update'] = array(
    'description' => 'Updates a datastore.',
    'callback' => 'dkan_datastore_api_datastore_update',
    'arguments' => array(
      'resource_id' => 'Id or alias of the data that is going to be updated.',
      'file' => 'The path to the file.'
    ),
  );

  return $items;
}

/**
 * Callback for the update-datastore-by-file command
 */
function dkan_datastore_api_datastore_update($resource_id = NULL, $file = NULL) {
  
  // IMPROVEMENT: Add option truncate
  // IMPROVEMENT: Add option keep_prev_file

  // Retrieve the datastore asociated with the resource that's going to
  // be updated.
  $datastore = dkan_datastore_go($resource_id);
 
  $datastore->updateByFile($file);
}