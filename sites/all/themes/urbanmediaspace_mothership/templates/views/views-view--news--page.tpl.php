<?php
// $Id: views-view.tpl.php,v 1.1.2.1 2010/01/12 16:32:29 johnalbin Exp $

/**
 * @file
 * Main view template
 *
 * Variables available:
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - view
 *   - view-[name]
 *   - view-id-[name]
 *   - view-display-id-[display id]
 *   - view-dom-id-[dom id]
 * - $css_name: A css-safe version of the view name.
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
?>

<?php if ($header): ?>
  <div class="view-header">
    <?php print $header; ?>
  </div>
<?php endif; ?>

<?php if ($rows): ?>
  <div class="view-content main-content grid-12">
    <?php print $rows; ?>
    <?php if ($pager): ?>
      <?php print $pager; ?>
    <?php endif; ?>
  </div>
<?php elseif ($empty): ?>
  <div class="view-empty">
    <?php print $empty; ?>
  </div>
<?php endif; ?>

<?php if ($more): ?>
  <?php print $more; ?>
<?php endif; ?>

<?php if ($feed_icon): ?>
  <div class="feed-icon">
    <?php print $feed_icon; ?>
  </div>
<?php endif; ?>

<!-- /.view -->

<?php if ($right): ?>
  <div id="sidebar-right" class="column sidebar region grid-4">
    <div id="sidebar-right-inner">
      <?php print $right; ?>
    </div>
  </div> <!--//end #sidebar-right-inner -->
<?php endif; ?>

<?php if ($content_bottom): ?>
  <div id="content-bottom" class="region region-content_bottom">
    <?php print $content_bottom; ?>
  </div> <!-- /#content-bottom -->
<?php endif; ?>