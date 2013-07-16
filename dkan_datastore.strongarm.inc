<?php
/**
 * @file
 * dkan_datastore.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dkan_datastore_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'schema_status_report';
  $strongarm->value = 0;
  $export['schema_status_report'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'schema_suppress_type_warnings';
  $strongarm->value = 1;
  $export['schema_suppress_type_warnings'] = $strongarm;

  return $export;
}
