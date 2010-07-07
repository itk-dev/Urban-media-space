<?php
// $Id: nodepicker.inc,v 1.1.2.1 2010/05/24 06:14:04 blixxxa Exp $

/**
 * @file
 * Wysiwyg API integration on behalf of Node Picker module.
 */

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function nodepicker_nodepicker_plugin() {
  if(module_exists('jquery_ui')){ jquery_ui_add('ui.dialog'); };
  drupal_add_css(drupal_get_path('module', 'nodepicker') . "/plugins/nodepicker/dialog.css");
  
	$plugins['nodepicker'] = array(
    'title' => t('Node picker'),
    'vendor url' => 'http://drupal.org/project/nodepicker',
    'icon file' => 'nodepicker.png',
    'icon title' => t('Insert links to internal content'),
    'settings' => array(
      'url' => url('nodepicker/nodes'),
    ),
  );
  return $plugins;
}