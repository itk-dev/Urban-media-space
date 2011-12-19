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
function viewer3d_ajax($type, $pid, $html = TRUE) {
  switch ($type) {
    case VIEWER3D_TYPE_TITLE:
      $data = array(
        'type' => VIEWER3D_TYPE_TITLE,
        'value' => viewer3d_get_point_title($pid, $html),
      );
      break;

    case VIEWER3D_TYPE_INFO:
      $data = array(
        'type' => VIEWER3D_TYPE_INFO,
        'value' => viewer3d_load_information($pid),
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

function viewer3d_download($filename = FALSE) {
  $settings = variable_get('viewer3d', array());

  // Image path.
  $path = file_directory_path() . '/3dviewer_user_generated';

  // If this is not a download, generate image.
  if (!$filename) {
    if (isset($_POST['data'])) {
      $img = imagecreatefromstring(base64_decode($_POST['data']));
      if ($img != false) {
        // Create file path.
        $filename = '/3dmodel_' . base_convert(mt_rand(0x1D39D3E06400000, 0x41C21CB8E0FFFFFF), 10, 36)  . '.png';
        file_check_directory($path, FILE_CREATE_DIRECTORY);

        // Save image.
        imagepng($img, $path . '/' . $filename);

        // Return file name.
        echo $settings['path'] . '/download' . $filename;
      }
    }
    return;
  }

  // Create filename.
  $filename = $path . '/' . basename($filename);

  // Download file.
  if (strstr($_SERVER["HTTP_USER_AGENT"],"MSIE")) {
    header('X-Download-Options: noopen '); // For IE8
    header('X-Content-Type-Options: nosniff'); // For IE8
  }
  header("Content-type: image/png");
  header("Content-Length: ".filesize($filename));
  header('Content-Disposition: attachment;filename="'.basename($filename).'"');
//  header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
  header('Pragma: public');

  readfile($filename);
}
