<?php
// $Id: 

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function urbanmediaspace_breadcrumb($breadcrumb) {

  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('zen_breadcrumb');
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('zen_breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $breadcrumb_separator = theme_get_setting('zen_breadcrumb_separator');
      $trailing_separator = $title = '';
      if (theme_get_setting('zen_breadcrumb_title')) {
        if ($title = drupal_get_title()) {
          $trailing_separator = $breadcrumb_separator;
        }
      }
      elseif (theme_get_setting('zen_breadcrumb_trailing')) {
        $trailing_separator = $breadcrumb_separator;
      }
      return '<div class="breadcrumb">' . implode($breadcrumb_separator, $breadcrumb) . "$trailing_separator<span>$title</span></div>";
    }
  }
  // Otherwise, return an empty string.
  return '';
}

function urbanmediaspace_preprocess_page(&$vars, $hook) {
  // If the user is silly and enables Zen as the theme, add some styles.
  if ($GLOBALS['theme'] == 'zen') {
    include_once './' . _zen_path() . '/zen-internals/template.zen.inc';
    _zen_preprocess_page($vars, $hook);
  }
  // Add conditional stylesheets.
  elseif (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  // Remove the mostly useless page-ARG0 class.
  if ($index = array_search(preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-'. drupal_strtolower(arg(0))), $vars['classes_array'])) {
    unset($vars['classes_array'][$index]);
  }
  if (!$vars['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    $vars['classes_array'][] = drupal_html_class('page-' . $path);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $vars['classes_array'][] = drupal_html_class('section-' . $section);
  }
  if (theme_get_setting('zen_wireframes')) {
    $vars['classes_array'][] = 'with-wireframes'; // Optionally add the wireframes style.
  }
  // We need to re-do the $layout and body classes because
  // template_preprocess_page() assumes sidebars are named 'left' and 'right'.
  $vars['layout'] = 'none';
  if (!empty($vars['sidebar_first'])) {
    $vars['layout'] = 'first';
  }
  if (!empty($vars['sidebar_second'])) {
    $vars['layout'] = ($vars['layout'] == 'first') ? 'both' : 'second';
  }
  // If the layout is 'none', then template_preprocess_page() will already have
  // set a 'no-sidebars' class since it won't find a 'left' or 'right' sidebar.
  if ($vars['layout'] != 'none') {
    // Remove the incorrect 'no-sidebars' class.
    if ($index = array_search('no-sidebars', $vars['classes_array'])) {
      unset($vars['classes_array'][$index]);
    }
    // Set the proper layout body classes.
    if ($vars['layout'] == 'both') {
      $vars['classes_array'][] = 'two-sidebars';
    }
    else {
      $vars['classes_array'][] = 'one-sidebar';
      $vars['classes_array'][] = 'sidebar-' . $vars['layout'];
    }
  }
  // Store the menu item since it has some useful information.
  $vars['menu_item'] = menu_get_item();
  switch ($vars['menu_item']['page_callback']) {
    case 'views_page':
      // Is this a Views page?
      $vars['classes_array'][] = 'page-views';
      break;
    case 'page_manager_page_execute':
      // Is this a Panels page?
      $vars['classes_array'][] = 'page-panels';
      break;
  }
  
  $vars['classes'] = implode(' ', $vars['classes_array']);

}


/* testing for compatibility with context */
function urbanmediaspace_blocks($region, $show_blocks = NULL) {

    if (module_exists("context")){

      // Since Drupal 6 doesn't pass $show_blocks to theme_blocks, we manually call
      // theme('blocks', NULL, $show_blocks) so that this function can remember the
      // value on later calls.
      static $render_sidebars = TRUE;
      if (!is_null($show_blocks)) {
        $render_sidebars = $show_blocks;
      }

      // Bail if this region is disabled.
      //$disabled_regions = context_active_values('theme_regiontoggle');
      //if (!empty($disabled_regions) && in_array($region, $disabled_regions)) {
        //return '';
      //}

      // If zen_blocks was called with a NULL region, its likely we were just
      // setting the $render_sidebars static variable.
      if ($region) {
        $output = '';

        $plugin = context_get_plugin('reaction', 'block');

        // Add any content assigned to this region through drupal_set_content() calls.
        $output .= $plugin->execute($region);

        $elements['#children'] = $output;
        $elements['#region'] = $region;

        return $output ? theme('region', $elements) : '';
      }
    }
    else {
        return zen_blocks($region, $show_blocks);
    }
}

function urbanmediaspace_preprocess_node(&$vars) {
  if (!drupal_is_front_page()) {
    $vars['right']          = theme('blocks', 'right');
    $vars['content_bottom'] = theme('blocks', 'content_bottom');
  }
}