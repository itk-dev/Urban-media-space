<?php
// $Id: block.tpl.php,v 1.1 2009/06/26 00:33:39 duvien Exp $

/**
 * @file block.tpl.php
 *
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $block->subject: Block title.
 * - $block->content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: This is a numeric id connected to each module.
 * - $block->region: The block region embedding the current block.
 * - $classes: A set of CSS classes for the DIV wrapping the block.
     Possible values are: block-MODULE, region-odd, region-even, odd, even,
     region-count-X, and count-X.
 *
 * Helper variables:
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_block()
 */
?>

<?php print views_embed_view('spotbox', 'page_1', $node->nid); ?>
<?php print views_embed_view('spotbox', 'page_2', $node->nid); ?>
<?php print views_embed_view('spotbox', 'page_3', $node->nid); ?>