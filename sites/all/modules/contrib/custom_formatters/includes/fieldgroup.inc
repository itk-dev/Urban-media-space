<?php
/**
 * @file
 * Fieldgroup module integration.
 */

/**
 * Implements hook_custom_formatters_theme().
 */
function fieldgroup_custom_formatters_theme(&$theme) {
  $theme['custom_formatters_fieldgroup'] = array(
    'arguments' => array('element' => NULL),
    'file' => 'modules/fieldgroup.inc',
  );
}

/**
 * Implements hook_custom_formatters_form_alter().
 */
function fieldgroup_custom_formatters_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'content_display_overview_form') {
    $settings = variable_get('custom_formatters_settings', array('label_prefix' => TRUE, 'label_prefix_value' => t('Custom')));
    foreach (custom_formatters_formatters() as $formatter) {
      if ($formatter->status && in_array('fieldgroup', unserialize($formatter->field_types))) {
        $label = $settings['label_prefix'] ? "{$settings['label_prefix_value']}: {$formatter->name}" : $formatter->name;
        foreach (fieldgroup_groups($form['#type_name']) as $group) {
          if ($group['group_type'] == 'standard') {
            $form[$group['group_name']]['full']['format']['#options'] = array_merge(
              array("custom_formatters_{$formatter->name}" => $label),
              $form[$group['group_name']]['full']['format']['#options']
            );
            $form[$group['group_name']]['teaser']['format']['#options'] = array_merge(
              array("custom_formatters_{$formatter->name}" => $label),
              $form[$group['group_name']]['teaser']['format']['#options']
            );
          }
        }
      }
    }
  }
}

/**
 * Implements hook_type_alter().
 */
function fieldgroup_custom_formatters_fields_alter(&$fields) {
  $fields['fieldgroup'] = array(
    'label' => 'Fieldgroup',
    'module' => 'fieldgroup',
  );
}

/**
 * Implements hook_fieldgroup_view().
 */
function custom_formatters_fieldgroup_view(&$node, &$element, $group, $context) {
  if (function_exists($function = "custom_formatters_fieldgroup_view_{$group['group_type']}")) {
    $function($node, $element, $group, $context);
  }

  $element['#node'] = $node;
}

function custom_formatters_fieldgroup_view_standard(&$node, &$element, $group, $context) {
  $element['#theme'] = 'custom_formatters_fieldgroup';
  $element['#formatter'] = $group['settings']['display'][$context]['format'];
}

function theme_custom_formatters_fieldgroup($element) {
  $output = '';

  $element['#item'] = array();
  foreach (element_children($element) as $field) {
    $element['#item'][$field] = array();
    foreach (element_children($element[$field]['field']['items']) as $item) {
      $element['#item'][$field][$element[$field]['field']['items'][$item]['#item']['#delta']] = $element[$field]['field']['items'][$item]['#item'];
      $element['#item'][$field][$element[$field]['field']['items'][$item]['#item']['#delta']]['view'] = theme($element[$field]['field']['items'][$item]['#theme'], $element[$field]['field']['items'][$item]);
      unset($element['#item'][$field][$element[$field]['field']['items'][$item]['#item']['#delta']]['#delta']);
    }

    $element['#item'][$field]['view'] = drupal_render($element[$field]);
  }

  return theme('custom_formatters_formatter', $element);
}

function fieldgroup_content_generate($node, $field) {
  $item = array();
  foreach (fieldgroup_groups($node['type']) as $group) {
    if ($group['group_type'] == 'standard') {
      foreach ($group['fields'] as $field) {
        if (function_exists($function = "{$field['widget_module']}_content_generate")) {
          $item[$field['field_name']][0] = $function($node, content_fields($field['field_name']));

          $element = array(
            '#node' => $node,
            '#item' => $item[$field['field_name']],
          );
          $item[$field['field_name']][0]['view'] = theme("{$field['widget_module']}_formatter_default", $element);
        }
      }
    }
  }
  return $item;
}

/**
 * Implements hook_token_list().
 */
function fieldgroup_token_list($type = 'all') {
  $tokens = array('fieldgroup' => array());

  foreach (fieldgroup_groups() as $groups) {
    foreach ($groups as $group) {
      if ($group['group_type'] == 'standard') {
        foreach ($group['fields'] as $field) {
          $tokens['fieldgroup']["fieldgroup-{$field['field_name']}"] = t('Themed results of the "!field" field.', array('!field' => $field['label']));
        }
      }
    }
  }

  return $tokens;
}

/**
 * Implements hook_token_values().
 */
function fieldgroup_token_values($type, $object = array()) {
  $values = array();

  if ($type == 'field') {
    foreach (fieldgroup_groups() as $groups) {
      foreach ($groups as $group) {
        if ($group['group_type'] == 'standard') {
          foreach ($group['fields'] as $field) {
            if (isset($object[0][$field['field_name']])) {
              $values["fieldgroup-{$field['field_name']}"] = $object[0][$field['field_name']]['view'];
            }
          }
        }
      }
    }
  }

  return $values;
}
