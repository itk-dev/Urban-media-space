<?php

/*
 * @file
 * Administration UI (form) to handle 3D viewer settings.
 */

/**
 * Implementation of 3D viewer administration form.
 */
function viewer3d_points_form(&$form_state) {
  $form = array();  
  $form['#tree'] = TRUE;

  $file = variable_get('viewer3d_points_file', '');

  $form['viewer3d_points_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Points file'),
    '#description' => t('The location of points.xml inside files or a URL (save to reload points from the file).'),
    '#required' => TRUE,
    '#default_value' => $file,
  );
  
  if (!empty($file)) {
    // Load points information form the file.
    $xml = simplexml_load_file($file);
    if (!xml) {
      drupal_set_message(t('The points file was not found!'), 'error');
      return system_settings_form($form);
    }

    // Load points from XML.
    $points = _viewer3d_load_points($xml);

    // Load default values.
    $default = variable_get('viewer3d_points', array());
    if (!empty($default)) {
      // Order point based on weight.
      viewer3d_points_weight_sort($default);
      // Loop the default points and unset them in the XML points. Then loop
      // over the remaining points, if any, they will be new points.
      foreach ($default as $point) {
        // Get point name from XML points and only show points avalible in XML.
        if ($points[$point['pid']]) {
          $point['name'] = $points[$point['pid']]['name'];
          $form['viewer3d_points'][$point['pid']] = _viewer3d_points_ordering_field($point);
        }        
        unset($points[$point['pid']]);
      }
    }

    // Output remaining points or all points if non have been unset above.
    foreach ($points as $point) {
      if (isset($form['viewer3d_points'][$point['pid']])) {
        echo '<br />Point id is allready used: ',$point['pid'];
      }
      $form['viewer3d_points'][$point['pid']] = _viewer3d_points_ordering_field($point);
    }
  }

  // Give the form some system settings options.
  $form = system_settings_form($form);
  $form['#theme'] = 'viewer3d_points_form';
  
  return $form;
}

/**
 * Load points from xml string.
 *
 * @param SimpleXMLelement $xml
 * @return array $points
 */
function _viewer3d_load_points($xml) {
  $points = array();
  foreach ($xml->Point as $point) {
    $pid = (Int)$point->attributes()->Id;
    $points[$pid] = array(
      'pid' => $pid,
      'name' => (String)$point->attributes()->Name,
      'weight' => 50,
    );
  }
  return $points;
}

/**
 * Build the fields for a single record on the drag-and-drop form.
 */
function _viewer3d_points_ordering_field($point) {
  $field = array();

  // Create label in a evil way, this is to make them sortable by label elsewhere.
  $label = empty($point['name']) ? 'Point ' . ($point['pid'] < 10 ? '0' . $point['pid'] : $point['pid']) : $point['name'];

  $field['pid'] = array(
    '#type' => 'hidden',
    '#value' => $point['pid'],
  );

  $field['name'] = array(
    '#type' => 'hidden',
    '#value' => $label,
  );

  $field['label'] = array(
    '#value' => $label,
  );

  $field['jump_type'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Jump to point',
    '#options' => array('prev' => 'Previous', 'next' => 'Next'),
    '#description' => '',
    '#default_value' => !empty($point['jump_type']) ? $point['jump_type'] : array(),
  );

  $field['weight'] = array(
    '#type' => 'weight',
    '#delta' => 50,
    '#default_value' => (is_null($point['weight'])) ? 0 : $point['weight'],
  );

  return $field;
}

/**
 * Theme the drag-and-drop overview form.
 *
 * Arranges records in a table, and adds the css and js for draggable sorting.
 *
 * @ingroup themeable
 * @ingroup forms
 */
function theme_viewer3d_points_form($form) {
  // Each record has a 'weight' that can be used to arrange it in relation to
  // other records. Drupal's tabledrag.js library allows users to control these
  // weights by dragging and dropping the records in a list -- we just need to
  // add identifying CSS classes to key elements in the table.

  // Start with the file field.
  $output = drupal_render($form['viewer3d_points_file']);
  
  $rows = array();
  foreach (element_children($form['viewer3d_points']) as $key) {
    $row = array();

    // Render the hidden 'node id' field and the title of the record into the
    // same column of the row.
    $row[] = drupal_render($form['viewer3d_points'][$key]['name']) . drupal_render($form['viewer3d_points'][$key]['pid']) . drupal_render($form['viewer3d_points'][$key]['label']);
    $row[] = drupal_render($form['viewer3d_points'][$key]['jump_type']);

    // Add an identifying CSS class to our weight field, as it's the one
    // the tabledrag.js will be controlling. This can be anything we want it to
    // be, we'll just tell the tabledrag.js library what it should look for.
    $form['viewer3d_points'][$key]['weight']['#attributes']['class'] = 'weight';
    $row[] = drupal_render($form['viewer3d_points'][$key]['weight']);

    // Add the new row to our collection of rows, and give it the 'draggable'
    // class, indicating that it should be... well, draggable.
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable',
    );
  }

  // If there were no records found, note the fact so users don't get confused
  // by a completely empty table.
  if (count($rows) == 0) {
    $rows[] = array(t('No records have been added.'), '<span class="scaffolding-example-weight"></span>', '');
  }

  // Render a list of header titles, and our array of rows, into a table. Even
  // we've already rendered all of our records, we always call drupal_render()
  // on the form itself after we're done, so hidden security fields and other
  // elements (like buttons) will appear properly at the bottom of the form.
  $header = array(t('Name'), t('Goto'), t('Weight'));
  $output .= theme('table', $header, $rows, array('id' => 'viewer3d-points-table'));
  $output .= drupal_render($form);

  // Now that we've built our output, tell Drupal to add the tabledrag.js library.
  // We'll pass in the ID of the table, the behavior we want it to use, and the
  // class that appears on each 'weight' form element it should be controlling.
  drupal_add_tabledrag('viewer3d-points-table', 'order', 'self', 'weight');

  return $output;
}