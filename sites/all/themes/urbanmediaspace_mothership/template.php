<?php

/**
 * Add current page to breadcrumb
 */
function urbanmediaspace_mothership_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    $title = drupal_get_title();
    if (!empty($title)) {
      $breadcrumb[]=$title;
    }
    return '<div class="breadcrumb">'. implode(' > ', $breadcrumb) .'</div>';
  }
}

/**
 * Implements theme_menu_item_link()
 */
function urbanmediaspace_mothership_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
  }

  return l($link['title'], $link['href'], $link['localized_options']);
}

function urbanmediaspace_mothership_preprocess_node(&$vars) {
  if (!drupal_is_front_page()) {
    $vars['left']           = theme('blocks', 'left');
    $vars['right']          = theme('blocks', 'right');
    $vars['content_bottom'] = theme('blocks', 'content_bottom');
  }
}

function urbanmediaspace_mothership_preprocess_views_view(&$vars) {
  $vars['right']          = theme('blocks', 'right');
  $vars['content_bottom'] = theme('blocks', 'content_bottom');
}

// 960 ns function
function ns() {
  $args = func_get_args();
  $default = array_shift($args);
  // Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state = 'var';
  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}