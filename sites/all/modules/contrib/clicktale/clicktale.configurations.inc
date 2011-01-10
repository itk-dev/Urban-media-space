<?php
// $Id$
/*
 * @file Main configuration page, shows different content depending on the configuration selected.
 * If no configuration is selected, shows the front page.
 */
function clicktale_configurations() {
  $cid = array_pop(explode('/', $_GET['q']));
  $configs = _clicktale_get_configurations();
  $active = variable_get('clicktale-active-configuration', 'default');
  $active = isset($configs[$active]) ? $active : 'default';
     
  $form['header_text'] = array(
    '#type' => 'markup',
    '#value' => '<div id="clicktale-header">'
            . '<div class="active" style="float:left; font-weight:bold;">' . t('Active configuration') . ': ' . $configs[$active]['name'] . '</div>'
            . '<br clear="all" /></div><br /><hr><br />');
  
  // edit an existing configuration
  if ($configs[$cid]) {
    $config = $configs[$cid];
    $form['header_text']['#value'] .= '<h2>Configuration "' . $config['name'] . '"</h2>';
    
    $form['cid'] = array(
      '#type' => 'hidden',
      '#value' => $config['cid']);
    
    // build configuration page
    _clicktale_configuration_block_main($form, $config, $active);
    _clicktale_configuration_block_roles($form, $config);
    _clicktale_configuration_block_pages($form, $config);
    _clicktale_configuration_block_ip($form, $config);
  }
  
  // main configuration page
  else{
    // build main configuration page
    _clicktale_configuration_block_create($form);
    _clicktale_configuration_block_configurations($form, $configs, $active);
    _clicktale_configuration_block_debug($form);
  }
  
  return $form;
}


/*
 * CREATE CONFIGURATION
 */
function _clicktale_configuration_block_create(&$form) {
  $form['new'] = array(
    '#type' => 'textfield',
    '#description' => t('The name of the new configuration. '),
      '#required' => FALSE,
    '#title' => t('Create a new configuration'));
  
  $form['create_new'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_create'),
    '#value' => 'Create');
}
function _clicktale_configuration_save_create($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  $new = $values['new'];
  $key = preg_replace('[^a-z0-9]', '', strtolower($new));
  if (isset($configs[$key])) {
    drupal_set_message('A configuration with this name already exists. ', 'error');
  }
  else {
    $configs[$key] = _clicktale_get_default_config();
    $configs[$key]['name'] = $new;
    $configs[$key]['cid'] = $key;
    _clicktale_save_configurations($configs);
    drupal_set_message('Your new configuration has been saved. ');
    drupal_goto('admin/settings/clicktale/configurations/' . $key);
    exit();
  }
}


/*
 * MANAGE CONFIGURATIONS
 */
function _clicktale_configuration_block_configurations(&$form, $configs, $active) {
  $url = 'http://' . $_SERVER['HTTP_HOST'] . request_uri();
  $url = substr($url, 0, strpos($url, 'settings/clicktale/configurations') + strlen('settings/clicktale/configurations'));
  
  $configs_options = _clicktale_get_configurations_options($configs);
  
  $form['configurations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Installed configurations'));
    
  $form['configurations']['configlist'] = array(
    '#type' => 'radios',
    '#default_value' => $active,
    '#options' => $configs_options);
  
  $form['configurations']['activate'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#value' => 'Activate',
    '#submit' => array('_clicktale_configuration_activate_configuration'),
    '#suffix' => '<input type="button" onclick="if ($(\'input[name=configlist]:checked\').val()) document.location = \'' . $url . '/\' + $(\'input[name=configlist]:checked\').val();" value="Configure" />');
  
  $form['configurations']['end'] = array(
    '#type' => 'markup',
    '#value' => '<!-- end -->');
}
function _clicktale_configuration_activate_configuration($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  $new = $values['configlist'];
  if (empty($new)) {
    drupal_set_message('Please select a configuration. ', 'error');
  }
  else {
    variable_set('clicktale-active-configuration', $new);
    drupal_set_message('The configuration has been activated. ');
  }
}


/*
 * DEBUG - PROCESSING LOG
 */
function _clicktale_configuration_block_debug(&$form) {
  $form['debug'] = array(
      '#type' => 'fieldset',
      '#title' => 'Processing log',
      '#description' => t('The processing log might come in handy to set up your configuration. It will show you wether or not the current page will be counted, and the matching rules if any are applicable.<br />Don\'t forget to turn this off after testing!'));
    
  $form['debug']['show_processing'] = array(
    '#type' => 'radios',
    '#options' => array( t('No'), t('Yes')),
    '#default_value' => _clicktale_show_process());
  
  $form['debug']['debug_s'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_debug'),
    '#value' => 'Apply processing options');
  
  $form['debug']['end'] = array(
    '#type' => 'markup',
    '#value' => '<!-- end -->');
}
function _clicktale_configuration_save_debug($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  variable_set('clicktale-show-debug', $values['show_processing'] ? 1 : 0);
  drupal_set_message("Processing is now " . ($values['show_processing'] ? 'on' : 'off') . ".");
}


/*
 * ROLES
 */
function _clicktale_configuration_block_roles(&$form, $config) {
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => 'Roles',
    '#description' => t('You can track users depending on their user role. Tick off all roles you wish to track.<br />To track a single user you can simply create a new role and add the user to that role. '),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE);
  
  $form['roles']['user_roles'] = array(
    '#type' => 'checkboxes',
    '#value' => $config['roles'],
    '#options' => user_roles());
    
  $form['roles']['save_roles'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_roles'),
    '#value' => 'Track selected roles');
  
  $form['roles']['end'] = array(
    '#type' => 'markup',
    '#value' => '<!-- end -->');
}
function _clicktale_configuration_save_roles($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  $configs[$cid]['roles'] = array();
  foreach ($values['user_roles'] as $rid => $role) {
    if (!empty($role)) {
      $configs[$cid]['roles'][$rid] = $rid;
    } 
  }
  _clicktale_save_configurations($configs);
  drupal_set_message('The selected roles have been saved. ');
}


/*
 * IP ADRESSES
 */
function _clicktale_configuration_block_ip(&$form, $config) {
  $form['ip'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('If the blacklist mode is selected, all IP addresses listed will NOT be counted by ClickTale. When the whitelist mode is active ONLY the addresses listed here will be counted. '),
    '#title' => t('Blacklist/whitelist IP addresses'));
  
  $form['ip']['ip_type'] = array(
    '#type' => 'select',
    '#options' => array('blacklist' => t('Blacklist: count all IP addresses except these'), 'whitelist' => t('Whitelist: only count listed IP addresses and discard all others')),
    '#default_value' => in_array($config['ip-type'], array('blacklist', 'whitelist'))? $config['ip-type'] : 'blacklist');
  
  $form['ip']['ip_type_s'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_ip'),
    '#prefix' => '<div style="position:relative; top:-35px; left:450px; width:100px;">', '#suffix' => '</div>',
    '#value' => 'Save IP type');
  
  if (!empty($config['ips'])) {
    $form['ip']['blacklist_info']['#value'] .= '<p>' . t('There are ' . count($config['ips']) . ' IP addresses listed. ') . '</p>';
    
    $form['ip']['iplist'] = array(
      '#type' => 'checkboxes',
      '#options' => array());
    
    foreach ($config['ips'] as $iid => $ip) {
      $form['ip']['iplist']['#options'][$iid] = $ip;
    }
  
    $form['ip']['remove_blacklisted'] = array(
      '#type' => 'button',
      '#executes_submit_callback' => TRUE,
      '#submit' => array('_clicktale_configuration_save_ip'),
      '#value' => 'Remove selected IP addresses');
  }
  else {
    $form['ip']['no-blacklisted-ips'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('There are no IP addresses in this list.') . '</p>');
  }

  $form['ip']['new_blacklist_ip'] = array(
    '#type' => 'textfield',
    '#description' => t('The IP address or IP address range to list, for example \'192.168.0.15\'.<br />You can use limited regular expressions and wildcard characters, for example \'192.168.0.*\' to add all matching addresses to the list.') .
              '<br />' . t('Your current IP address is ') . '<span style="background-color:#eee;">' . $_SERVER['REMOTE_ADDR'] . '</span>',
      '#required' => FALSE,
    '#title' => t('Add an IP or IP range to the list'));
  
  $form['ip']['save_blacklist_ip'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_ip'),
    '#value' => 'Add IP address');
  
  $form['ip']['end'] = array(
    '#type' => 'markup',
    '#value' => '<!-- end -->');
}
function _clicktale_configuration_save_ip($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  switch ($values['op']) {
    case 'Add IP address':
      $ip = $values['new_blacklist_ip'];
      if (substr($ip, 0, 6) != 'regex:' && !preg_match('/\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.) {3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b/', $ip)) {
        $ip = "regex:$ip";
      }
      $iid = str_replace('.', '-', $ip);
      $configs[$cid]['ips'][$iid] = $ip;
      drupal_set_message('The IP address "' . $ip . '" has been listed.');
      break;
      
    case 'Save IP type':
      $configs[$cid]['ip-type'] = $values['ip_type'];
      drupal_set_message("The IP type has been set to '$values[ip_type]'.");
      break;
      
    case 'Remove selected IP addresses':
      foreach ($values['iplist'] as $k => $v) {
        if (!empty($v)) {
          unset($configs[$cid]['ips'][$k]);
        }
      }
      drupal_set_message('The IP address(es) have been removed from the list. ');
      break;
  }
  _clicktale_save_configurations($configs);
}


/*
 * PAGES
 */
function _clicktale_configuration_block_pages(&$form, $config) {
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('If the blacklist mode is selected, all pages listed will NOT be counted by ClickTale. When the whitelist mode is active ONLY the pages listed here will be counted. '),
    '#title' => t('Blacklist/whitelist pages'));
  
  $form['pages']['page_type'] = array(
    '#type' => 'select',
    '#options' => array('blacklist' => t('Blacklist: count all pages except these'), 'whitelist' => t('Whitelist: only count these pages and discard all others')),
    '#default_value' => in_array($config['page-type'], array('blacklist', 'whitelist'))? $config['page-type'] : 'blacklist');
  
  $form['pages']['page_type_s'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_pages'),
    '#prefix' => '<div style="position:relative; top:-35px; left:450px; width:100px;">', '#suffix' => '</div>',
    '#value' => 'Save page type');
  
  
  
  if (!empty($config['pages'])) {
    $form['pages']['blacklist_info']['#value'] .= '<p>' . t('There are ' . count($config['pages']) . ' listed pages.') . '</p>';
    
    $form['pages']['pagelist'] = array(
      '#type' => 'checkboxes',
      '#options' => array());
    
    foreach ($config['pages'] as $pid => $page) {
      $form['pages']['pagelist']['#options'][$pid] = $page;
    }
  
    $form['pages']['remove_pages'] = array(
      '#type' => 'button',
      '#executes_submit_callback' => TRUE,
      '#submit' => array('_clicktale_configuration_save_pages'),
      '#value' => 'Remove selected pages');
  }
  else {
    $form['pages']['no-listed-pages'] = array(
      '#type' => 'markup',
      '#value' => '<p>' . t('There are no listed pages. ') . '</p>');
  }

  $form['pages']['new_page'] = array(
    '#type' => 'textfield',
    '#description' => t('The Drupal path of the page to list, for example \'node/5485\'.<br />You can use limited regular expressions and wildcard characters, for example \'admin/settings/clicktale*\'.'),
      '#required' => FALSE,
    '#title' => t('Add a page to the list'));
  
  $form['pages']['save_pages'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_pages'),
    '#value' => 'Add page');
  
  $form['pages']['end'] = array(
    '#type' => 'markup',
    '#value' => '<!-- end -->');
}

function _clicktale_configuration_save_pages($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  switch ($values['op']) {
    case 'Add page':
      $page = $values['new_page'];
      if (substr($page, 0, 6) != 'regex:' && preg_match('/[\*()\|\?]/', $page)) {
        $page = "regex:$page";
      }
      $pid = preg_replace('[^a-z0-9_]', '', strtolower(str_replace('/', '_', $page)));
      $configs[$cid]['pages'][$pid] = $page;
      drupal_set_message('The page "' . $page . '" has been listed.');
      break;
    
    case 'Remove selected pages':
      foreach ($values['pagelist'] as $k => $v) {
        if (!empty($v)) {
          unset($configs[$cid]['pages'][$k]);
        }
      }
      drupal_set_message('The page(s) have been removed from the list.');
      break;
      
    case 'Save page type':
      $configs[$cid]['page-type'] = $values['page_type'];
      drupal_set_message("The page type has been set to '$values[page_type]'.");
      break;
  }
  _clicktale_save_configurations($configs);
}


/*
 * CONFIGURATION NAME
 */
function _clicktale_configuration_block_main(&$form, $config, $active) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Configuration name'),
    '#default_value' => $config['name']);
  
  $form['save_name'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#submit' => array('_clicktale_configuration_save_main'),
    '#value' => 'Save');
  
  if ($config['cid'] != 'default' && $config['cid'] != $active) {
    $form['delete'] = array(
      '#type' => 'button',
      '#executes_submit_callback' => TRUE,
      '#submit' => array('_clicktale_configuration_save_main'),
      '#value' => 'Delete');
  }
}

function _clicktale_configuration_save_main($form, &$form_state) {
  extract(_clicktale_configurations_prepare_save($form, $form_state));
  switch ($values['op']) {
    case 'Save':
      $configs[$cid]['name'] = $values['name'];
      _clicktale_save_configurations($configs);
      drupal_set_message('The configuration name has been changed.');
      break;
            
    case 'Delete':
      if ($cid == 'default') {
        drupal_set_message('The default configuration cannot be removed.', 'error');
        drupal_goto('admin/settings/clicktale/configurations/default');
        exit();
      }
      unset($configs[$cid]);
      _clicktale_save_configurations($configs);
      drupal_set_message('The configuration has been removed.');
      drupal_goto('admin/settings/clicktale/configurations');
      exit();
      break;
  }
}

/*
 * Validators
 */
function clicktale_configurations_validate($form, &$form_state) {
  $values = $form_state['values'];
  switch ($values['op']) {
    case 'Create':
      $new = $values['new'];
    case 'Save':
      $new = $new ? $new : $values['name'];
      if (strlen($new) < 3) {
        form_set_error($values['new'] ? 'new' : 'name', 'A configuration name must contain at least 3 characters. ');
      }
      break;
      
    case 'Add page':
      if (strlen($values['new_page']) < 1) {
        form_set_error('new_page', 'A page must contain at least one character!');
      }
      break;
      
    case 'Add IP address':
      $check = preg_replace('/[^0-9\*.\?() {}\|]/i', '', str_replace('regex:', '', $values['new_blacklist_ip']));
      if ($check != str_replace('regex:', '', $values['new_blacklist_ip'])) {
        form_set_error('new_blacklist_ip', 'Please enter a valid IP regex or address');
      }
      break;
  }
}

/*
 * Prepare variables for a block save handler
 */
function _clicktale_configurations_prepare_save($form, &$form_state) {
  $values = $form_state['values'];
  $cid = $values['cid'];
  $configs = _clicktale_get_configurations();
  $config = $configs[$cid];
  return compact('values', 'cid', 'configs', 'config');
}




/*
 * Saves all configurations
 */
function _clicktale_save_configurations($configs) {
  variable_set('clicktale-configurations', $configs);
}

/*
 * Retrieves an array of configurations
 */
function _clicktale_get_configurations() {
  $default = _clicktale_get_default_config();
  $configs = variable_get('clicktale-configurations', array('default' => $default));
  if (is_array($configs)) {
    $data = array();
    foreach ($configs as $name => $config) {
      if (is_array($config)) {
        $data[$name] = array_merge($default, $config); // make sure we have all keys
      }
    }
  }
  if (empty($data)) {
    return array('default' => $default);
  }
  return $data;
}

/*
 * Returns a boolean, wether or not to activate tracking for the current pageview.
 */
function _clicktale_track_for_user() {
  global $user;
  $debug = _clicktale_show_process(); $debugmessages = array();
  $active = variable_get('clicktale-active-configuration', 'default');
  $configs = _clicktale_get_configurations();
  $config = $configs[$active] ? $configs[$active] : $configs['default'];
  $page = $_GET['q'];
  $ip = $_SERVER['REMOTE_ADDR'];
  $allowed = array('pages' => TRUE, 'ips' => TRUE, 'roles' => TRUE);
  
  $page_blacklist = ($config['page-type'] == 'blacklist') ? TRUE : FALSE;
  $ip_blacklist = ($config['ip-type'] == 'blacklist') ? TRUE : FALSE;
  
  // check blacklisted/whitelisted pages
  if (is_array($config['pages'])) {
    $allowed['pages'] = $page_blacklist;
    if (in_array($page, $config['pages'])) {
      if ($debug) $debugmessages[] = "Page list match on '$page'.";
      $allowed['pages'] = $page_blacklist ? FALSE : TRUE;
    }
    if (($allowed['pages'] && $page_blacklist) || (!$allowed['pages'] && !$page_blacklist)) {
      foreach ($config['pages'] as $path) {
        if (substr($path, 0, 6) == 'regex:') {
          $regex = strtr($path, array(
            '/' => '\\/',
            'regex:' => ''));
          if (preg_match("/$regex/i", $page)) {
            if ($debug) $debugmessages[] = "Page list match on '$page'(regex).";
            $allowed['pages'] = $page_blacklist ? FALSE : TRUE;
            if (($allowed['pages'] && !$page_blacklist) || (!$allowed['pages'] && $page_blacklist)) {
              break;
            }
          }
        }
      }
    }
  }
  
  // check blacklisted/whitelisted ips
  if (is_array($config['ips'])) {
    $allowed['ips'] = $ip_blacklist;
    if (in_array($ip, $config['ips'])) {
      if ($debug) $debugmessages[] = "Listed IP match on '$ip'.";
      $allowed['ips'] = $ip_blacklist ? FALSE : TRUE;
    }
    if (($allowed['ips'] && $ip_blacklist) || (!$allowed['ips'] && !$ip_blacklist)) {
      foreach ($config['ips'] as $aipee) {
        if (substr($aipee, 0, 6) == 'regex:') {
          $regex = strtr($aipee, array(
            'regex:' => ''));
          if (preg_match("/$regex/i", $ip)) {
            if ($debug) $debugmessages[] = "Listed IP match on '$ip'(regex).";
            $allowed['ips'] = $ip_blacklist ? FALSE : TRUE;
            if (($allowed['ips'] && !$ip_blacklist) || (!$allowed['ips'] && $ip_blacklist)) {
              break;
            }
          }
        }
      }
    }
  }
  
  
  // check roles
  if (is_array($config['roles'])) {
    foreach ($user->roles as $rid => $role) {
      if (in_array($rid, $config['roles'])) {
        $allow_role = TRUE;
      }
    }
    if (!$allow_role) {
      if ($debug) $debugmessages[] = "None of your roles are accepted by Clicktale. (" . implode(',', $user->roles) . ").";
      $allowed['role'] = FALSE;
    }
  }
  
  // calculate allowed
  //if ($debug) $debugmessages[] = "Allowed: " .var_export($allowed, TRUE) ." .";
  $allowed = in_array(FALSE, $allowed) ? FALSE : TRUE;
  
  // count this user
  if ($debug) {
    $debugmessages[] = $allowed ? "Clicktale has <b>registered</b> the pageview." : "Clicktale has <b>discarded</b> the pageview.";
    drupal_set_message('<b>Clicktale processing:</b><ul><li>' . implode('</li><li>', $debugmessages) . '</li></ul>');
  }
    
  return $allowed;
}

/*
 * Returns an array of configuration ID's and URL's
 */
function _clicktale_get_configurations_options($config) {
  $url = 'http://' . $_SERVER['HTTP_HOST'] . request_uri();
  $url = substr($url, 0, strpos($url, 'settings/clicktale/configurations') + strlen('settings/clicktale/configurations'));
  foreach ($config as $id => $c) {
    if (!empty($id) && !empty($c)) {
      $output[$id] = $c['name'];
    }
    //$output .= "<option value=\"$url/$id\">$c[name]</option>";
  }
  return $output;
}

/*
 * Wether or not to show the debug variables. Usefull when setting rules.
 */
function _clicktale_show_process() {
  return variable_get('clicktale-show-debug', 0);
}

/*
 * Creates a default config. All configurations are based on this.
 */
function _clicktale_get_default_config() {
  return array('name' => 'Default', 
          'cid' => 'default', 
          'pages' => array(), 
          'ips' => array(), 
          'page-type' => 'blacklist',
          'ip-type' => 'blacklist',
          'roles' => array_keys(user_roles()));
}