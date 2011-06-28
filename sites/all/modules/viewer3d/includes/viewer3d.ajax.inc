<?php

/**
 * @file
 * Implementation of ajax callbacks from the flash 3D viewer.
 */

define('VIEWER3D_TYPE_TITLE', 'title');
define('VIEWER3D_TYPE_INFO', 'info');

/**
 * Dispatch the call to the right function based on the $type parameter.
 *
 * @param string $type the type of data to fetch.
 * @param int $pid point id.
 */
function viewer3d_ajax($type, $pid) {
  switch ($type) {
    case VIEWER3D_TYPE_TITLE:
      $data = array(
        'type' => VIEWER3D_TYPE_TITLE,
        'html' => viewer3d_get_point_title($pid),
      );
      break;

    case VIEWER3D_TYPE_INFO:
      $data = array(
        'type' => VIEWER3D_TYPE_INFO,
        'html' => viewer3d_load_information($pid),
      );

    default:
      break;
  }

  // Return json.
  drupal_json($data);
}

/**
 * Returns rendered node content based on point id.
 *
 * @param int $pid
 * @return html
 */
function viewer3d_load_information($pid) {
  $points = variable_get('viewer3d_points_used', array());  
  if (isset($points[$pid])) {
    return node_view(node_load($points[$pid]));
  }
  else {
    return '<p>' . t('No information found about this point.') . '<p>';
  }
}