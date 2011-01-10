<?php
// $Id$

/*
 * @file Settings form to configure your clicktale account
 */

function clicktale_admin_settings_form() {
  $form = array('#submit' => array('clicktale_admin_settings_submit'));
  
  // check requirements, especially PHP Integrations installation
  // maybe this shouldn't be here but its the best way of telling if everything is working or not.
  require_once('clicktale.install');
  $requirements = clicktale_requirements('runtime');
  if (is_array($requirements)) {
  	foreach ($requirements as $req_key => $req_meta) {
  	  if ($req_meta['severity'] == REQUIREMENT_WARNING) {
        drupal_set_message($req_meta['description'], 'warning');
  	  }
  	  elseif ($req_meta['severity'] == REQUIREMENT_ERROR) {
        drupal_set_message($req_meta['description'], 'error');
  	  }
  	}
  }
  //drupal_set_message(var_export($requirements, true));
  
  if ($xml = $_SESSION['clicktale-xml']) {
    unset($_SESSION['clicktale-xml']);
       
  $phpint_path = variable_get('ClickTale_PHP_Kit', NULL);
  
    $form['clicktale_manualxml'] = array(
      '#type' => 'markup',
      '#value' => t("Use the XML below to manually create the file '$phpint_path/ClickTaleScripts.xml', or make this file writable and save your settings again.") . '<textarea style="width:100%;" rows="15">' . $xml . '</textarea>');
  }
  
  $form['clicktale_info'] = array(
    '#type' => 'markup',
    '#value' => t('Clicktale will most likely only work in Mozilla Firefox and Internet Explorer, and is not supported by Google Chrome.'));
  
  $form['clicktale_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Clicktale ID'),
    '#default_value' => variable_get('clicktale_id', ''),
    '#size' => 10,
    '#description' => t('You\'ll get this number on registering your project.'),
    '#required' => FALSE,
  );
  $form['clicktale_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Clicktale Ratio'),
    '#default_value' => variable_get('clicktale_ratio', '0.1'),
    '#size' => 3,
    '#description' => t('The amount of visits you wish to track (1 = 100%)'),
    '#required' => FALSE,
  );
  $form['clicktale_partition'] = array(
    '#type' => 'textfield',
    '#title' => t('The Clicktale project number'),
    '#default_value' => variable_get('clicktale_partition', 'www'),
    '#size' => 30,
    '#description' => t('The partition you wish to track. If you don\'t know this, just leave it as is'),
    '#required' => FALSE,
  );
  
  $form['clicktale_info2'] = array(
    '#type' => 'markup',
    '#value' => '<hr><br />' . t('If one of the fields below is not empty the information entered in the fields above will not be used.<br />Leave these fields empty if you wish to use the data entered above!'));
  $form['clicktale_top'] = array(
    '#type' => 'textarea',
    '#title' => t('The Clicktale Javascript top code'),
    '#default_value' => variable_get('clicktale_top', ''),
    '#required' => FALSE,
  );
  $form['clicktale_bottom'] = array(
    '#type' => 'textarea',
    '#title' => t('The ClickTale Javascript bottom code'),
    '#default_value' => variable_get('clicktale_bottom', ''),
    '#required' => FALSE,
  );
  return system_settings_form($form);
}


function clicktale_admin_settings_submit(&$form) {
  $default_top = '<!-- ClickTale Top part -->
<script type="text/javascript">
var WRInitTime=(new Date()).getTime();
</script>
<!-- ClickTale end of Top part -->';
  
  $default_bottom = '<!-- ClickTale Bottom part -->
<div id="ClickTaleDiv" style="display: none;"></div>
<script type="text/javascript">
if (document.location.protocol!=\'https:\')
  document.write(unescape("%3Cscript src=\'http://s.clicktale.net/WRb.js\' type=\'text/javascript\'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
ClickTaleFetchFrom="%FetchFromUrl%";
if (typeof ClickTale==\'function\') ClickTale(%clicktale-id%,%clicktale-ratio%,"%clicktale-number%");
</script>
<!-- ClickTale end of Bottom part -->';
  
  $values = $form['clicktale_id']['#post'];
  $top = $values['clicktale_top'];
  $bottom = $values['clicktale_bottom'];
  
  if (empty($bottom) || empty($top)) {
    $bottom = strtr($default_bottom, array(
      '%clicktale-id%' => $values['clicktale_id'],
      '%clicktale-ratio%' => $values['clicktale_ratio'],
      '%clicktale-number%' => $values['clicktale_partition']));
    $top = $default_top;
  }
  else{
    // add extra line;
    if (strpos($bottom, 'ClickTaleFetchFrom="%FetchFromUrl%";') === FALSE) {
      $bottom = preg_replace('/<script([^>]*)>/', '<script$1>ClickTaleFetchFrom="%FetchFromUrl%";', $bottom);
    }
  }
  
  $phpint_path = variable_get('ClickTale_PHP_Kit', NULL);
  if ($phpint_path === NULL) {
    drupal_set_message('PHP Integration kit could not be found.', 'error');
    return;
  }
  
  $filepath = realpath(file_directory_path()) . '/clicktale/ClickTaleScripts.xml';
  if (!file_exists($filepath)) {
  	return drupal_set_message("Cannot save the settings in PHP Integrations ClickTaleScripts.xml because the file does not exists. Please copy the file from the PHP Integrations directory in the library to '$filepath'.", 'warning');
  }
  
  $vars = clicktale_xml_read($filepath);
  $vars['Top']['value'] = $top;
  $vars['Bottom']['value'] = $bottom;
  
  if ($xml = clicktale_xml_write($vars, $filepath)) {
    $_SESSION['clicktale-xml'] = $xml;
    drupal_set_message("Could not save variables in XML format in file '$filepath'.<br />Please check the rights to this file and save these settings again, or copy the XML below and create the file manually.", 'warning');
  }
}

function clicktale_xml_write($vars, $file) {
  $xml = simplexml_load_file($file);
  $j = 0;
  foreach ($xml->script as $script) {
    $name = (string)$script['name'];
    $val = $vars[$name]['value'];
    $xml->script[$j] = 'CDATA_START' . $val . 'CDATA_END';//"<![CDATA[$val]]>";//$script;
    $j++;
  }
  //return $xml->asXML($file);
  
  /* cdata stuff - it's crap */
  $the_xml = str_replace('&#13;', "", $xml->asXML());
  while ($s = strpos($the_xml, 'CDATA_START')) {
    $e = strpos($the_xml, 'CDATA_END', $s);
    $c = substr($the_xml, $s, $e - $s + 9);
    $c = strtr($c, array('CDATA_START' => '<![CDATA[', 'CDATA_END' => ']]>', '&lt;' => '<', '&gt;' => '>'));
    $the_xml = substr($the_xml, 0, $s) . $c . substr($the_xml, $e + 9);
  }
  
  if (!$fh = @fopen($file, 'w+')) {
    return $the_xml;
  }
  $result = fwrite($fh, $the_xml);
  fclose($fh);
  return $result ? FALSE : $the_xml;
}

function clicktale_xml_read($file) {
  $xml = simplexml_load_file($file);
  $vars = array();
  foreach ($xml->script as $script) {
    $item = array();
    foreach ($script->attributes() as $attr => $attrval) {
      $item[(string)$attr] = (string)$attrval;
    }
    $item['value'] = (string)$script;
    $vars[$item['name']] = $item;
  }
  return $vars;
}
