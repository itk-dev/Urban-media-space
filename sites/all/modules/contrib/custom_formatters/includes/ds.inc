<?php
/**
 * @file
 * Provides Custom Formatters integration with the Display Suite module and submodules.
 */

/**
 * Implements hook_type_alter().
 */
function custom_formatters_ds_fields_alter(&$fields) {
  foreach (custom_formatters_formatters() as $formatter) {
    $field_types = implode(' ', unserialize($formatter->field_types));
    if (preg_match_all('/\bnd_(.*?)\b/', $field_types, $matches) || preg_match_all('/\bud_(.*?)\b/', $field_types, $matches) || preg_match_all('/\bvd_(.*?)\b/', $field_types, $matches)) {
      foreach ($matches[1] as $key) {
        if (isset($fields[$key])) {
          $fields[$key]['properties']['formatters']["custom_formatters_formatter_custom_formatters_{$formatter->name}"] = t('Custom: !label', array('!label' => $formatter->label));
        }
      }
    }
  }
}
