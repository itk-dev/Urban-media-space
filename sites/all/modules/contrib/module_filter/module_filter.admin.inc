<?php
// $Id: module_filter.admin.inc,v 1.1.2.2 2010/06/29 17:09:30 greenskin Exp $

/**
 * @file
 *
 * @author greenSkin
 */

/*******************************************************************************
 * Callback Functions, Forms, and Tables
 ******************************************************************************/

function module_filter_autocomplete($string) {
  $files = module_rebuild_cache();
  $matches = array();
  $count = 1;
  foreach (module_rebuild_cache() as $id => $module) {
    if ($count > 10) {
      break;
    }

    $name = $module->info['name'];
    if (ereg(strtolower($string), strtolower($name)) && strtolower($string) != strtolower($name)) {
      $matches[$name] = $name;
      $count++;
    }
  }
  print drupal_to_js($matches);
  exit();
}

/**
 * Settings form for module filter.
 */
function module_filter_settings() {
  $form['module_filter_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabs'),
    '#description' => t('Themes module list into tabs.'),
    '#default_value' => variable_get('module_filter_tabs', 1)
  );
  $form['module_filter_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocomplete'),
    '#description' => t('Enable this to provide autocomplete suggestions of existing module names in the module filter textfield. Currently only works if tabs are disabled below.'),
    '#default_value' => variable_get('module_filter_autocomplete', 0)
  );
  return system_settings_form($form);
}
