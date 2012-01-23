<?php
/**
 * @file
 * Provides Custom Formatters integration with the Content Multigroup module.
 */

/**
 * Implements hook_custom_formatters_theme().
 */
function content_multigroup_custom_formatters_theme(&$theme) {
  $theme['custom_formatters_content_multigroup'] = array(
    'arguments' => array('element' => NULL),
    'file' => 'modules/content_multigroup.inc',
  );
}

/**
 * Implements hook_custom_formatters_form_alter().
 */
function content_multigroup_custom_formatters_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'content_display_overview_form') {
    $settings = variable_get('custom_formatters_settings', array('label_prefix' => TRUE, 'label_prefix_value' => t('Custom')));
    foreach (custom_formatters_formatters() as $formatter) {
      if ($formatter->status && in_array('content_multigroup', unserialize($formatter->field_types))) {
        $label = $settings['label_prefix'] ? "{$settings['label_prefix_value']}: {$formatter->name}" : $formatter->name;
        foreach (fieldgroup_groups($form['#type_name']) as $group) {
          if ($group['group_type'] == 'multigroup') {
            $form[$group['group_name'] . '_subgroup']['full']['format']['#options'] += array('custom_formatters_' . $formatter->name => $label);
            $form[$group['group_name'] . '_subgroup']['teaser']['format']['#options'] += array('custom_formatters_' . $formatter->name => $label);
          }
        }
      }
    }
  }
}

/**
 * Implements hook_type_alter().
 */
function content_multigroup_custom_formatters_fields_alter(&$fields) {
  $fields['content_multigroup'] = array(
    'label' => 'Content Multigroup',
    'module' => 'content_multigroup',
  );
}

function custom_formatters_fieldgroup_view_multigroup(&$node, &$element, $group, $context) {
  $subgroup_settings = isset($group['settings']['multigroup']['subgroup']) ? $group['settings']['multigroup']['subgroup'] : array();
  $subgroup_format = isset($subgroup_settings[$context]['format']) ? $subgroup_settings[$context]['format'] : 'fieldset';

  if (strpos($subgroup_format, 'custom_formatters_') === 0) {
    $element['#node'] = $node;
    $element['#theme'] = 'custom_formatters_content_multigroup';
    $element['#formatter'] = $subgroup_format;
  }
}

function theme_custom_formatters_content_multigroup($element) {
  $output = '';
  foreach (element_children($element) as $child) {
    $items = array();
    foreach (element_children($element[$child]) as $field) {
      $delta = $element[$child][$field]['field']['items'][1]['#item']['#delta'];
      $items[$field] = $element[$child][$field]['field']['items'][1]['#item'];
      $items[$field]['view'] = theme($element[$child][$field]['field']['items'][1]['#theme'], $element[$child][$field]['field']['items'][1]);
      unset($items[$field]['#delta']);
    }
    $items['#delta'] = $delta;

    $element[$child] = array(
      '#node' => $element['#node'],
      '#item' => $items,
      '#formatter' => $element['#formatter'],
    );

    $output .= theme('custom_formatters_formatter', $element[$child]);
  }
  return $output;
}

function content_multigroup_content_generate($node, $field) {
  $item = array();
  foreach (fieldgroup_groups($node['type']) as $group) {
    if ($group['group_type'] == 'multigroup') {
      foreach ($group['fields'] as $field) {
        if (function_exists($function = "{$field['widget_module']}_content_generate")) {
          $item[$field['field_name']] = $function($node, content_fields($field['field_name']));

          $element = array(
            '#node' => $node,
            '#item' => $item[$field['field_name']],
          );
          $item[$field['field_name']]['view'] = theme("{$field['widget_module']}_formatter_default", $element);
        }
      }
    }
  }
  return $item;
}

/**
 * Implements hook_token_list().
 */
function content_multigroup_token_list($type = 'all') {
  $tokens = array('content_multigroup' => array());

  foreach (fieldgroup_groups() as $groups) {
    foreach ($groups as $group) {
      if ($group['group_type'] == 'multigroup') {
        foreach ($group['fields'] as $field) {
          if (function_exists($function = "{$field['widget_module']}_token_list")) {
            foreach ($function('field') as $token) {
              foreach ($token as $key => $value) {
                $tokens['content_multigroup']["content_multigroup-{$field['field_name']}-{$key}"] = $value;
              }
            }
          }
        }
      }
    }
  }

  return $tokens;
}

/**
 * Implements hook_token_values().
 */
function content_multigroup_token_values($type, $object = array()) {
  $values = array();

  if ($type == 'field') {
    foreach (fieldgroup_groups() as $groups) {
      foreach ($groups as $group) {
        if ($group['group_type'] == 'multigroup') {
          foreach ($group['fields'] as $field) {
            if (isset($object[0][$field['field_name']]) && function_exists($function = "{$field['widget_module']}_token_values")) {
              $tokens = $function($type, array($object[0][$field['field_name']]));
              foreach ($tokens as $key => $value) {
                $values["content_multigroup-{$field['field_name']}-{$key}"] = $value;
              }
            }
          }
        }
      }
    }
  }

  return $values;
}
