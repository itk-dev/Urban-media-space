<?php

// $Id: clicktale.helpers.inc,v 1.1.2.5 2011/01/20 08:11:33 cableman0408 Exp $

/**
 * @file
 * This file contains helper functions to handel the clicktale PHP integration
 * library.
 */

define('CLICKTALE_PATH', '/clicktale');
define('CLICKTALE_PATH_CACHE', '/Cache');
define('CLICKTALE_PATH_LOGS', '/Logs');

/**
 * Tries to locate the PHP integration library installation path. If the library
 * is not located false is returned and the msg parameter is set.
 *
 * @param string $msg
 * @return string $library_path
 */
function clicktale_get_library_path(&$msg = '') {
  $library_path = variable_get('clicktale_library_path', FALSE);

  // Set default message (error)
  $msg = t('The PHP Integration library was not found. Please download the library from http://www.clicktale.com/integration/ClickTalePHPIntegrationModule_latest.zip and install it in the libraries folder (e.g site/all/libraries/clicktale).<br /><br />Reload this page, when you have installed it.');

  if (!$library_path) {
    if (module_exists('libraries')) {
      $libraries = libraries_get_libraries();
      if (isset($libraries['clicktale'])) {
        $library_path = $libraries['clicktale'];
        variable_set('clicktale_library_path', $library_path);
      }
      else {
        return FALSE;
      }
    }
  }

  // Check that the library is stille there, and not only in the cache
  if (!is_dir($library_path)) {
    variable_set('clicktale_library_path', FALSE);
    return FALSE;
  }

  $msg = t('The PHP integration library was found in "%path"', array('%path' => $library_path));
  return $library_path;
}

/**
 * Try to detected if the PHP integration library's config.php file is writable.
 *
 * @param string $config
 * @return boolean
 */
function clicktale_is_config_writable($config) {
  if (file_exists($config)) {
    if (@touch($config)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Creates the paths need to store PHP integration library cache and logs
 * folders in the files folder. If the folder dos not exists it is create
 * and TRUE is returned else false.
 *
 * @return bool
 */
function clicktale_create_paths() {
  // Build the paths
  $path = clicktale_get_path();
  $cache_path = clicktale_get_cache_path();
  $logs_path = clicktale_get_logs_path();

  // Create clicktale folder in files and subfolders
  if (file_check_directory($path, FILE_CREATE_DIRECTORY)) {
    if (file_check_directory($cache_path, FILE_CREATE_DIRECTORY)) {
      if (file_check_directory($logs_path, FILE_CREATE_DIRECTORY)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 * Build the clicktale path.
 *
 * @return string $path
 */
function clicktale_get_path() {
  return file_directory_path() . CLICKTALE_PATH;
}

/**
 * Build the clicktale cache path.
 *
 * @return string $path
 */

function clicktale_get_cache_path() {
  return clicktale_get_path() . CLICKTALE_PATH_CACHE;
}

/**
 * Build the clicktale logs path.
 *
 * @return string $path
 */

function clicktale_get_logs_path() {
  return clicktale_get_path() . CLICKTALE_PATH_LOGS;
}

/**
 * Updates the clicktale PHP integration library configuration file, which is
 * a PHP file containing a PHP array.
 * 
 * The array is appened by adding an extra configuration array to the end of
 * the file config.php, which overrides the values in the array.
 *
 * @param array $options (path, method, delete, size, ttl)
 * @return <type>
 */
function clicktale_configuration_update_php(array $options) {
  // Get the library path
  $lib_path = clicktale_get_library_path($msg);
  if (!$lib_path) {
    drupal_set_message($msg, 'error');
    return;
  }

  // Load config file and change values in it
  if (file_exists($lib_path . '/config.php')) {
    $config = file_get_contents($lib_path . '/config.php');

    /*
     *  This is not pretty, but we are overriding the php array values by adding
     *  them to the end of the PHP integration module config.php file.
     */
    $start_tag = "// APPENDED BY DRUPAL CLICKTALE MODULE\n";
    $start_of_config = strpos($config, $start_tag);
    if ($start_of_config) {
      // Clicktale module configuration found, so remove it first
      $config = substr($config, 0, $start_of_config);
    }
    else {
      // Clean clicktale config, so remove php end tag, it's done this way
      // because the recorded videos only contained php error (sess_start).
      $config = substr($config, 0, strrpos($config, '?>'));
    }

    // Build the new config string (file)
    $config .= $start_tag;

    // Enforced options
    $config .= clicktale_configuration_value('ScriptsFile', realpath(clicktale_get_path()) . '/ClickTaleScripts.xml', 'string');
    $config .= clicktale_configuration_value('CacheFetchingUrl', 'http://%ClickTaleCacheUrl%/ClickTaleCache.php?t=%CacheToken%', 'string');
    $config .= clicktale_configuration_value('LogPathMask', realpath(clicktale_get_logs_path()) . '/Log_{0}.txt', 'string');
    $config .= clicktale_configuration_value('CacheLocation', realpath(clicktale_get_cache_path()), 'string');

    // Admin supplyed
    $config .= clicktale_configuration_value('CacheProvider', $options['method'], 'string');
    $config .= clicktale_configuration_value('MaxFolderSize', $options['size'], 'integer');
    $config .= clicktale_configuration_value('DeleteAfterPull', ($options['delete'] == 'yes' ? TRUE : FALSE), 'boolean');
    $config .= clicktale_configuration_value('MaxCachedSeconds', $options['ttl'], 'integer');

    // End config tag
    $config .= "\n?>";

    // As the file is outside "files" we have to do it the old way
    $file = realpath($lib_path . '/config.php');
    if (!$fp = fopen($file, 'wb')) {
      drupal_set_message(t('The file could not be created: %file.', array('%file' => $file)), 'error');
      return 0;
    }
    else {
      drupal_set_message(t('PHP integration config file written.'));
    }
    fwrite($fp, $config);
    fclose($fp);
  }
}

/**
 * Helper function to create the string written to config.php in
 * clicktale_configuration_update_php()
 *
 * @param string $name
 * @param string $value
 * @param string $type
 * @return string
 */
function clicktale_configuration_value($name, $value, $type) {

  switch ($type) {
    case 'integer':
      $str = '$config[\'' . $name . '\'] = ' . $value . ';' . "\n";
      break;

    case 'string':
      $str = '$config[\'' . $name . '\'] = \'' . $value . '\';' . "\n";
      break;

    case 'boolean':
      if ($value) {
        $value = 'true';
      }
      else {
        $value = 'false';
      }
      $str = '$config[\'' . $name . '\'] = ' . $value . ';' . "\n";
      break;
  }

  return $str;
}

function clicktale_tracking_configuration() {
  global $user;

  // Check that a configuration exists
  $config = variable_get('clicktale', NULL);
  if ($config && isset($config['top']) && isset($config['bottom'])) {
    $track = TRUE;
  }
  else {
    return FALSE;
  }

  // Check that the we are not in the admin section or is user 1
  if (arg(0) != 'admin' || $user->uid != 1) {
    $track = TRUE;
  }
  else {
    return FALSE;
  }

  return TRUE;
}