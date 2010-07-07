<?php
// $Id: nodepicker.page.inc,v 1.1.2.3 2010/05/18 13:43:56 blixxxa Exp $

/**
 * Node Picker UI powered by Views.
 */
function nodepicker_page($page) {
  // Add styles.
  drupal_add_css(drupal_get_path('module', 'nodepicker') .'/theme/nodepicker.css');
  
  // Set styles.
  $styles = drupal_get_css();
  
  // Add javascript.
  drupal_add_js(drupal_get_path('module', 'views') .'/js/base.js');
  drupal_add_js(drupal_get_path('module', 'views') .'/js/ajax_view.js');
  drupal_add_js(drupal_get_path('module', 'nodepicker') .'/js/nodepicker.js');
  
  // Theme content.
  switch($page) {
    case 'menu':
      $content = theme('nodepicker_menu');
    break;
    
    case 'taxonomy':
      $content = theme('nodepicker_taxonomy');
    break;
    
    default:
      $content = theme('nodepicker_nodes');
    break;
  }

  // Set messages.
  $messages = theme('status_messages');
  
  // Set scripts.
  $scripts = drupal_get_js();
  
  // Theme the output.
  print theme('nodepicker_page',
    $styles,
    $messages,
    $content,
    $scripts
  );
  
  exit();
}

/**
 * Return JSON of link properties from supplied tag.
 */
function nodepicker_ajax() {
  // Supress Devel module output.
  $GLOBALS['devel_shutdown'] = FALSE;
  
  if (empty($_GET['tag'])) {
    exit;
  }

  if (!preg_match("/^\[nodepicker\s*([^\]]*?)\]$/", $_GET['tag'])) {
    exit;
  }
  
  $rawtag = substr($_GET['tag'], 13, -1);
  
  $attr = nodepicker_decode_tag($rawtag);

  print json_encode($attr);
  exit;
}

/**
 * Substitute for json_encode function for PHP version < 5.2.
 */
if(!function_exists('json_encode')) {
  function json_encode($a=false) {
    if (is_null($a)) return 'null';
    if ($a === false) return 'false';
    if ($a === true) return 'true';
    if (is_scalar($a)) {
      if (is_float($a)) {
        // Always use "." for floats.
        return floatval(str_replace(",", ".", strval($a)));
      }

      if (is_string($a)) {
        static $jsonReplaces = array(array("\\", "/", "\n", "\t", "\r", "\b", "\f", '"'), array('\\\\', '\\/', '\\n', '\\t', '\\r', '\\b', '\\f', '\"'));
        return '"' . str_replace($jsonReplaces[0], $jsonReplaces[1], $a) . '"';
      }
      else
        return $a;
    }
    $isList = true;
    for ($i = 0, reset($a); $i < count($a); $i++, next($a)) {
      if (key($a) !== $i) {
        $isList = false;
        break;
      }
    }
    $result = array();
    if ($isList) {
      foreach ($a as $v) $result[] = json_encode($v);
      return '[' . join(',', $result) . ']';
    }
    else {
      foreach ($a as $k => $v) $result[] = json_encode($k).':'.json_encode($v);
      return '{' . join(',', $result) . '}';
    }
  }
}