<?php

/**
 * Implements hook_drush_command().
 */
function dkan_datastore_api_drush_command() {
  
  // Delete datastore.
  $items['datastore-delete'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),
    'aliases' => array('dsd'),
    'description' => 'Deletes a datastore.',
    'callback' => 'dkan_datastore_api_datastore_delete',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource asociated with the Datastore.'
    ),
  );  
  
  // Get the number of rows in the datastore.
  $items['datastore-rows'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),
    'aliases' => array('dsr'),
    'description' => 'Get the number of rows in the datastore.',
    'callback' => 'dkan_datastore_api_datastore_rows',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource asociated with the Datastore.'
    ),
  );    

  // Update datastore file.
  $items['datastore-file-update'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),
    'aliases' => array('dsfu'),
    'description' => 'Updates the file on a datastore.',
    'callback' => 'dkan_datastore_api_datastore_file_update',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource that is going to be updated.',
      'file' => 'The path to the file.'
    ),
  );
  
  // Delete datastore file
  $items['datastore-file-delete'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),  
    'aliases' => array('dsfd'),
    'description' => 'Deletes the file on a datastore.',
    'callback' => 'dkan_datastore_api_datastore_file_delete',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource that is going to be updated.'
    ),
  );  
  
  // Show the URI of the datastore file
  $items['datastore-file-uri'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),  
    'aliases' => array('dsfuri'),  
    'description' => 'Shows the URI of the datastore file.',
    'callback' => 'dkan_datastore_api_datastore_file_uri',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource that holds the file.'
    ),
  );     
  
  // Show the URL of the datastore file
  $items['datastore-file-url'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),  
    'aliases' => array('dsfurl'),  
    'description' => 'Shows the URL of the datastore file.',
    'callback' => 'dkan_datastore_api_datastore_file_url',
    'arguments' => array(
      'resource_id' => 'Id or alias of the resource that holds the file.'
    ),      
  ); 

  return $items;
}

/**
 * Callback for the datastore-delete command
 */
function dkan_datastore_api_datastore_delete($resource_id = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {
    drupal_set_message('The command is not implemented yet.');
  }
}

/**
 * Callback for the datastore-rows command
 */
function dkan_datastore_api_datastore_rows($resource_id = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {
    drupal_set_message('The command is not implemented yet.');
  }
}

/**
 * Callback for the datastore-file-update command
 */
function dkan_datastore_api_datastore_file_update($resource_id = NULL, $file = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {
    // IMPROVEMENT: Add option truncate
    // IMPROVEMENT: Add option keep_prev_file
    $datastore->updateByFile($file);
  }
}

/**
 * Callback for the datastore-file-delete command
 */
function dkan_datastore_api_datastore_file_delete($resource_id = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {
    drupal_set_message('The command is not implemented yet.');
  }
}

/**
 * Callback for the datastore-file-uri command
 */
function dkan_datastore_api_datastore_file_uri($resource_id = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {  
    drupal_set_message('The URI of the file in the datastore is: ' . $datastore->fileUri());
  }
}

/**
 * Callback for the datastore-file-url command
 */
function dkan_datastore_api_datastore_file_url($resource_id = NULL) {
  
  if ($datastore = get_datastore($resource_id)) {  
    drupal_set_message('The URL of the file in the datastore is: ' . $datastore->fileUrl());
  }
}

/*
 * Get the datastore asociated with a Resource with $resource_id
 */
function get_datastore( $resource_id ) {
  
  // Show an error if the resource_id is not present
  if ( !$resource_id ) {
    drupal_set_message('No resource_id was provided.', 'error');
    return false;
  }
  
  // Search for a datastore asociated with the resource
  try {
    $datastore = dkan_datastore_go($resource_id);
    
  } catch (Exception $e) {
    drupal_set_message('The resource_id was not found.', 'error');
    return false;
  }
  
  return $datastore;
}