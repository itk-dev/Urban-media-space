<?php
/**
 * @file
 * Libraries API module Integration.
 */

/**
 * Implements hook_custom_formatters_form_alter().
 */
function libraries_custom_formatters_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'custom_formatters_formatter_form' && isset($form['editor'])) {
    // EditArea library support.
    if (in_array('editarea', array_keys(libraries_get_libraries()))) {
      $form['#after_build'][] = 'custom_formatters_libraries_after_build';

      $form['editor']['code']['#resizable'] = FALSE;
      $form['editor']['code']['#attributes']['style'] = 'width: 100%';
    }
  }
}

function custom_formatters_libraries_after_build(&$form_element) {
  drupal_add_js(libraries_get_path('editarea') . '/edit_area/edit_area_full.js');
  drupal_add_js(drupal_get_path('module', 'custom_formatters') . '/scripts/custom_formatters.admin.js');
  return $form_element;
}
