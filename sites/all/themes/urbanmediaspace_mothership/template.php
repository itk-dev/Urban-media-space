<?php

function urbanmediaspace_mothership_preprocess_page(&$vars, $hook) {
  // Set variables for the logo and site_name.
  if (!empty($vars['logo'])) {
    // Return the site_name even when site_name is disabled in theme settings.
    $vars['logo_alt_text'] = (empty($vars['logo_alt_text']) ? variable_get('site_name', '') : $vars['logo_alt_text']);
    $vars['site_logo'] = '<a id="site-logo" href="'. $vars['front_page'] .'" title="'. $vars['logo_alt_text'] .'" rel="home"><img src="'. $vars['logo'] .'" alt="'. $vars['logo_alt_text'] .'" /></a>';
  }
  // Unset title on node (printet in node tpl's instead because of title/image order)
  if ($vars['node']) {
    unset($vars['title']);
  }
}

function urbanmediaspace_mothership_site_map_box($title, $content, $class = '') {
  $output = '';
  if ($title || $content) {
    $class = $class ? 'site-map-box '. $class : 'site-map-box';
    $output .= '<div class="'. $class .'">';
    if ($content) {
      $output .= '<div class="content">'. $content .'</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

/**
 * Add current page to breadcrumb
 */
function urbanmediaspace_mothership_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
//    // Rewrite "all" link when viewing news view
//    // Danish
//    if ($breadcrumb[1] == '<a href="/nyheder/all">Nyheder</a>') {
//      $breadcrumb[1] = '<a href="/nyheder">Nyheder</a>';
//    }
//    // English
//    if ($breadcrumb[1] == '<a href="/news/all">News</a>') {
//      $breadcrumb[1] = '<a href="/news">News</a>';
//    }
    
    global $language;
    $trail = array();
    urbanmediaspace_mothership_active_trail(menu_tree_page_data('primary-links'), $trail, $language->language);
    $trail= array_reverse($trail, TRUE);
    array_pop($trail);    
    
    foreach ($trail as $key => $value) {
      $breadcrumb[] = l($key, $value);
    }

    $title = drupal_get_title();
    if (!empty($title)) {
      $breadcrumb[]=$title;
    }


    //i18nmenu_translated_tree('primary-links');

    return '<div class="breadcrumb">'. implode(' > ', $breadcrumb) .'</div>';
  }
}

function urbanmediaspace_mothership_active_trail($menu, &$trail, $langcode) {

  foreach ($menu as $key => $item) {
    if ($item['link']['in_active_trail']) {
      if (is_array($item['below'])) {
        urbanmediaspace_mothership_active_trail($item['below'], $trail, $langcode);
      }
      $title = !empty($item['link']['localized_options']['attributes']['title']) ? $item['link']['localized_options']['attributes']['title'] : $item['link']['link_title'];
      $link_path = 'node/' . $item['link']['options']['translations'][$langcode]->nid;
      $trail[$title] = $link_path;
      break;
    }
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