<?php

define('VIEWER3D_TYPE_TITLE', 'title');
define('VIEWER3D_TYPE_INFO', 'info');

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

function viewer3d_load_information($pid) {
  $points = variable_get('viewer3d_points_used', array());  
  if (isset($points[$pid])) {
    return node_view(node_load($points[$pid]));
  }
  else {
    return '<p>' . t('No information about this point was found.') . '<p>';
  }
}