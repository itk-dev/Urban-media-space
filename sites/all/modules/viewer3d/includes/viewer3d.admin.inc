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
    '#collapsible' => TRUE,
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
    '#title' => t('Horizontal angle'),
    '#description' => t('The horizontal angle used to define the camera direction.'),
    '#maxlenght' => 3,
    '#size' => 3,
    '#required' => TRUE,
    '#default_value' => isset($default['angle']) ? $default['angle'] : '4.54',
  );

  $form['viewer3d']['angle2'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical angle'),
    '#description' => t('The vertical angle used to define the camera direction.'),
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

  $form['viewer3d']['showPrevNext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show prev / next buttons'),
    '#description' => t('Show log, mostly used for debuging.'),
    '#default_value' => isset($default['showPrevNext']) ? $default['showPrevNext'] : 1,
  );

  $form['viewer3d']['infoPoints'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use information points'),
    '#description' => t('Display markers for information points.'),
    '#default_value' => isset($default['infoPoints']) ? $default['infoPoints'] : 0,
  );

  $form['viewer3d']['markerSize'] = array(
    '#type' => 'textfield',
    '#title' => t('Marker size'),
    '#description' => t('The size of the markers.'),
    '#size' => '4',
    '#default_value' => isset($default['markerSize']) ? $default['markerSize'] : 40,
  );

  $form['viewer3d']['markerMinSize'] = array(
    '#type' => 'textfield',
    '#title' => t('Marker min. size'),
    '#description' => t('The markers min. size.'),
    '#size' => '4',
    '#default_value' => isset($default['markerMinSize']) ? $default['markerMinSize'] : 30,
  );

  $form['viewer3d']['rotationSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Rotation speed'),
    '#description' => t('Rotation speed when arriving at a point.'),
    '#size' => '4',
    '#default_value' => isset($default['rotationSpeed']) ? $default['rotationSpeed'] : 1,
  );

  $form['viewer3d']['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Introduction text'),
    '#description' => t('Description placed at the top of the page.'),
    '#default_value' => isset($default['text']) ? $default['text'] : '',
  );

  $default = variable_get('viewer3d_help', array());
  $form['viewer3d_help'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Help text'),
    '#tree' => TRUE,
  );

  $form['viewer3d_help']['header'] = array(
    '#type' => 'textfield',
    '#title' => t('Header'),
    '#description' => t('Help text title.'),
    '#required' => TRUE,
    '#default_value' => isset($default['header']) ? $default['header'] : '',
  );


  $form['viewer3d_help']['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#description' => t('Help text shown when the viewer starts.'),
    '#required' => TRUE,
    '#wysiwyg' => TRUE,
    '#default_value' => isset($default['text']) ? $default['text'] : '',
  );

  // Add wysiwyg editor
  $form['viewer3d_help']['format'] = filter_form(FILTER_FORMAT_DEFAULT);

  return system_settings_form($form);
}

/**
 * TODO: NEED TO CLEAR MENU CACHE ON PATH CHANGE
 */