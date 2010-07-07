<?php // $Id: nodepicker-menu.tpl.php,v 1.1.2.1 2010/03/09 08:43:04 blixxxa Exp $ ?>
<div class="page" id="page-menu">
  <div class="header">
    <h2><?php print t('Menu') ?></h2>
    <?php if($taxonomy_button) print $taxonomy_button; ?>
    <?php if($nodes_button) print $nodes_button; ?>
  </div>
  <div id="browse">
    <div class="browse">
      <?php print $content; ?>
    </div>
  </div>
</div>