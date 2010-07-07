<?php // $Id: nodepicker-taxonomy.tpl.php,v 1.1.2.2 2010/04/09 09:55:41 blixxxa Exp $ ?>
<div class="page" id="page-taxonomy">
  <div class="header">
    <h2><?php print t('Taxonomy') ?></h2>
    <?php if($menu_button) print $menu_button; ?>
    <?php if($nodes_button) print $nodes_button; ?>
    <a href="#" class="button" id="filter-button"><?php print t('Filters') ?></a>
  </div>
  <div id="browse">
    <div class="browse">
      <?php print $content; ?>
    </div>
  </div>
</div>