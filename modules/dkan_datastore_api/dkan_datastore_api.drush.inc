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
      'id' => 'Datastore (or resource) id.'
    ),
  );  
  
  // Get the number of rows in the datastore.
  $items['datastore-rows'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),
    'aliases' => array('dsr'),
    'description' => 'Get the number of rows in the datastore.',
    'callback' => 'dkan_datastore_api_datastore_rows',
    'arguments' => array(
      'id' => 'Datastore (or resource) id.'
    ),
  );    

  // Update datastore file.
  $items['datastore-file-update'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),
    'aliases' => array('dsfu'),
    'description' => 'Updates the datastore file.',
    'callback' => 'dkan_datastore_api_datastore_file_update',
    'arguments' => array(
      'id' => 'Datastore (or resource) id.',
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
      'id' => 'Datastore (or resource) id.'
    ),
  );  
  
  // Show the URI of the datastore file
  $items['datastore-file-uri'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),  
    'aliases' => array('dsfuri'),  
    'description' => 'Shows the URI of the datastore file.',
    'callback' => 'dkan_datastore_api_datastore_file_uri',
    'arguments' => array(
      'id' => 'Datastore (or resource) id.'
    ),
  );     
  
  // Show the URL of the datastore file
  $items['datastore-file-url'] = array(
    'drupal dependencies' => array('dkan_datastore_api'),  
    'aliases' => array('dsfurl'),  
    'description' => 'Shows the URL of the datastore file.',
    'callback' => 'dkan_datastore_api_datastore_file_url',
    'arguments' => array(
      'id' => 'Datastore (or resource) id.'
    ),      
  ); 

  return $items;
}

/**
 * Callback for the datastore-delete command
 */
function dkan_datastore_api_datastore_delete($id = NULL) {
  
  if ($datastore = get_datastore($id)) {
    drupal_set_message('The command is not implemented yet.');
  }
}

/**
 * Callback for the datastore-rows command
 */
function dkan_datastore_api_datastore_rows($id = NULL) {
  
  if ($datastore = get_datastore($id)) {
    drupal_set_message('The command is not implemented yet.');
  }
}

/**
 * Callback for the datastore-file-update command
 */
function dkan_datastore_api_datastore_file_update($id = NULL, $file = NULL) {
  
  if ($datastore = get_datastore($id)) {
    // IMPROVEMENT: Add option truncate
    // IMPROVEMENT: Add option keep_prev_file
    $datastore->updateByFile($file);
  }
}

/**
 * Callback for the datastore-file-delete command
 */
function dkan_datastore_api_datastore_file_delete($id = NULL) {
  
  if ($datastore = get_datastore($id)) {
    $datastore->deleteFile();
  }
}

/**
 * Callback for the datastore-file-uri command
 */
function dkan_datastore_api_datastore_file_uri($id = NULL) {
  
  if ($datastore = get_datastore($id)) {  
    drupal_set_message('The URI of the file in the datastore is: ' . $datastore->fileUri());
  }
}

/**
 * Callback for the datastore-file-url command
 */
function dkan_datastore_api_datastore_file_url($id = NULL) {
  
  if ($datastore = get_datastore($id)) {  
    drupal_set_message('The URL of the file in the datastore is: ' . $datastore->fileUrl());
  }
}

/*
 * Get datastore based on id
 */
function get_datastore($id) {
  
  // Show an error if the resource_id is not present
  if (!$id) {
    drupal_set_message('No datastore id was provided.', 'error');
    return false;
  }
  
  // Search for a datastore asociated with the resource
  try {
    $datastore = dkan_datastore_go($id);
    
  } catch (Exception $e) {
    drupal_set_message('There are no datastores with that id.', 'error');
    return false;
  }
  
  return $datastore;
}