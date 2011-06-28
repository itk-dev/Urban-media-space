<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function points3d_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer points
  $permissions['administer points'] = array(
    'name' => 'administer points',
    'roles' => array(
      '0' => 'admin',
      '1' => 'super admin',
    ),
  );

  // Exported permission: administer viewer
  $permissions['administer viewer'] = array(
    'name' => 'administer viewer',
    'roles' => array(
      '0' => 'admin',
      '1' => 'super admin',
    ),
  );

  // Exported permission: create point3d content
  $permissions['create point3d content'] = array(
    'name' => 'create point3d content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'super admin',
    ),
  );

  // Exported permission: delete any point3d content
  $permissions['delete any point3d content'] = array(
    'name' => 'delete any point3d content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'super admin',
    ),
  );

  // Exported permission: delete own point3d content
  $permissions['delete own point3d content'] = array(
    'name' => 'delete own point3d content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'super admin',
    ),
  );

  // Exported permission: edit any point3d content
  $permissions['edit any point3d content'] = array(
    'name' => 'edit any point3d content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'super admin',
    ),
  );

  // Exported permission: edit field_3dpoint_description
  $permissions['edit field_3dpoint_description'] = array(
    'name' => 'edit field_3dpoint_description',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'super admin',
    ),
  );

  // Exported permission: edit field_3dpoint_image
  $permissions['edit field_3dpoint_image'] = array(
    'name' => 'edit field_3dpoint_image',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'super admin',
    ),
  );

  // Exported permission: edit own point3d content
  $permissions['edit own point3d content'] = array(
    'name' => 'edit own point3d content',
    'roles' => array(),
  );

  // Exported permission: view field_3dpoint_description
  $permissions['view field_3dpoint_description'] = array(
    'name' => 'view field_3dpoint_description',
    'roles' => array(
      '0' => 'admin',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'editor',
      '4' => 'super admin',
    ),
  );

  // Exported permission: view field_3dpoint_image
  $permissions['view field_3dpoint_image'] = array(
    'name' => 'view field_3dpoint_image',
    'roles' => array(
      '0' => 'admin',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'editor',
      '4' => 'super admin',
    ),
  );

  // Exported permission: view imagecache point3d_image
  $permissions['view imagecache point3d_image'] = array(
    'name' => 'view imagecache point3d_image',
    'roles' => array(
      '0' => 'admin',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'editor',
      '4' => 'super admin',
    ),
  );

  // Exported permission: view model
  $permissions['view model'] = array(
    'name' => 'view model',
    'roles' => array(
      '0' => 'admin',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'editor',
      '4' => 'super admin',
    ),
  );

  return $permissions;
}
