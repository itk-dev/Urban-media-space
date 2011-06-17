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

  $form['viewer3d']['data'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of the data'),
    '#description' => t('The location of the data folder or data URL.'),
    '#required' => TRUE,
    '#default_value' => isset($default['data']) ? $default['data'] : '',
  );

   $form['viewer3d']['overviewURL'] = array(
    '#type' => 'textfield',
    '#title' => t('Overview URL'),
    '#description' => t('URL to get overview images and more.'),
    '#required' => TRUE,
    '#default_value' => isset($default['overviewURL']) ? $default['overviewURL'] : '',
  );

   $form['viewer3d']['currentLocation'] = array(
    '#type' => 'textfield',
    '#title' => t('Current location'),
    '#description' => t('The location that the viewer should start at.'),
    '#maxlenght' => 3,
    '#size' => 3,
    '#required' => TRUE,
    '#default_value' => isset($default['currentLocation']) ? $default['currentLocation'] : '8',
  );

   $form['viewer3d']['angle'] = array(
    '#type' => 'textfield',
    '#title' => t('Angle'),
    '#description' => t('The angle to use at the start location.'),
    '#maxlenght' => 3,
    '#size' => 3,
    '#required' => TRUE,
    '#default_value' => isset($default['angle']) ? $default['angle'] : '4.54',
  );

  $form['viewer3d']['angle2'] = array(
    '#type' => 'textfield',
    '#title' => t('Angle 2'),
    '#description' => t('The angle to use at the start location.'),
    '#maxlenght' => 3,
    '#size' => 3,
    '#required' => TRUE,
    '#default_value' => isset($default['angle2']) ? $default['angle2'] : '6.10',
  );
  
  $form['viewer3d']['showTopBar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Top bar'),
    '#description' => t('Show top bar.'),
    '#default_value' => isset($default['showTopBar']) ? $default['showTopBar'] : 0,
  );

  $form['viewer3d']['showLog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show log'),
    '#description' => t('Show log, mostly used for debuging.'),
    '#default_value' => isset($default['showLog']) ? $default['showLog'] : 0,
  );

  return system_settings_form($form);
}

/**
 * TODO: NEED TO CLEAR MENU CACHE ON PATH CHANGE
 */