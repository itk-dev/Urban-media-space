<?php
/**
 * @file
 * Theme for Custom Formatters Convert.
 *
 * Available variables:
 * - $code: A string containing the formatter code for conversion.
 */
?>
// Parse tokens.
$formatter = is_object($element['#formatter']) ? clone $element['#formatter'] : clone custom_formatters_formatter($element['#formatter']);
$formatter->code = "<?php print addslashes($code); ?>";
return _custom_formatters_token_replace($formatter, $element);
