<?php
/**
 * @file
 * Hooks provided by the Custom Formatters module.
 */

/**
 * Prepare a CCK field array for use with Devel Generate.
 *
 * @return
 *   A keyed array with keys defined as necessary for the $field array passed
 *   to your modules implementation of Devel Generates hook_content_generate().
 */
function hook_custom_formatters_field_prepare() {
  file_check_directory($path = file_directory_path() . '/._tmp.custom_formatters', TRUE);
  return array(
    'widget' => array(
      'file_extensions' => 'jpg png',
      'file_path' => '._tmp.custom_formatters'
    ),
  );
}

/**
 * Inform Custom Formatters of modules with available tokens for  a specific
 * CCK field.
 *
 * @return
 *   An array of modules.
 */
function hook_custom_formatters_field_tokens() {
  return array(
    'filefield_paths',
    'file_aliases',
  );
}

/**
 * Provide your own default Custom Formatters.
 *
 * @return
 *   An array containing Custom Formatters settings.
 */
function hook_custom_formatters_defaults() {
  return array(
    'image' => array(
      'name' => 'image',
      'label' => 'Image',
      'field_types' => 'a:1:{i:0;s:9:"filefield";}',
      'multiple' => '0',
      'description' => '',
      'mode' => 'basic',
      'code' => '<img src="[site-url]/[filefield-filepath]" alt="" />',
      'status' => 1,
    ),
  );
}
