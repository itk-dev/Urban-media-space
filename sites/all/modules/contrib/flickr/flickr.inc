<?php

define('FLICKR_REST_ENDPOINT', 'https://api.flickr.com/services/rest/');
require_once(drupal_get_path('module', 'flickr') .'/flickr.api.inc');

/**
 * Submit a request to Flickr.
 *
 * @param $method
 *   string method name
 * @param $args
 *   associative array of arguments names and values
 * @param $cacheable
 *   boolean indicating if it's safe to cache the results of this request
 * @param $return_errors
 *   boolean indicating if the caller will handle displaying error messages
 *
 * @return
 *   an array with the the result of the request, or FALSE on error.
 */
function flickr_request($method, $args, $cacheable = TRUE, $return_errors = FALSE) {
  // Add in additional parameters then sort them for signing.
  $args['api_key'] = trim(variable_get('flickr_api_key', ''));
  $args['method'] = $method;
  $args['format'] = 'php_serial';
  ksort($args);

  // Build an argument hash API signing (we'll also use it for the cache id)
  $arg_hash = '';
  foreach ($args as $k => $v) {
    $arg_hash .= $k . $v;
  }
  // If we've got a secret, sign the arguments.
  if ($secret = trim(variable_get('flickr_api_secret', ''))) {
    $args['api_sig'] = md5($secret . $arg_hash);
  }

  // Build the URL.
  foreach ($args as $k => $v){
    $encoded_params[] = urlencode($k) . '=' . urlencode($v);
  }
  $url = FLICKR_REST_ENDPOINT .'?'. implode('&', $encoded_params);  

  // If it's a cachable request, try to load a cached value.
  if ($cacheable) {
    if ($cache = cache_get("flickr_$arg_hash", 'cache')) {
      // Check that the value is still "fresh".
      if( $cache->expire > time() ) {
        return unserialize($cache->data);
      }
    }
  }

  // If a cached value wasn't suitable, attempt to connect and fetch a result.
  $result = drupal_http_request($url);
  if ($result->code != 200) {
    if ($return_errors) {
      return array(
        'stat' => 'error',
        'code' => $result->code,
        'message' => $result->error,
      );
    }
    flickr_set_error(t("Could not connect to Flickr, Error: @error", array('@error' => $result->error)));
    return FALSE;
  }

  // Make sure it unserializes.
  $response = unserialize($result->data);
  if (!$response) {
    if ($return_errors) {
      return array(
        'stat' => 'error',
        'code' => '-1',
        'message' => 'The response was corrupted, it could not be unserialized.',
      );
    }
    flickr_set_error(t("Flickr's response was corrupted and could not be unserialized."));
    return FALSE;
  }

  // Check that the request was successful.
  if (flickr_response_has_error($response)) {
    if ($return_errors) {
      return $response;
    }
    flickr_set_error($response);
    return FALSE;
  }

  // Save cacheable results for future use.
  if ($cacheable) {
    cache_set("flickr_$arg_hash", $result->data, 'cache', time() + variable_get('flickr_cache_duration', 3600));
  }

  return $response;
}

/**
 * This function will try to create a html image tag referencing the Flickr
 * photo with the desired size if that size is available for this photo.
 *
 * @param $photo
 *   the photo variable
 * @param $size
 *   the desired image size
 * @param $attribs
 *   an optional array of HTML attributes to pass to the image
 *
 * @return
 *   a html image tag referencing the image of the desired
 *   size if it is available.
 */
function flickr_img($photo, $size = NULL, $attributes = NULL) {
  $sizes = flickr_photo_sizes();
  if (!isset($size)) {
    $size = '-';
  }
  if (!isset($sizes[$size])) {
    return;
  }
  if (!isset($attributes) || !is_array($attributes)) {
    $attributes = array();
  }
  if (empty($attributes['class'])) {
    $attributes['class'] = NULL;
  }

  // photoset's use primary instead of id to specify the image.
  if (isset($photo['primary'])) {
    $id = $photo['primary'];
    $attributes['class'] = implode(' ', array($attributes['class'], 'flickr-photoset-img'));
  }
  else {
    $id = $photo['id'];
    $attributes['class'] = implode(' ', array($attributes['class'], 'flickr-photo-img'));
  }

  if ($size == 's') {
    $attributes['height'] = $attributes['width'] = 75;
    $img_url = flickr_photo_img($photo, $size);
  }
  else {
    $image_sizes = flickr_photo_get_sizes($id);
    if ($image_sizes) {
      foreach ($image_sizes as $image_size) {
        if ($image_size['label'] == $sizes[$size]['label']) {
          break;
        }
      }
      if (isset($image_size)) {
        $img_url = $image_size['source'];
        $attributes['height'] = $image_size['height'];
        $attributes['width'] = $image_size['width'];
      }
    }
    else {
      $img_url = flickr_photo_img($photo, $size);
    }
  }
  $title = is_array($photo['title']) ? $photo['title']['_content'] : $photo['title'];
  return theme('image', $img_url, $title, $title, $attributes, FALSE);
}

/**
 * Create the url to $photo with size $size using the correct image farm
 * from the $photo variable
 *
 * @param $photo
 *   photo to which the url should point
 * @param $size
 *   size of the photo
 * @param $format
 *   format of the photo
 *
 * @return
 *   url for $photo with the correct size and format
 */
function flickr_photo_img($photo, $size = NULL, $format = NULL) {
  // early images don't have a farm setting so default to 1.
  $farm = isset($photo['farm']) ? $photo['farm'] : 1;
  $server = $photo['server'];
  // photoset's use primary instead of id to specify the image.
  $id = isset($photo['primary']) ? $photo['primary'] : $photo['id'];
  $secret = $photo['secret'];

  return "http://farm{$farm}.static.flickr.com/{$server}/{$id}_{$secret}" .($size ? "_$size." : '.') . ($size == 'o' ? $format : 'jpg');
}

/**
 * Return a list of all possible photo sizes with the right
 * description and label
 *
 * @return An array of photo sizes
 */
function flickr_photo_sizes() {
  return array(
    's' => array(
      'label' => 'Square',
      'description' => t('75 px square'),
    ),
    't' => array(
      'label' => 'Thumbnail',
      'description' => t('100 px on longest side'),
    ),
    'q' => array(
      'label' => 'Large Square',
      'description' => t('150 px square'),
    ),
    'm' => array(
      'label' => 'Small',
      'description' => t('240 px on longest side'),
    ),
    'n' => array(
      'label' => 'Small 320',
      'description' => t('320 px on longest side'),
    ),
    '-' => array(
      'label' => 'Medium',
      'description' => t('500 px on longest side'),
    ),
    'z' => array(
      'label' => 'Medium 640',
      'description' => t('640 px on longest side'),
    ),
    'c' => array(
      'label' => 'Medium 800',
      'description' => t('800 px on longest side'),
    ),
    'b' => array(
      'label' => 'Large',
      'description' => t('1024 px on longest side'),
    ),
    'h' => array(
      'label' => 'Large 1600',
      'description' => t('1600 px on longest side'),
    ),
    'k' => array(
      'label' => 'Large 2048',
      'description' => t('2048 px on longest side'),
    ),
    'o' => array(
      'label' => 'Original',
      'description' => t('Original image')
    ),
  );
};

/**
 * @param $owner
 *   owner of the photo
 * @param $id
 *   id of the photo to reference in the url
 *
 * @return
 *   url for the flickr photo page showing photo with $id of $owner
 */
function flickr_photo_page_url($owner, $id = NULL) {
  $nsid = is_array($owner) ? $owner['nsid']: $owner;
  if ($person = flickr_people_get_info($nsid)) {
    return $person['photosurl']['_content'] . $id;
  }
  else {
    return "http://flickr.com/photos/$nsid/$id";
  }
}

/**
 * @param $owner
 *   owner of the photoset
 * @param $id
 *   id of the photoset to which the url must lead
 *
 * @return
 *   url for the photoset page of photoset $id of owner $owner
 */
function flickr_photoset_page_url($owner, $id = NULL) {
  $nsid = is_array($owner) ? $owner['nsid']: $owner;
  if ($person = flickr_people_get_info($nsid)) {
    return $person['photosurl']['_content'] .'sets/'. $id;
  }
  else {
    return "http://flickr.com/photos/$nsid/sets/$id";
  }
}

/**
 * Tries to match an 'identifier' onto a flickr nsid
 *
 * This function will first see whether $identifier is already
 * a nsid (format check only, no api call).  If it is not and the
 * identifier has the format of an email, an api call will be made to
 * check whether there is an nsid for that email.  If this is not the
 * case, the $identifier is treated as a username and an api call is
 * made to find the nsid for that username.
 *
 * If none of these succeed, the result will be false
 *
 * @param $identifier
 *   identifier to find an nsid for
 *
 * @return
 *   valid nsid or false if none can be found
 */
function flickr_user_find_by_identifier($identifier) {
  if (flickr_is_nsid($identifier)) {
    //identifier is an NSID
    return $identifier;
  }
  if (valid_email_address($identifier) && ($user = flickr_user_find_by_email($identifier))) {
    return $user['nsid'];
  }
  if ($user = flickr_user_find_by_username($identifier)) {
    return $user['nsid'];
  }

  return FALSE;
}

function flickr_is_nsid($id) {
  return preg_match('/^\d+@N\d+$/', $id);
}



function flickr_tag_request_args($tags = array(), $mode = 'all') {
  $args = array();
  if (!empty($tags)) {
    $args['tags'] = implode(',', $tags);
    $args['tag_mode'] = $mode == 'all' ? $mode : 'any';
  }
  return $args;
}

/**
 * Check if the response from the Flickr api call was an error
 *
 * @param $response
 *   response to check
 *
 * @return
 *    true if the response is an error message
 */
function flickr_response_has_error($response) {
  return !(isset($response['stat']) && $response['stat'] == 'ok');
}

/**
 * Display an error message to flickr administrators and write an error to
 * watchdog.
 *
 * @param $error_message
 *   error message to display
 */
function flickr_set_error($error_message) {
  if (is_array($error_message)) {
    $message = t('Flickr error @error_id: %flickr_error', array(
      '@error_id' => $error_message['code'],
      '%flickr_error'=> $error_message['message'],
    ));
  }
  else {
    $message = $error_message;
  }

  if (user_access('administer flickr')) {
    drupal_set_message($message, 'error');
  }
  watchdog('flickr', $message, array(), WATCHDOG_WARNING);
}

