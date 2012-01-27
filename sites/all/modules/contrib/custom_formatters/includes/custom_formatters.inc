<?php
/**
 * @file
 * Custom Formatters module integration.
 */

/**
 * Implements hook_custom_formatters_defaults().
 */
function custom_formatters_custom_formatters_defaults() {
  $formatters = array();

  // Example: File (advanced).
  $formatters['example_file'] = array(
    'name' => 'example_file',
    'label' => 'Example: File (advanced)',
    'field_types' => 'a:1:{i:0;s:9:"filefield";}',
    'multiple' => '0',
    'description' => 'An advanced example custom formatter for the "filefield" field type.',
    'mode' => 'advanced',
    'code' => 'return $element[\'#item\'][\'filepath\'] . " " . l(t("<sup>Custom Formatter Example</sup>"), \'admin/build/formatters\', array(\'html\' => TRUE));',
    'status' => '0',
  );

  // Example: Image (basic).
  $formatters['example_image'] = array(
    'name' => 'example_image',
    'label' => 'Example: Image (basic)',
    'field_types' => 'a:1:{i:0;s:9:"filefield";}',
    'multiple' => '0',
    'description' => 'A basic example custom formatter for the "filefield" field type.',
    'mode' => 'basic',
    'code' => '<img src="[site-url]/[filefield-filepath]" alt="[filefield-alt]" title="[filefield-title]" /> <a href="[site-url]/admin/build/formatters"><sup>Custom Formatter Example</sup></a>',
    'status' => '0',
  );

  // Example: Multiple fields.
  $formatters['example_basic'] = array(
    'name' => 'example_basic',
    'label' => 'Example: Multiple fields',
    'field_types' => 'a:4:{i:0;s:14:"number_decimal";i:1;s:12:"number_float";i:2;s:14:"number_integer";i:3;s:4:"text";}',
    'multiple' => '0',
    'description' => 'An advanced example custom formatter for an assorted group of basic field types.',
    'mode' => 'advanced',
    'code' => 'return $element[\'#item\'][\'value\'] . " " . l(t("<sup>Custom Formatter Example</sup>"), \'admin/build/formatters\', array(\'html\' => TRUE));',
    'status' => '0',
  );

  // Example: Multiple values.
  $formatters['example_basic_multiples'] = array(
    'name' => 'example_basic_multiples',
    'label' => 'Example: Multiple values',
    'field_types' => 'a:4:{i:0;s:14:"number_decimal";i:1;s:12:"number_float";i:2;s:14:"number_integer";i:3;s:4:"text";}',
    'multiple' => '1',
    'description' => 'An advanced example custom formatter for an assorted group of basic field types, with support for multiple values.',
    'mode' => 'advanced',
    'code' => '$output = \'\';
foreach (element_children($element) as $key) {
  $output .= \'<div>\' . $element[$key][\'#item\'][\'value\'] . " " . l(t("<sup>Custom Formatter Example</sup>"), \'admin/build/formatters\', array(\'html\' => TRUE)) . \'</div>\';
}
return $output;',
    'status' => '0',
  );

  return $formatters;
}
