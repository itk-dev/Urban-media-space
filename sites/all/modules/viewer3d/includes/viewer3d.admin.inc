<?php

/*
 * @file
 * Administration UI (form) to handle 3D viewer settings.
 */

/**
 * Implementation of 3D viewer administration form.
 */
function viewer3d_admin_form() {

  $form = array();

  // Load setting array
  $default = variable_get('viewer3d', array());
  
  $form['viewer3d'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#title' => t('General settings'),
    '#tree' => TRUE,
  );

  $form['viewer3d']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to display viewer'),
    '#description' => t('The path that should display the 3D viewer (optional)'),
    '#required' => FALSE,
    '#default_value' => isset($default['path']) ? $default['path'] : '',
  );

  $form['viewer3d']['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of the viewer'),
    '#description' => t('The location of the flash viewer inside the files folder'),
    '#required' => TRUE,
    '#default_value' => isset($default['location']) ? $default['location'] : '',
  );

  return system_settings_form($form);
}

/**
 * TODO: NEED TO CLEAR MENU CACHE ON PATH CHANGE
 */