<?php
// $Id$

/**
 * @file
 * Context Respect reaction plugin file.
 */

/**
 * Expose permissions as context reactions.
 */
class context_respect_reaction_block extends context_reaction_block {
  /**
  * Override of block_list().
  * An alternative version of block_list() that provides any context enabled blocks.
  */
  function block_list($region) {
    static $build_queue;
    static $build_contexts;
    static $context_blocks = array();
    static $empty_blocks = array();
    global $user;
    $rids = array_keys($user->roles);

    // Static cache a region => block array of blocks that should be built *if*
    // the given region is requested. Note that the blocks are not themselves
    // built in this first run as not all regions may be requested even if
    // active contexts include blocks in those regions.
    $contexts = context_active_contexts();
    if (!isset($build_queue) || $build_contexts != array_keys($contexts)) {
      $info = $this->get_blocks();
      $build_queue = array();
      $build_contexts = array_keys($contexts);
      foreach ($contexts as $context) {
        $options = $this->fetch_from_context($context);
        if (!empty($options['blocks'])) {
          foreach ($options['blocks'] as $block) {
            $block = (object) $block;
            $block->context = $context->name;
            $block->bid = "{$block->module}-{$block->delta}";
            $block->cache = isset($info[$block->bid]->cache) ? $info[$block->bid]->cache : BLOCK_NO_CACHE;
            $page_match = TRUE;
            $role_match = TRUE;

            // check role access
            if (!drupal_match_path($_GET['q'], 'admin/build/context/*')) {
              $block_rids = db_query("SELECT rid FROM {blocks_roles} WHERE module = '%s' AND delta = '%s'", $block->module, $block->delta);
              while ($block_rid = db_result($block_rids)) {
                if (in_array($block_rid, $rids)) {
                  // found a matching role
                  $role_match = TRUE;
                  break;
                }
                else {
                  // didn't match the role
                  $role_match = FALSE;
                }
              }
            }

            // check page access
            $block_data = db_fetch_object(db_query("SELECT pages, visibility FROM {blocks} WHERE module = '%s' AND delta = '%s'", $block->module, $block->delta));
            if ($block_data->pages && !drupal_match_path($_GET['q'], 'admin/build/context/*')) {
              if ($block_data->visibility < 2) {
                $path = drupal_get_path_alias($_GET['q']);
                // Compare with the internal and path alias (if any).
                $page_match = drupal_match_path($path, $block_data->pages);
                if ($path != $_GET['q']) {
                  $page_match = $page_match || drupal_match_path($_GET['q'], $block_data->pages);
                }
                // When $block->visibility has a value of 0, the block is displayed on
                // all pages except those listed in $block->pages. When set to 1, it
                // is displayed only on those pages listed in $block->pages.
                $page_match = !($block_data->visibility xor $page_match);
              }
              else {
                $page_match = drupal_eval($block_data->pages);
              }
            }

            // add to region
            if ($page_match && $role_match) {
              $build_queue[$block->region][] = $block;
            }
          }
        }
      }
    }

    // Context blocks.
    if (!isset($context_blocks[$region])) {
      $context_blocks[$region] = array();
      $empty_blocks[$region] = array();
      if (!empty($build_queue[$region])) {
        foreach ($build_queue[$region] as $block) {
          $block = $this->build_block($block);
          if (!empty($block->content)) {
            $context_blocks[$region][] = $block;
          }
          else {
            $empty_blocks[$region][] = $block;
          }
        }
      }
    }

    // Get core blocks only if enabled.
    $blocks = !variable_get('context_reaction_block_disable_core', FALSE) ? block_list($region) : array();
    $blocks = array_merge($blocks, $context_blocks[$region]);

    // Only include empty blocks if all regions should be shown or there are
    // non-empty blocks in the same region.
    $all_regions = variable_get('context_reaction_block_all_regions', FALSE);
    if ($this->is_editable() && ($all_regions || !empty($blocks))) {
      $blocks = array_merge($blocks, $empty_blocks[$region]);
    }

    // Sort everything.
    uasort($blocks, array('context_respect_reaction_block', 'block_sort'));
    return $blocks;
  }

  /**
   * Override of get_blocks().
   * Helper function to generate a list of blocks from a specified region. If provided a context object,
   * will generate a full list of blocks for that region distinguishing between system blocks and
   * context-provided blocks.
   *
   * @param $region
   *   The string identifier for a theme region. e.g. "left"
   * @param $context
   *   A context object.
   *
   * @return
   *   A keyed (by "module_delta" convention) array of blocks.
   */
  function get_blocks($region = NULL, $context = NULL, $reset = FALSE) {
    static $block_info;
    global $user;
    $rids = array_keys($user->roles);

    if (!isset($block_info) || $reset) {
      $block_info = array();
      if (!$reset) {
        $block_info = context_cache_get('block_info');
      }
      if (empty($block_info)) {
        $block_info = array();
        foreach (module_implements('block') as $module) {
          $module_blocks = module_invoke($module, 'block', 'list');
          if (!empty($module_blocks)) {
            foreach ($module_blocks as $delta => $block) {
              $block = (object) $block;
              $block->module = $module;
              $block->delta = $delta;
              $block->bid = "{$block->module}-{$block->delta}";

              $page_match = TRUE;
              $role_match = TRUE;

              // check role access
              if (!drupal_match_path($_GET['q'], 'admin/build/context/*')) {
                $block_rids = db_query("SELECT rid FROM {blocks_roles} WHERE module = '%s' AND delta = '%s'", $block->module, $block->delta);
                while ($block_rid = db_result($block_rids)) {
                  if (in_array($block_rid, $rids)) {
                    // found a matching role
                    $role_match = TRUE;
                    break;
                  }
                  else {
                    // didn't match the role
                    $role_match = FALSE;
                  }
                }
              }

              // check page access
              $block_data = db_fetch_object(db_query("SELECT pages, visibility FROM {blocks} WHERE module = '%s' AND delta = '%s'", $block->module, $block->delta));
              if ($block_data->pages && !drupal_match_path($_GET['q'], 'admin/build/context/*')) {
                if ($block_data->visibility < 2) {
                  $path = drupal_get_path_alias($_GET['q']);
                  // Compare with the internal and path alias (if any).
                  $page_match = drupal_match_path($path, $block_data->pages);
                  if ($path != $_GET['q']) {
                    $page_match = $page_match || drupal_match_path($_GET['q'], $block_data->pages);
                  }
                  // When $block->visibility has a value of 0, the block is displayed on
                  // all pages except those listed in $block->pages. When set to 1, it
                  // is displayed only on those pages listed in $block->pages.
                  $page_match = !($block_data->visibility xor $page_match);
                }
                else {
                  $page_match = drupal_eval($block_data->pages);
                }
              }

              // add to region
              if ($page_match && $role_match) {
                $block_info[$block->bid] = $block;
              }
            }
          }
        }
        context_cache_set('block_info', $block_info);
      }
      // Allow other modules that may declare blocks dynamically to alter
      // this list.
      drupal_alter('context_block_info', $block_info);

      // Gather only region info from the database.
      $theme_key = variable_get('theme_default', 'garland');
      $result = db_query("SELECT module,weight,delta,region,status FROM {blocks} WHERE theme = '%s' ORDER BY weight ASC", $theme_key);
      while ($row = db_fetch_object($result)) {
        if ($row->status && isset($block_info["{$row->module}-{$row->delta}"])) {
          $block_info["{$row->module}-{$row->delta}"]->weight = $row->weight;
          $block_info["{$row->module}-{$row->delta}"]->region = $row->region;
        }
      }
    }

    $blocks = array();

    // No region specified, provide all blocks.
    if (!isset($region)) {
      $blocks = $block_info;
    }
    // Region specified.
    else {
      foreach ($block_info as $bid => $block) {
        if (isset($block->region) && $block->region == $region) {
          $blocks[$bid] = $block;
        }
      }
    }

    // Add context blocks if provided.
    if (is_object($context) && $options = $this->fetch_from_context($context)) {
      if (!empty($options['blocks'])) {
        foreach ($options['blocks'] as $block) {
          if (
            isset($block_info["{$block['module']}-{$block['delta']}"]) && // Block is valid.
            (!isset($region) || (!empty($region) && $block['region'] == $region)) // No region specified or regions match.
          ) {
            
              global $language;
              echo 'kristian kaas hook: ' . $language;
              
              
            $context_block = $block_info["{$block['module']}-{$block['delta']}"];
            $context_block->weight = $block['weight'];
            $context_block->region = $block['region'];
            $context_block->context = $context->name;
            $blocks[$context_block->bid] = $context_block;
          }
        }
      }
      uasort($blocks, array('context_respect_reaction_block', 'block_sort'));
    }
    return $blocks;
  }
}