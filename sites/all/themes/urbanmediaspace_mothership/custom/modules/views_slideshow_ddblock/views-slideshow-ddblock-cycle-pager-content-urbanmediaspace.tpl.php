<?php
// $Id$ 

/*!
 * Dynamic display block module template: vsd-upright-40p - pager template
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.0 (19-AUG-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

/**
 * @file
 * Dynamic display block module template: urbanmediaspace - pager template
 * - Custom pager with images
 *
 * Available variables:
 * - $views_slideshow_ddblock_pager_settings['delta']: Block number of the block.
 * - $views_slideshow_ddblock_pager_settings['pager']: Type of pager to add
 * - $views_slideshow_ddblock_pager_settings['pager2']: Add prev/next pager
 * - $views_slideshow_ddblock_pager_settings['pager_position']: position of the slider (top | bottom) 
 * - $views_slideshow_ddblock_pager_items: array with pager_items
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */
 $settings = $views_slideshow_ddblock_pager_settings;
 
 $number_of_items = 4;        // total number of items to show
 $number_of_items_per_row=6;  // number of items to show in a row
?>

<div id="views-slideshow-ddblock-custom-pager-<?php print $settings['delta'] ?>" class="<?php print $settings['pager'] ?> clear-block border">
  <?php if ($views_slideshow_ddblock_pager_items): ?>
   <?php $item_counter=1; ?>
   <?php foreach ($views_slideshow_ddblock_pager_items as $pager_item): ?>
    <div class="<?php print $settings['pager'] ?>-item <?php print $settings['pager'] ?>-item-<?php print $item_counter ?> <?php if ($item_counter == $number_of_items) print ' last' ?>">
     <div class="<?php print $settings['pager'] ?>-item-inner">
       <a href="<?php print drupal_get_path_alias($pager_item['pager_link'], isset($language) ? $language : ''); ?>" title="<?php print $pager_item['pager_teaser_title']; ?>" class="pager-link"><?php print $pager_item['pager_image']; ?>
        <span class="<?php print $settings['pager'] ?>-item-inner-teaser-wrapper">
          <b class="<?php print $settings['pager'] ?>-item-inner-teaser-title"><?php print $pager_item['pager_teaser_title']; ?></b>
          <span class="<?php print $settings['pager'] ?>-item-inner-teaser"><?php print $pager_item['pager_teaser']; ?></span>
        </span>
      </a>
     </div>
    </div> <!-- /custom-pager-item -->
    <?php $item_counter++ ?>
   <?php endforeach; ?>
  <?php endif; ?>
</div>  <!-- /pager-->
<?php if ($settings['pager2'] == 1 && $settings['pager2_position']['slide'] === 'slide'): ?>
 <div class="views-slideshow-ddblock-prev-next-slide">
  <div class="prev-container">
   <a class="prev" href="#"><?php print $settings['pager2_slide_prev']?></a>
  </div>
  <div class="next-container">
   <a class="next" href="#"><?php print $settings['pager2_slide_next'] ?></a>
  </div>
 </div>
<?php endif; ?>