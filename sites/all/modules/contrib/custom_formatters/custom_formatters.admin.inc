<?php
/**
 * @file
 * Contains administration functions for the Custom Formatters module.
 */

function custom_formatters_overview($form_state) {
  if (isset($form_state['values']['operation'])) {
    switch ($form_state['values']['operation']) {
      case 'delete':
        return custom_formatters_bulk_delete_confirm($form_state, array_filter($form_state['values']['formatters']));

      case 'convert':
        return custom_formatters_bulk_convert_confirm($form_state, array_filter($form_state['values']['formatters']));

      case 'export':
        drupal_set_title(t('Bulk export'));
        return custom_formatters_formatter_export_form($form_state, array_filter($form_state['values']['formatters']));
    }
  }

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => array(
      'export' => t('Export'),
      'convert' => t('Convert'),
      'delete' => t('Delete'),
    ),
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('custom_formatters_bulk_submit'),
  );

  $destination = drupal_get_destination();
  $formatters = array();
  foreach (custom_formatters_formatters(FALSE, 25) as $formatter) {
    $operations = array();
    if ($formatter->status) {
      $operations[] = l(t('Edit'), 'admin/build/formatters/edit/' . $formatter->name, array('query' => $destination));
      $operations[] = $formatter->source == 'module' ? t('Revert') : l(isset($formatter->override) ? t('Revert') : t('Delete'), 'admin/build/formatters/delete/' . $formatter->name, array('query' => $destination));
      $operations[] = l(t('Clone'), 'admin/build/formatters/clone/' . $formatter->name, array('query' => $destination));
      $operations[] = l(t('Export'), 'admin/build/formatters/export/' . $formatter->name, array('query' => $destination));
      $operations[] = $formatter->mode == 'basic' ? l(t('Convert'), 'admin/build/formatters/convert/' . $formatter->name, array('query' => $destination)) : t('Convert');
      $operations[] = l(t('Disable'), 'admin/build/formatters/status/' . $formatter->name, array('query' => $destination));
    }
    else {
      $operations[] = l(t('Enable'), 'admin/build/formatters/status/' . $formatter->name, array('query' => $destination));
    }

    $formatters[$formatter->name] = '';
    $form['label'][$formatter->name] = array('#value' => l($formatter->label, 'admin/build/formatters/edit/' . $formatter->name, array('query' => $destination)));
    $form['mode'][$formatter->name] = array('#value' => $formatter->mode);
    $form['fields'][$formatter->name] = array('#value' => implode(', ', unserialize($formatter->field_types)));
    $form['operations'][$formatter->name] = array('#value' => implode('&nbsp;&nbsp;&nbsp;&nbsp;', $operations));
  }

  $form['formatters'] = array('#type' => 'checkboxes', '#options' => $formatters);
  $form['pager'] = array('#value' => theme('pager', NULL, 25));

  return $form;
}

function theme_custom_formatters_overview($form) {
  $output = '';
  if (isset($form['export'])) {
    return $output;
  }

  $empty = !(isset($form['label']) && is_array($form['label']));
  $select_header = !$empty ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Label'), t('Mode'), t('Field types'), t('Operations'));
  $rows = array();

  $output .= drupal_render($form['options']);
  if (!$empty) {
    foreach (element_children($form['label']) as $key) {
      $row = array();
      $row[] = drupal_render($form['formatters'][$key]);
      $row[] = drupal_render($form['label'][$key]);
      $row[] = drupal_render($form['mode'][$key]);
      $row[] = drupal_render($form['fields'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No formatters available.'), 'colspan' => '4'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

function custom_formatters_bulk_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function custom_formatters_formatter_form($form_state, $formatter = NULL, $op = 'add') {
  $form = array();

  if ($op == 'clone' && empty($form_state['post'])) {
    drupal_set_message(t('Be sure to change the formatter %name when cloning.', array('%name' => 'Name')), 'warning');
  }

  $form['new'] = array(
    '#type' => 'value',
    '#value' => (empty($formatter) || $op == 'clone'),
  );
  $form['old'] = array(
    '#type' => 'value',
    '#value' => !empty($formatter) ? $formatter->name : NULL,
  );

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic information'),
  );
  $form['basic']['name'] = array(
    '#type' => 'textfield',
    '#size' => '64',
    '#maxlength' => '64',
    '#title' => t('Name'),
    '#default_value' => !is_null($formatter) ? $formatter->name : '',
    '#description' => t('For internal use, must be unique and only use alphanumeric characters and underscores (_).'),
    '#required' => TRUE,
    '#disabled' => (isset($formatter) && ($op !== 'clone' && (isset($formatter->override) || $formatter->source == 'database'))),
  );
  if (isset($formatter) && ($op !== 'clone' && (isset($formatter->override) || $formatter->source == 'database'))) {
    $form['basic']['name']['#value'] = $form['basic']['name']['#default_value'];
  }
  $form['basic']['label'] = array(
    '#type' => 'textfield',
    '#size' => '64',
    '#maxlength' => '64',
    '#title' => t('Label'),
    '#default_value' => !is_null($formatter) ? $formatter->label : '',
    '#description' => t('Human readable name, displayed on Custom Formatters overview page and when selecting your CCK Formatter (prefixed with "!custom").', array('!custom' => t('Custom:'))),
    '#required' => TRUE,
  );
  $form['basic']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => !is_null($formatter) ? $formatter->description : '',
  );
  $form['basic']['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Editor mode'),
    '#options' => array(
      'basic' => t('Basic'),
      'advanced' => t('Advanced'),
    ),
    '#default_value' => !is_null($formatter) ? $formatter->mode : 'basic',
  );

  if ($op == 'add' && !isset($form_state['storage']['op'])) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
    );
  }

  else {
    if ($op == 'add' && $form_state['storage']['op'] == t('Next')) {
      $form['basic']['name']['#default_value'] = $form_state['values']['name'];
      $form['basic']['label']['#default_value'] = $form_state['values']['label'];
      $form['basic']['description']['#default_value'] = $form_state['values']['description'];
      $form['basic']['mode']['#default_value'] = $form_state['values']['mode'];
    }

    $form['basic']['#collapsible'] = ($op != 'clone');
    $form['basic']['#collapsed'] = ($op != 'clone');
    $form['basic']['mode']['#disabled'] = TRUE;
    $form['basic']['mode_value'] = array(
      '#type' => 'hidden',
      '#value' => $form['basic']['mode']['#default_value'],
    );

    $form['editor'] = array(
      '#type' => 'fieldset',
      '#title' => t('!mode editor', array('!mode' => $form['basic']['mode']['#options'][$form['basic']['mode']['#default_value']])),
    );

    // Custom Formatter preview.
    if (module_exists('devel_generate')) {
      $form['preview'] = array(
        '#type' => 'fieldset',
        '#title' => t('Preview'),
        '#description' => '<div id="preview-wrapper">' . theme('custom_formatters_preview', $formatter) . '</div>',
        '#collapsible' => TRUE,
      );
      $form['preview']['node'] = array(
        '#type' => 'select',
        '#title' => t('Preview node type'),
        '#options' => devel_generate_content_types(),
        '#default_value' => array_shift(array_keys(devel_generate_content_types())),
        '#description' => '<p><em><strong>' . t('Note') . ':</strong> ' . t('Be aware that previews are only available if a module provides support for !develgen and is not guaranteed to be an exact representation of the formatter. It is recommended to test the formatter on an active node for the best results.', array('!develgen' => '<strong>' . t('Devel Generate') . '</strong>')) . '</em></p>',
      );
      $form['buttons']['preview'] = array(
        '#type' => 'button',
        '#value' => t('Preview'),
        '#ahah' => array(
          'path' => 'js/formatters/preview',
          'wrapper' => 'preview-wrapper',
        ),
      );
    }

    // Invoke selected editor form.
    $function = "custom_formatters_formatter_{$form['basic']['mode']['#default_value']}_editor_form";
    $form['editor'] = array_merge($form['editor'], $function($form_state, $formatter));

    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('custom_formatters_formatter_form_save'),
    );

    $form['buttons']['edit'] = array(
      '#type' => 'submit',
      '#value' => t('Save & Edit'),
      '#submit' => array('custom_formatters_formatter_form_save_edit'),
    );

    if ($op == 'edit') {
      $form['buttons']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#submit' => array('custom_formatters_formatter_form_delete'),
      );
    }
  }

  return $form;
}

function custom_formatters_formatter_basic_editor_form($form_state, $formatter = NULL) {
  $form = array();

  $options = array();
  $fields = _content_field_types();
  // Give modules a chance to alter fields.
  drupal_alter('custom_formatters_fields', $fields);

  ksort($fields);
  foreach ($fields as $id => $field) {
    $options[$field['module']][$id] = $field['label'];
  }
  ksort($options);

  $form['field_types'] = array(
    '#type' => 'select',
    '#title' => t('Field type'),
    '#options' => $options,
    '#default_value' => !is_null($formatter) ? implode(unserialize($formatter->field_types)) : '',
    '#ahah' => array(
      'path' => 'js/formatters/tokens',
      'wrapper' => 'tokens-wrapper',
    ),
    '#required' => TRUE,
  );
  $form['multiple'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML'),
    '#rows' => 10,
    '#default_value' => !is_null($formatter) ? $formatter->code : '',
    '#required' => TRUE,
    '#attributes' => array(
      'class' => 'syntax-html',
    ),
  );
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tokens'),
    '#description' => '<div id="tokens-wrapper">' . theme('custom_formatters_token_tree', $form['field_types']['#default_value']) . '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return $form;
}

function custom_formatters_formatter_advanced_editor_form($form_state, $formatter = NULL) {
  $form = array();

  $form['field_types'] = array(
    '#type' => 'textfield',
    '#title' => t('Field type(s)'),
    '#autocomplete_path'  => 'js/formatters/autocomplete',
    '#default_value' => !is_null($formatter) ? implode(', ', unserialize($formatter->field_types)) : '',
    '#description' => t('A comma-separated list of CCK fields. Example: number_integer, number_decimal.'),
    '#required' => TRUE,
  );
  $form['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Handle multiple values'),
    '#default_value' => !is_null($formatter) ? $formatter->multiple : 0,
  );
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP'),
    '#rows' => 10,
    '#default_value' => !is_null($formatter) ? $formatter->code : '',
    '#description' => t('Enter the php code that will be evaluated. You do not need to use enclose the code between %php. The $element variable provides information about the field being formatted. The code should return a string.', array('%php' => '<?php ?>')),
    '#required' => TRUE,
    '#attributes' => array(
      'class' => 'syntax-php',
    ),
  );

  return $form;
}

function custom_formatters_formatter_form_validate($form, &$form_state) {
  // Formatter name must be alphanumeric characters and underscores (_) only.
  if (preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['name'])) {
    form_error($form['basic']['name'], t('Formatter name must be alphanumeric characters and underscores (_) only.'));
  }

  // Make sure formatter name isn't already in use by another formatter.
  $formatter = custom_formatters_formatter($form_state['values']['name']);
  if (!empty($formatter) && ($form_state['values']['new'] == TRUE || $form_state['values']['old'] !== $formatter->name)) {
    form_error($form['basic']['name'], t('A formatter with the name %name already exists.', array('%name' => $form_state['values']['name'])));
  }
}

function custom_formatters_formatter_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Next')) {
    $form_state['storage'] = $form_state['values'];
  }
}

function custom_formatters_formatter_form_save($form, &$form_state) {
  $form_state['values']['field_types'] = explode(', ', $form_state['values']['field_types']);

  // Create new formatter.
  if ($form_state['values']['new']) {
    drupal_set_message(t('Formatter %name was added.', array('%name' => $form_state['values']['name'])));
  }

  // Update existing formatter.
  else {
    db_query("DELETE FROM {formatters} WHERE name = '%s'", $form_state['values']['old']);
    drupal_set_message(t('Formatter %name was updated.', array('%name' => $form_state['values']['name'])));
  }

  // Write formatter to database.
  drupal_write_record('formatters', $form_state['values']);

  custom_formatters_clear_cache();
  unset($form_state['storage']);

  $_REQUEST['destination'] = empty($_REQUEST['destination']) ? 'admin/build/formatters' : $_REQUEST['destination'];
}

function custom_formatters_formatter_form_save_edit($form, &$form_state) {
  custom_formatters_formatter_form_save($form, $form_state);
  $_REQUEST['destination'] = 'admin/build/formatters/edit/' . $form_state['values']['name'] . '?destination=' . $_REQUEST['destination'];
}

function custom_formatters_formatter_form_delete($form, &$form_state) {
  $_REQUEST['destination'] = 'admin/build/formatters/delete/' . $form_state['values']['name'] . '?destination=' . $_REQUEST['destination'];
}

function custom_formatters_formatter_delete_form($form_state, $formatter = array()) {
  if (empty($formatter)) {
    drupal_set_message(t('The specified formatter was not found'), 'error');
    drupal_goto('admin/build/formatters');
  }

  $form = array();

  $form['name'] = array(
    '#type' => 'value',
    '#value' => $formatter->name,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to !delete the formatter %name?', array('%name' => $formatter->name, '!delete' => isset($formatter->override) ? t('revert') : t('delete'))),
    'admin/build/formatters',
    t('This action cannot be undone.'),
    t('!delete', array('!delete' => isset($formatter->override) ? t('Revert') : t('Delete'))),
    t('Cancel')
  );
}

function custom_formatters_formatter_delete_form_submit($form, &$form_state) {
  $formatter = custom_formatters_formatter($form_state['values']['name']);

  db_query("DELETE FROM {formatters} WHERE name = '%s'", $form_state['values']['name']);
  custom_formatters_clear_cache();

  drupal_set_message(t('Formatter %name was deleted.', array('%name' => $formatter->name)));
  $form_state['redirect'] = 'admin/build/formatters';
}

function custom_formatters_bulk_delete_confirm(&$form_state, $formatters) {
  $form = array();
  $form['formatters'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );

  foreach ($formatters as $name) {
    $formatter = custom_formatters_formatter($name);
    $form['formatters'][$name] = array(
      '#type' => 'hidden',
      '#value' => $name,
      '#prefix' => '<li>',
      '#suffix' => check_plain($formatter->label) . ' <em>(' . check_plain($formatter->name) . ")</em></li>\n",
    );
  }

  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'delete',
  );
  $form['#submit'][] = 'custom_formatters_bulk_delete_confirm_submit';

  return confirm_form(
    $form,
    t('Are you sure you want to delete these items?'),
    'admin/build/formatters', t('This action cannot be undone.'),
    t('Delete all'), t('Cancel')
  );
}

function custom_formatters_bulk_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['formatters'] as $name) {
      db_query("DELETE FROM {formatters} WHERE name = '%s'", $name);
    }
    custom_formatters_clear_cache();
    drupal_set_message(t('The items have been deleted.'));
  }
  $form_state['redirect'] = 'admin/build/formatters';
  return;
}

function custom_formatters_formatter_export_form($form_state, $formatters = array()) {
  if (empty($formatters)) {
    drupal_set_message(t('The specified formatter was not found'), 'error');
    drupal_goto('admin/build/formatters');
  }

  $form = array('#tree' => TRUE);
  if (is_array($formatters)) {
    $form['operation'] = array(
      '#type' => 'hidden',
      '#value' => 'export',
    );
    $form['formatters']['#tree'] = TRUE;
    foreach ($formatters as $formatter) {
      $formatters[$formatter] = custom_formatters_formatter($formatter);
      $form['formatters'][$formatter] = array(
        '#type' => 'hidden',
        '#value' => $formatter,
      );
    }
    $form['#submit'][] = 'custom_formatters_formatter_export_form_submit';
  }
  else {
    $formatters = array($formatters->name => $formatters);
  }

  // Cleanup formatters array.
  foreach ($formatters as &$formatter) {
    unset($formatter->source);
    if (isset($formatter->override)) {
      unset($formatter->override);
    }
  }

  $form['extra'] = array(
    '#type' => 'fieldset',
  );
  $form['extra']['module'] = array(
    '#type' => 'textfield',
    '#title' => t('Module name'),
    '#default_value' => !empty($form_state['storage']) ? $form_state['storage']['module'] : 'mymodule',
  );
  $form['extra']['mode'] = array(
    '#type' => 'select',
    '#title' => t('Export mode'),
    '#options' => array(
      'standard' => t('Standard CCK formatters'),
      'custom' => t('Custom formatters'),
    ),
    '#default_value' => !empty($form_state['storage']) ? $form_state['storage']['mode'] : 'standard',
    '#description' =>
      '<strong>' . t('Standard CCK formatters') . ':</strong> ' . t('Uses the standard CCK formatters API (hook_theme(), hook_field_formatter_info(), etc) for a pure code export.') . '<br /><br />' .
      '<strong>' . t('Custom formatters') . ':</strong> ' . t('Uses a custom format (hook_custom_formatters_default()) to provide more control, in a similar fashion to Views.'),
  );
  $form['extra']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update',
  );

  $filename = drupal_strtolower(str_replace(' ', '_', $form['extra']['module']['#default_value']));
  $form['export']['output'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export'),
    '#description' => t('Download !file.', array('!file' => l($filename . (function_exists('gzencode') ? '.tgz' : '.tar'), 'admin/build/formatters/download/' . $form['extra']['mode']['#default_value'] . '/' . $filename . (function_exists('gzencode') ? '.tgz' : '.tar') . '?' . implode(',', array_keys($formatters))))),
  );
  $info = theme('custom_formatters_export_info', $formatters, $form['extra']['module']['#default_value']);
  $form['export']['output']['info'] = array(
    '#type' => 'textarea',
    '#title' => "{$filename}.info",
    '#value' => $info,
    '#rows' => count(explode("\n", $info)),
  );
  $module = theme("custom_formatters_export_module", $formatters, $form['extra']['module']['#default_value'], $form['extra']['mode']['#default_value']);
  $form['export']['output']['module'] = array(
    '#type' => 'textarea',
    '#title' => "{$filename}.module",
    '#value' => $module,
    '#rows' => count(explode("\n", $module)),
  );

  return $form;
}

function custom_formatters_formatter_export_form_submit($form, &$form_state) {
  $form_state['storage']['module'] = $form_state['values']['extra']['module'];
  $form_state['storage']['mode'] = $form_state['values']['extra']['mode'];
  $formatters = isset($form_state['values']['formatters']) ? $form_state['values']['formatters'] : NULL;
}

/**
 * Export var function -- from Views.
 */
function custom_formatters_var_export($var, $prefix = '', $init = TRUE) {
  if (is_object($var)) {
    $output = method_exists($var, 'export') ? $var->export() : custom_formatters_var_export((array) $var);
  }
  elseif (is_array($var)) {
    if (empty($var)) {
      $output = 'array()';
    }
    else {
      $output = "array(\n";
      foreach ($var as $key => $value) {
        $output .= "  '$key' => " . custom_formatters_var_export($value, '  ', FALSE) . ",\n";
      }
      $output .= ')';
    }
  }
  elseif (is_bool($var)) {
    $output = $var ? 'TRUE' : 'FALSE';
  }
  elseif (is_string($var) && strpos($var, "\n") !== FALSE) {
    // Replace line breaks in strings with a token for replacement
    // at the very end. This protects whitespace in strings from
    // unintentional indentation.
    $var = str_replace("\n", "***BREAK***", $var);
    $output = var_export($var, TRUE);
  }
  else {
    $output = var_export($var, TRUE);
  }

  if ($prefix) {
    $output = str_replace("\n", "\n$prefix", $output);
  }

  if ($init) {
    $output = str_replace("***BREAK***", "\n", $output);
  }

  return $output;
}

function custom_formatters_formatter_export_tar() {
  $args = explode('?', arg(5));
  $file = explode('.', $args[0]);

  $formatters = array();
  foreach (explode(',', $args[1]) as $name) {
    $formatters[$name] = custom_formatters_formatter($name);
  }

  $tar = array(
    "{$file[0]}/{$file[0]}.info" => theme('custom_formatters_export_info', $formatters, $file[0]),
    "{$file[0]}/{$file[0]}.module" => theme('custom_formatters_export_module', $formatters, $file[0], arg(4))
  );
  $tar = _custom_formatters_tar_create($tar);

  // Clear out output buffer to remove any garbage from tar output.
  if (ob_get_level()) {
    ob_end_clean();
  }

  $header = function_exists('gzencode') ? 'Content-type: application/x-gzip' : 'Content-type: application/x-tar';
  drupal_set_header($header);
  drupal_set_header("Content-Disposition: attachment; filename='{$args[0]}'");
  print $tar;
  exit;
}

/**
 * Tar creation function. Written by dmitrig01. Taken from the Features module.
 *
 * @param $files
 *   A keyed array where the key is the filepath and the value is the
 *   string contents of the file.
 *
 * @return
 *   A string of the tar file contents.
 */
function _custom_formatters_tar_create($files) {
  $tar = '';
  foreach ($files as $name => $contents) {
    $binary_data_first = pack("a100a8a8a8a12A12",
      $name,
      '100644 ', // File permissions
      '   765 ', // UID,
      '   765 ', // GID,
      sprintf("%11s ", decoct(strlen($contents))), // Filesize,
      sprintf("%11s", decoct(time())) // Creation time
    );
    $binary_data_last = pack("a1a100a6a2a32a32a8a8a155a12", '', '', '', '', '', '', '', '', '', '');

    $checksum = 0;
    for ($i = 0; $i < 148; $i++) {
      $checksum += ord(substr($binary_data_first, $i, 1));
    }
    for ($i = 148; $i < 156; $i++) {
      $checksum += ord(' ');
    }
    for ($i = 156, $j = 0; $i < 512; $i++, $j++) {
      $checksum += ord(substr($binary_data_last, $j, 1));
    }

    $tar .= $binary_data_first;
    $tar .= pack("a8", sprintf("%6s ", decoct($checksum)));
    $tar .= $binary_data_last;

    $buffer = str_split($contents, 512);
    foreach ($buffer as $item) {
      $tar .= pack("a512", $item);
    }
  }
  if (function_exists('gzencode')) {
    $tar = gzencode($tar);
  }
  return $tar;
}

function custom_formatters_formatter_convert_form($form_state, $formatter = array()) {
  if (empty($formatter)) {
    drupal_set_message(t('The specified formatter was not found'), 'error');
    drupal_goto('admin/build/formatters');
  }

  $form = array();

  $form['name'] = array(
    '#type' => 'value',
    '#value' => $formatter->name,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to convert the formatter %name?', array('%name' => $formatter->name)),
    'admin/build/formatters',
    t('This action cannot be undone.'),
    t('Convert'),
    t('Cancel')
  );
}

function custom_formatters_formatter_convert_form_submit($form, &$form_state) {
  $formatter = _custom_formatter_convert($form_state['values']['name']);

  drupal_set_message(t('Formatter %name was converted.', array('%name' => $formatter->name)));
  custom_formatters_clear_cache();

  $form_state['redirect'] = 'admin/build/formatters';
}

function custom_formatters_bulk_convert_confirm(&$form_state, $formatters) {
  $form = array();
  $form['formatters'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul><p><strong>' . t('Note') . ':</strong> ' . t("Only 'basic' formatters will be converted.") . '</p>',
    '#tree' => TRUE,
  );

  foreach ($formatters as $name) {
    $formatter = custom_formatters_formatter($name);
    if ($formatter->mode == 'basic') {
      $form['formatters'][$name] = array(
        '#type' => 'hidden',
        '#value' => $name,
        '#prefix' => '<li>',
        '#suffix' => check_plain($formatter->label) . ' <em>(' . check_plain($formatter->name) . ")</em></li>\n",
      );
    }
  }

  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'convert',
  );
  $form['#submit'][] = 'custom_formatters_bulk_convert_confirm_submit';

  return confirm_form(
    $form,
    t('Are you sure you want to convert these items?'),
    'admin/build/formatters', t('This action cannot be undone.'),
    t('Convert all'), t('Cancel')
  );
}

function custom_formatters_bulk_convert_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['formatters'] as $name) {
      $formatter = _custom_formatter_convert($name);
    }
    custom_formatters_clear_cache();
    drupal_set_message(t('The items have been converted.'));
  }
  $form_state['redirect'] = 'admin/build/formatters';
  return;
}

function _custom_formatter_convert($name) {
  $formatter = custom_formatters_formatter($name);

  // Prepare formatter for conversion.
  $formatter->field_types = unserialize($formatter->field_types);
  $formatter->mode = 'advanced';

  // Convert formatter code to advanced formatter code.
  $formatter->code = theme('custom_formatters_convert', $formatter->code);

  // Save formatter.
  drupal_write_record('formatters', $formatter, 'name');

  return $formatter;
}

function custom_formatters_formatter_status_form($form_state, $formatter = array()) {
  if (empty($formatter)) {
    drupal_set_message(t('The specified formatter was not found'), 'error');
    drupal_goto('admin/build/formatters');
  }

  $form = array();

  $form['name'] = array(
    '#type' => 'value',
    '#value' => $formatter->name,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to !status the formatter %name?', array('%name' => $formatter->name, '!status' => $formatter->status ? t('disable') : t('enable'))),
    'admin/build/formatters',
    $formatter->status ? t('If this formatter is in use it could cause issues.') : '',
    t('!status', array('!status' => $formatter->status ? t('Disable') : t('Enable'))),
    t('Cancel')
  );
}

function custom_formatters_formatter_status_form_submit($form, &$form_state) {
  $formatter = custom_formatters_formatter($form_state['values']['name']);

  // Prepare formatter for conversion.
  $formatter->field_types = unserialize($formatter->field_types);
  $formatter->status = $formatter->status ? 0 : 1;

  db_query("DELETE FROM {formatters} WHERE name = '%s'", $formatter->name);
  drupal_write_record('formatters', $formatter);

  custom_formatters_clear_cache();

  drupal_set_message(t('Formatter %name was !status.', array('%name' => $formatter->name, '!status' => ($formatter->status ? t('enabled') : t('disabled')))));
  $form_state['redirect'] = 'admin/build/formatters';
}

function custom_formatters_token_tree() {
  drupal_json(array(
    'status' => TRUE,
    'data' => theme_custom_formatters_token_tree($_POST['field_types'])
  ));
}

/**
 * Provide a 'tree' display of nested tokens.
 */
function theme_custom_formatters_token_tree($field = NULL, $global_types = FALSE, $click_insert = TRUE) {
  module_load_include('inc', 'token', 'token.pages');

  $info = array();
  $fields = _content_field_types();
  if (!empty($fields) && isset($fields[$field])) {
    $module = $fields[$field]['module'];

    // Build tokens list.
    foreach (_custom_formatters_tokens_list($field) as $module) {
      if (function_exists($function = "{$module}_token_list")) {
        $info = array_merge_recursive($function('field'), $info);
      }
    }
  }
  $info = array_merge($info, token_get_list('node'));

  // Prepend node tokens with 'node-' to prevent namespace conflicts.
  $info['node'] = array_flip($info['node']);
  foreach ($info['node'] as &$token) {
    $token = "node-{$token}";
  }
  $info['node'] = array_flip($info['node']);

  $token_types = array_keys($info);
  sort($token_types);

  $header = array(
    t('Token'),
    t('Description'),
  );
  $rows = array();

  foreach ($token_types as $type) {
    $parent = NULL;

    if (count($token_types) > 1) {
      $rows[] = _token_token_tree_format_row($type, array(), TRUE);
      $parent = $type;
    }

    foreach ($info[$type] as $token => $description) {
      $rows[] = _token_token_tree_format_row("[$token]", array('description' => $description, 'parent' => $parent));
    }
  }

  if (count($rows)) {
    drupal_add_js(drupal_get_path('module', 'token') . '/jquery.treeTable.js');
    drupal_add_css(drupal_get_path('module', 'token') . '/jquery.treeTable.css');
    drupal_add_js(drupal_get_path('module', 'token') . '/token.js');
    drupal_add_css(drupal_get_path('module', 'token') . '/token.css');
  }
  else {
    $rows[] = array(array(
      'data' => t('No tokens available.'),
      'colspan' => 2,
    ));
  }

  $table_options = array(
    'attributes' => array('class' => 'token-tree'),
    'caption' => '',
  );
  if ($click_insert) {
    $table_options['caption'] = t('Click a token to insert it into the field you\'ve last clicked.');
    $table_options['attributes']['class'] .= ' token-click-insert';
  }
  return theme('table', $header, $rows, $table_options['attributes'], $table_options['caption']);
}

/**
 * Menu callback; Retrieve a JSON object containing Custom Formatter preview.
 */
function custom_formatters_preview() {
  $_POST['field_types'] = serialize(explode(', ', $_POST['field_types']));
  $_POST['mode'] = $_POST['mode_value'];
  unset($_POST['form_build_id'], $_POST['form_token'], $_POST['form_id'], $_POST['op'], $_POST['mode_value']);

  $output = theme('custom_formatters_preview', (object) $_POST);
  drupal_json(array(
    'status' => TRUE,
    'data' => theme('status_messages') . $output,
  ));
}

function theme_custom_formatters_preview($formatter) {
  module_load_include('inc', 'devel_generate');
  include_once(drupal_get_path('module', 'content') . '/includes/content.devel.inc');
  $output = '';

  // Build temporary node object.
  $node = new stdClass;
  $node->type = isset($formatter->node) ? $formatter->node : array_shift(array_keys(devel_generate_content_types()));
  module_load_include('inc', 'node', 'node.pages');
  node_object_prepare($node);
  devel_generate_content_pre_node($results);
  $users = $results['users'];
  $node->nid = rand(1, 3);
  $node->uid = $users[array_rand($users)];
  $node->title = devel_create_greeking(mt_rand(1, 8), TRUE);
  $node->body = "node ($node->type) - " . devel_create_content();
  $node->teaser = node_teaser($node->body);
  $node->filter = variable_get('filter_default_format', 1);
  $node->format = FILTER_FORMAT_DEFAULT;
  $node->language = '';
  $node->revision = mt_rand(0, 1);
  $node->promote = mt_rand(0, 1);
  $node->created = time() - mt_rand(0, 604800);

  $info = content_types($node->type);
  $fields = _content_field_types();
  // Give modules a change to alter fields.
  drupal_alter('custom_formatters_fields', $fields);

  if (isset($formatter) && !empty($formatter)) {
    foreach (unserialize($formatter->field_types) as $field_type) {
      if (isset($fields[$field_type])) {
        $module = $fields[$field_type]['module'];

        $field = array(
          'field_name' => NULL,
          'type' => $field_type,
          'multiple' => 1,
          'widget' => array(
            'module' => '_custom_formatters',
            'type' => isset($formatter->multiple) && $formatter->multiple ? 'multiple' : 'single',
          ),
        );
        // Invoke hook_custom_formatters_field_prepare().
        if (function_exists($function = "{$module}_custom_formatters_{$field_type}_prepare")) {
          $field = _custom_formatters_array_merge_recursive($function(), $field);
        }

        $items = array();
        // Invoke hook_content_generate().
        if (function_exists($function = "{$module}_content_generate")) {
          $items = $function((array) $node, $field);
          foreach ($info['fields'] as $key => $node_field) {
            if ($node_field['type'] == $field['type']) {
              $node->{$key} = array($items);
            }
          }
        }

        $element = array(
          'items' => array(),
          '#single' => FALSE,
          '#title' => $fields[$field_type]['label'],
          '#page' => TRUE,
          '#field_name' => 'field_devel_generate',
          '#label_display' => 'above',
          '#teaser' => FALSE,
          '#node' => $node,
          '#type' => 'content_field',
          '#custom_formatters' => TRUE,
        );

        if (!isset($formatter->multiple) || !$formatter->multiple) {
          $element['items'] = array(
            '#formatter' => $formatter,
            '#node' => $node,
            '#type_name' => $node->type,
            '#field_name' => 'field_devel_generate',
            '#theme' => 'custom_formatters_formatter',
            '#item' => $items,
            '#devel' => TRUE,
            '#field_type' => $field_type,
          );
        }

        else {
          $element['items'] = array(
            '#formatter' => $formatter,
            '#node' => $node,
            '#type_name' => $node->type,
            '#field_name' => 'field_devel_generate',
            '#theme' => 'custom_formatters_formatter',
            '#node' => $node,
            '#devel' => TRUE,
            '#field_type' => $field_type,
          );
          foreach ($items as $item) {
            $element['items'][] = array(
              '#item' => $item,
            );
          }
        }

        $output .= drupal_render($element);
      }

      else {
        // Field type doesn't exist.
        drupal_set_message(t("Field type %field_type does not exist or it's parent module is currently disabled.", array('%field_type' => $field_type)), 'warning');
      }
    }
  }

  return $output;
}

function custom_formatters_preprocess_content_field(&$vars) {
  if (isset($vars['element']['#custom_formatters'])) {
    $vars['field_type_css'] = 'custom_formatter-preview';
    $vars['field_name_css'] = 'custom_formatter-preview';
    $vars['label'] = $vars['element']['#title'];
    $vars['label_display'] = 'above';
  }
}

function custom_formatters_preprocess_custom_formatters_export_info(&$vars) {
  $vars['basic'] = FALSE;
  foreach ($vars['formatters'] as &$formatter) {
    if ($formatter->mode == 'basic' || strstr($formatter->code, '_custom_formatters_token_replace')) {
      $vars['basic'] = TRUE;
    }
  }
}

function custom_formatters_preprocess_custom_formatters_export_module(&$vars) {
  $vars['basic'] = FALSE;
  foreach ($vars['formatters'] as &$formatter) {
    if ($formatter->mode == 'basic' && !isset($formatter->converted) && $vars['mode'] == 'standard') {
      $formatter->code = theme('custom_formatters_convert', $formatter->code);
      $formatter->converted = TRUE;
      $vars['basic'] = TRUE;
    }
    elseif (strstr($formatter->code, '_custom_formatters_token_replace')) {
      $vars['basic'] = TRUE;
    }
  }
}

function _custom_formatters_array_merge_recursive() {
  $args = func_get_args();
  $a = array_shift($args);

  foreach ($args as $b) {
    foreach ($b as $key => $val) {
      if (is_array($val) && isset($a[$key]) && is_array($a[$key])) {
        $b[$key] = _custom_formatters_array_merge_recursive($a[$key], $val);
      }
    }
    $a = array_merge($a, $b);
  }

  return $a;
}

function _custom_formatters_widget_info() {
  return array(
    'single' => array(
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
    'multiple' => array(
      'multiple values' => CONTENT_HANDLE_MODULE,
    ),
  );
}

/**
 * Custom Formatters settings form.
 */
function custom_formatters_settings_form($form_state) {
  $form = array();
  $settings = variable_get('custom_formatters_settings', array('label_prefix' => TRUE, 'label_prefix_value' => t('Custom')));

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['label_prefix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Label prefix?'),
    '#description' => t('If checked, all Custom Formatters labels will be prefixed with a set value.'),
    '#default_value' => $settings['label_prefix'],
  );

  $form['settings']['label_prefix_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Label prefix'),
    '#default_value' => $settings['label_prefix_value'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Settings form callback; Validate.
 */
function custom_formatters_settings_form_validate($form, $form_state) {
  if ($form_state['values']['settings']['label_prefix'] && empty($form_state['values']['settings']['label_prefix_value'])) {
    form_set_error('settings][label_prefix_value', t('A label prefix must be defined if you wish to use the prefix.'));
  }
}

/**
 * Settings form callback; Submit.
 */
function custom_formatters_settings_form_submit($form, $form_state) {
  drupal_set_message(t('Custom Formatters settings have been updated.'));
  variable_set('custom_formatters_settings', $form_state['values']['settings']);
  custom_formatters_clear_cache();
}

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions
 * for cck widgets.
 */
function custom_formatters_autocomplete($string = '') {
  // The user enters a comma-separated list of fields. We only autocomplete the last tag.
  $array = drupal_explode_tags($string);

  // Fetch last field.
  $last_string = trim(array_pop($array));
  $matches = array();
  if ($last_string != '') {
    $prefix = count($array) ? implode(', ', $array) . ', ' : '';

    $fields = _content_field_types();
    // Give modules a chance to alter fields.
    drupal_alter('custom_formatters_fields', $fields);

    $fields = array_keys($fields);
    sort($fields);
    foreach ($fields as $field) {
      if (preg_match('/^' . drupal_strtolower($last_string) . '/', $field)) {
        $matches[$prefix . $field] = check_plain($field);
      }
    }
  }

  drupal_json($matches);
}
