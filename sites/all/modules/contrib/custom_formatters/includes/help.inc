<?php
/**
 * Help module integration.
 */

/**
 * Implements hook_help().
 */
function custom_formatters_help($path, $arg) {
  switch ($path) {
    case 'admin/help#custom_formatters':
      $output = '<p>' . t('The Custom Formatters module is a simple utility for creating CCK Formatters which are used to theme the output of CCK fields.') . '</p>';
      $output .= '<p>' . t('There are two different modes available for creating a custom formatter, each with their own pros and cons:') . '</p>';
      $output .= '<h2>' . t('Basic') . '</h2>';
      $output .= '<p>' . t('The basic formatter editor offers a simple HTML editor with support for the !token module for extremely easy custom formatter creation, but your formatter can only be created for a single CCK field type and can not support multiple values.', array('!token' => l(t('Token'), 'http://drupal.org/project/token'))) . '</p>';
      $output .= '<h2>' . t('Advanced') . '</h2>';
      $output .= '<p>' . t('The advanced formatter editor supports multiple CCK field types and multiple values in a single formatter, but as it is PHP based the user will require a basic knowledge of !php and the !drupalapi.', array('!php' => l(t('PHP'), 'http://www.php.net'), '!drupalapi' => l(t('Drupal API'), 'http://api.drupal.org'))) . '</p>';
      $output .= '<p>&nbsp;</p><h2>' . t('Tips & Tricks for Advanced formatters') . '</h2>';
      $output .= '<ul><li>' . t('Before you can create an advanced formatter, you need to know what variables you have to work with, the easiest way to determine this information is by doing the following:');
      $output .= '<ol><li>' . t('Install and enable the !devel module and the !devel_generate sub-module.', array('!devel' => l(t('Devel'), 'http://drupal.org/project/devel'), '!devel_generate' => '<strong>' . t('Devel generate') . '</strong>')) . '</li>';
      $output .= '<li>' . t('Create a new formatter containing the following code:') . '<br /><code>dpm($element);</code></li>';
      $output .= '<li>' . t('Hit the !preview button.', array('!preview' => '<strong>' . t('Preview') . '</strong>')) . '</li></ol></li>';
      $output .= '<li>' . t('As mentioned above, the code for formatters with %multiple option disabled is different than with it enabled, this is due to the structure of the $element variable. The following code snippets are basic templates for handling these two variations:', array('%multiple' => t('Handle multiple values')));
      $output .= '<br /><br /><strong>' . t('With %multiple disabled:', array('%multiple' => t('Handle multiple values'))) . '</strong><br /><code>$output = \'\';<br />if ($element[\'#item\']) {<br />&nbsp;&nbsp;// Formatter code goes here.<br />}<br />return $output;</code>';
      $output .= '<br /><br /><strong>' . t('With %multiple enabled:', array('%multiple' => t('Handle multiple values'))) . '</strong><br /><code>$output = \'\';<br />foreach (element_children($element) as $key) {<br />&nbsp;&nbsp;if ($element[$key][\'#item\']) {<br />&nbsp;&nbsp;&nbsp;&nbsp;// Formatter code goes here.<br />&nbsp;&nbsp;}<br />}<br />return $output;</code>';
      $output .= '<br /><br /><strong>' . t('Note') . ':</strong> ' . t('This code is only meant as an example of the differences and may not satisfy all users needs.') . '</li>';
      $output .= '</ul>';
      return $output;
  }
}
