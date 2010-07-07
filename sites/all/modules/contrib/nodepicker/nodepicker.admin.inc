<?php
// $Id: nodepicker.admin.inc,v 1.1.2.3 2010/05/24 06:31:27 blixxxa Exp $

/**
 * Module settings page.
 */
function nodepicker_settings() {
  $form = array();
  
  $form['nodepicker_tabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Additional tabs'),
    '#default_value' => variable_get('nodepicker_tabs', array()),
    '#options' => array(
      'taxonomy' => t('Taxonomy'),
      'menu' => t('Menu'),
    ),
    '#description' => t('Select additional tabs you want available in the <em>Node Picker</em> dialog.'),
  );
  
  return system_settings_form($form);
}