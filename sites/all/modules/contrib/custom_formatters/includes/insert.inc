<?php
/**
 * @file
 * Provides Custom Formatters integration with the Insert module.
 */

/**
 * Implements hook_insert_styles().
 */
function custom_formatters_insert_styles() {
  $settings = variable_get('custom_formatters_settings', array('label_prefix' => TRUE, 'label_prefix_value' => t('Custom')));
  $formatters = custom_formatters_formatters();
  $insert_styles = array();
  foreach ($formatters as $formatter) {
    if ($formatter->status && !$formatter->multiple && in_array('filefield', unserialize($formatter->field_types))) {
      $label = $settings['label_prefix'] ? "{$settings['label_prefix_value']}: {$formatter->name}" : $formatter->name;
      $insert_styles['custom_formatters_' . $formatter->name] = array(
        'label' => $label,
      );
    }
  }
  return $insert_styles;
}

/**
 * Implementation of hook_insert_content().
 */
function custom_formatters_insert_content($item, $style, $widget) {
  $item['data']['description'] = $item['data']['description'] == '' ? '__description__' : $item['data']['description'];

  $element = array(
    '#formatter' => $style['name'],
    '#item' => $item,
    '#node' => (object) $_POST,
  );

  return theme('custom_formatters_formatter_' . $element['#formatter'], $element);
}
