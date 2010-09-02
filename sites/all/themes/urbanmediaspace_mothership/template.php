<?php

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