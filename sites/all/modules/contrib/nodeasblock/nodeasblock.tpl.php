<?php
// $Id: nodeasblock.tpl.php,v 1.1.2.1 2009/08/20 04:12:14 herc Exp $

/**
 * @file nodeasblock.tpl.php
 * Default theme implementation for rendering a single node as a block.
 *
 * Available variables:
 * - $content: Node content rendered.
 * - $edit_link: Node edit link.
 *
 * @see template_preprocess_nodeasblock()
 */
?>
<?php print $content; ?>
<?php if ($edit_link): ?>
<div class="nodeasblock-edit-link"><?php print $edit_link ?></div>
<?php endif; ?>
