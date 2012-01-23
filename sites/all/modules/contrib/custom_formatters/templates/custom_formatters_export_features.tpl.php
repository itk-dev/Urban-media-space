<?php
/**
 * @file
 * Theme for Custom Formatters Export Features.
 *
 * Available variables:
 * - $formatters: An array of formatters to export.
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
  $formatters = array();
<?php foreach ($formatters as $formatter) : ?>

  // <?php echo $formatter->label ?>.
  $formatters['<?php echo $formatter->name ?>'] = array(
<?php foreach ((array) $formatter as $key => $value) : ?>
    '<?php echo $key ?>' => <?php echo custom_formatters_var_export($value, '  ') ?>,
<?php endforeach; ?>
  );
<?php endforeach; ?>

  return $formatters;
