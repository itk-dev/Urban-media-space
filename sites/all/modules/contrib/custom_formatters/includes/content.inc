<?php
/**
 * @file
 * Provides Custom Formatters integration with the CCK module.
 */

/**
 * Implements hook_custom_formatters_field_prepare().
 */
function number_custom_formatters_number_decimal_prepare() {
  return array(
    'min' => 1,
    'max' => NULL,
    'precision' => NULL,
    'scale' => NULL,
  );
}

/**
 * Implements hook_custom_formatters_field_prepare().
 */
function number_custom_formatters_number_float_prepare() {
  return array(
    'min' => 1,
    'max' => NULL,
  );
}

/**
 * Implements hook_custom_formatters_field_prepare().
 */
function number_custom_formatters_number_integer_prepare() {
  return array(
    'min' => 1,
    'max' => NULL,
  );
}
