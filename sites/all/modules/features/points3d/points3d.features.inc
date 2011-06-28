<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function points3d_imagecache_default_presets() {
  $items = array(
    'point3d_image' => array(
      'presetname' => 'point3d_image',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '250',
            'height' => '250',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function points3d_node_info() {
  $items = array(
    'point3d' => array(
      'name' => t('3D point'),
      'module' => 'features',
      'description' => t('3D point in the model of the building. This content type can contain extended information about the building at a given point. '),
      'has_title' => '1',
      'title_label' => t('Titel'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
