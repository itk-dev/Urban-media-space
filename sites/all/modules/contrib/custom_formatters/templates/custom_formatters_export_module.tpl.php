<?php
/**
 * @file
 * Theme for Custom Formatters Export Module.
 *
 * Available variables:
 * - $formatters: An array of formatters to export.
 * - $name: A string containing the exported module name.
 * - $mode: A string containing the selected export mode.
 *
 * Each $formatter in $formatters contains:
 * - $formatter->name: The alphanumeric id of the formatter.
 * - $formatter->label: The human-readable title of the formatter.
 * - $formatter->field_types: A serialized array of supported field types.
 * - $formatter->multiple: A boolean value determining whether the formatter
 *   supports multiple values.
 * - $formatter->description: The description of the formatter.
 * - $formatter->mode: The mode of the formatter (basic/advanced).
 * - $formatter->code: The formatter data.
 */
?>
<?php print "<?php\n"; ?>
/**
 * @file
 * Contains exported formatters for the '<?php print $name; ?>' module.
 */

<?php if ($mode == 'standard') : ?>
/**
 * Implements hook_theme().
 */
function <?php print drupal_strtolower(str_replace(' ', '_', $name)); ?>_theme() {
  return array(
<?php foreach ($formatters as $formatter) : ?>
    '<?php print drupal_strtolower(str_replace(' ', '_', $name)); ?>_formatter_<?php print $formatter->name; ?>' => array(
      'arguments' => array('element' => NULL),
    ),
<?php endforeach; ?>
  );
}

/**
 * Implements hook_field_formatter_info().
 */
function <?php print drupal_strtolower(str_replace(' ', '_', $name)); ?>_field_formatter_info() {
  return array(
<?php foreach ($formatters as $formatter) : ?>
    '<?php print $formatter->name; ?>' => array(
      'label' => '<?php print addslashes($formatter->label); ?>',
      'description' => t('<?php print addslashes($formatter->description); ?>'),
      'field types' => array('<?php print implode("', '", unserialize($formatter->field_types)); ?>'),
      'multiple values' => <?php print $formatter->multiple ? 'CONTENT_HANDLE_MODULE' : 'CONTENT_HANDLE_CORE' ?>,
    ),
<?php endforeach; ?>
  );
}
<?php foreach ($formatters as $formatter) : ?>

function theme_<?php print drupal_strtolower(str_replace(' ', '_', $name)); ?>_formatter_<?php print $formatter->name ?>($element) {
<?php foreach (explode("\n", $formatter->code) as $line) { ?>
  <?php print $line . "\n"; ?><?php } ?>
}
<?php endforeach; ?>
<?php elseif ($mode == 'custom') : ?>
/**
 * Implements hook_custom_formatters_defaults().
 */
function <?php print drupal_strtolower(str_replace(' ', '_', $name)); ?>_custom_formatters_defaults() {
<?php echo theme('custom_formatters_export_features', $formatters) ?>
}
<?php endif;
