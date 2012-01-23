<?php
/**
 * @file
 * Theme for Custom Formatters Export Info.
 *
 * Available variables:
 * - $formatters: An array of formatters to export.
 * - $name: A string containing the exported module name.
 * - $basic: A boolean value indicating presence of 'basic' formatters.
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
name = <?php print $name . "\n"; ?>
description = Contains exported formatters for the '<?php print $name; ?>' module.
core = 6.x
<?php if (!isset($basic) || !$basic) : ?>
dependencies[] = content
<?php else : ?>
dependencies[] = custom_formatters
<?php endif;
