<?php
/**
 * @file
 * Provides Custom Formatters integration with the Features module.
 */

/**
 * Implements hook_features_api().
 */
function custom_formatters_features_api() {
  return array(
    'custom_formatters' => array(
      'name' => t('Custom Formatters'),
      'feature_source' => TRUE,
      'default_hook' => 'custom_formatters_defaults',
      'default_file' => FEATURES_DEFAULTS_INCLUDED_COMMON,
    ),
  );
}

/**
 * Implements hook_features_export_options().
 */
function custom_formatters_features_export_options() {
  $formatters = array();

  foreach (custom_formatters_formatters() as $id => $formatter) {
    $formatters[$formatter->name] = $formatter->label;
  }

  return $formatters;
}

/**
 * Implements hook_features_export().
 */
function custom_formatters_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['custom_formatters'] = 'custom_formatters';
  foreach ($data as $name) {
    $formatter = custom_formatters_formatter($name);

    $export['features']['custom_formatters'][$formatter->name] = $formatter->name;
  }
}

/**
 * Implements hook_features_export_render().
 */
function custom_formatters_features_export_render($module_name = '', $data) {
  module_load_include('inc', 'custom_formatters', 'custom_formatters.admin');
  $formatters = array();
  foreach ($data as $name) {
    $formatter = custom_formatters_formatter($name);
    unset($formatter->override);
    $formatters[$formatter->name] = $formatter;
    $formatters[$formatter->name]->source = 'module';
  }

  return array(
'custom_formatters_defaults' => theme('custom_formatters_export_features', $formatters),
  );
}

/**
 * Implementation of hook_features_revert().
 */
function custom_formatters_features_revert($module) {
  if ($default_formatters = features_get_default('custom_formatters', $module)) {
    foreach (array_keys($default_formatters) as $default_formatter) {
      if ($formatter = custom_formatters_formatter($default_formatter)) {
        db_query("DELETE FROM {formatters} WHERE name = '%s'", $default_formatter);
        custom_formatters_clear_cache();
      }
    }
  }
}

/**
 * Implements hook_features_pipe_component_alter().
 */
function custom_formatters_features_pipe_content_alter(&$pipe, $data, $export) {
  foreach ($data as $field) {
    $args = explode('-', $field);
    $field = content_fields($args[1], $args[0]);
    foreach ($field['display_settings'] as $child) {
      if (is_array($child) && isset($child['format']) && strpos($child['format'], 'custom_formatters_') === 0) {
        $name = drupal_substr($child['format'], 18);
        if (is_object(custom_formatters_formatter($name)) && !in_array($name, array_keys(module_invoke_all('custom_formatters_defaults')))) {
          $pipe['custom_formatters'][$name] = $name;
        }
      }
    }
  }
}

/**
 * Implements hook_features_pipe_component_alter().
 */
function custom_formatters_features_pipe_views_alter(&$pipe, $data, $export) {
  foreach ($data as $view) {
    $view = views_get_view($view);
    foreach ($view->display as $display) {
      if ($display->display_options['fields']) {
        foreach ($display->display_options['fields'] as $field) {
          if (isset($field['format'])) {
            $name = drupal_substr($field['format'], 18);
            if (is_object(custom_formatters_formatter($name)) && !in_array($name, array_keys(module_invoke_all('custom_formatters_defaults')))) {
              $pipe['custom_formatters'][$name] = $name;
            }
          }
        }
      }
    }
  }
}
