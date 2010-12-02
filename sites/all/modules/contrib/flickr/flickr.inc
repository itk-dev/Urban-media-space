<?php
// $Id: flickr.inc,v 1.31.2.3 2009/03/17 21:56:54 paulbooker Exp $

define('FLICKR_REST_ENDPOINT', 'http://api.flickr.com/services/rest/');

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
      'description' => t('75x75 pixel square'),
    ),
    't' => array(
      'label' => 'Thumbnail',
      'description' => t('100 pixels on longest side'),
    ),
    'm' => array(
      'label' => 'Small',
      'description' => t('240 pixels on longest side'),
    ),
    '-' => array(
      'label' => 'Medium',
      'description' => t('500 pixels on longest side'),
    ),
    'b' => array(
      'label' => 'Large',
      'description' => t('1024 pixels on longest side'),
    ),
    'o' => array(
      'label' => 'Original',
      'description' => t('Original image')
    ),
  );
};

/**
 * Submit a request to Flickr.
 *
 * @param $method
 *   string method name
 * @param $args
 *   associative array of arguments names and values
 * @param $cacheable
 *   boolean indicating if it's safe cache the results of this request
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
    $encoded_params[] = urlencode($k) .'='. urlencode($v);
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
        //In Drupal <= 5.1, only HTTP errors are stored in $result->code correctly, not TCP/IP errors.
        //We can not count on this variable being correct until this module requires Drupal 5.2 or above.
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
 * @param $photo_id
 *   id of the photo to get info about
 *
 * @return
 *   response from the flickr method flickr.photos.getInfo
 *   (http://www.flickr.com/services/api/flickr.photos.getInfo.html)
 */
function flickr_photo_get_info($photo_id) {
  $response = flickr_request(
    'flickr.photos.getInfo',
    array('photo_id' => $photo_id)
  );
  if ($response) {
    return $response['photo'];
  }
  return FALSE;
}

/**
 * @param $photo_id
 *   id of the photo to get the available sizes of
 *
 * @return
 *   response from the flickr method flickr.photos.getSizes
 *   (http://www.flickr.com/services/api/flickr.photos.getSizes.html)
 */
function flickr_photo_get_sizes($photo_id) {
  $response = flickr_request(
    'flickr.photos.getSizes',
    array('photo_id' => $photo_id)
  );
  if ($response) {
    return $response['sizes']['size'];
  }
  return FALSE;
}

/**
 * @param $photoset_id
 *   id of the photoset to get information about
 *
 * @return
 *   response from the flickr method flickr.photosets.getInfo
 *   (http://www.flickr.com/services/api/flickr.photosets.getInfo.html)
 */
function flickr_photoset_get_info($photoset_id) {
  $response = flickr_request(
    'flickr.photosets.getInfo',
    array('photoset_id' => $photoset_id)
  );
  if ($response) {
    return $response['photoset'];
  }
  return FALSE;
}

/**
 * @param $nsid
 *   nsid of the user whose photoset list you want
 *
 * @return
 *   response from the flickr method flickr.photosets.getList
 *   (http://www.flickr.com/services/api/flickr.photosets.getList.html)
 */
function flickr_photoset_get_list($nsid) {
  $response = flickr_request(
    'flickr.photosets.getList',
    array('user_id' => $nsid)
  );
  if ($response) {
    return $response['photosets']['photoset'];
  }
  return FALSE;
}

/**
 * @param $nsid The Flickr user's NSID
 *
 * @return
 *   array with person's info from flickr.people.getInfo
 *   (http://www.flickr.com/services/api/flickr.people.getInfo.html)
 *   or FALSE on error.
 */
function flickr_people_get_info($nisd) {
  $response = flickr_request(
    'flickr.people.getInfo',
    array('user_id' => $nisd)
  );
  if ($response) {
    return $response['person'];
  }
  return FALSE;
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

/**
 * Lookup an nsid for a username.
 *
 * @param $username
 *   username to look for
 *
 * @return
 *   response from the flickr method flickr.people.findByUsername
 *   (http://www.flickr.com/services/api/flickr.people.findByUsername.html)
 */
function flickr_user_find_by_username($username) {
  $response = flickr_request(
    'flickr.people.findByUsername',
    array('username' => $username)
  );
  if ($response) {
    return $response['user'];
  }
  return FALSE;
}

/**
 * Lookup an nsid for an email address
 *
 * @param $email
 *   email to look for
 *
 * @return
 *   response from the flickr method flickr.people.findByEmail
 *   (http://www.flickr.com/services/api/flickr.people.findByEmail.html)
 */
function flickr_user_find_by_email($email) {
  $response = flickr_request(
    'flickr.people.findByEmail',
    array('find_email' => $email)
  );
  if ($response) {
    return $response['user'];
  }
  return FALSE;
}

/**
 * @param $nsid
 *   nsid of the user whose tags will be returned
 * @param $count
 *   number of tags to return
 *
 * @return
 *   response from the flickr method flickr.tags.getListUserPopular
 *   (http://www.flickr.com/services/api/flickr.tags.getListUserPopular.html)
 */
function flickr_tags_get_list_user_popular($nsid, $count=NULL) {
  $args = array('user_id'=>$nsid);
  if ($count != NULL) {
    $args['count'] = $count;
  }
  $response = flickr_request('flickr.tags.getListUserPopular', $args);
  if ($response) {
    return $response['who']['tags']['tag'];
  }
  return FALSE;
}

/**
 * @param $nsid
 *   nsid of the user whose photoset tags will be returned
 *
 * @return
 *   response from the flickr method flickr.tags.getListUser
 *   (http://www.flickr.com/services/api/flickr.tags.getListUser.html)
 */
function flickr_tags_get_list_user($nsid){
  $response = flickr_request(
    'flickr.tags.getListUser',
    array('user_id' => $nsid)
  );
  if ($response) {
    return $response['who']['tags']['tag'];
  }
  return FALSE;
}


function flickr_photos_search($nsid, $page = 1, $other_args = array()){
  $args = array (
    'user_id' => $nsid,
    'page' => $page,
  );

  //set per_page to flickr module default if it is not specified in $other_args
  if (!isset($other_args['per_page'])) {
    $args['per_page'] = variable_get('flickr_photos_per_page', 20);
  }

  $response = flickr_request('flickr.photos.search', array_merge($args, $other_args));
  if ($response) {
    return $response['photos'];
  }
  return FALSE;
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
 * @param $mesageOrResposne
 *   message or error response to display
 */
function flickr_set_error($messageOrResponse) {
  if (is_array($messageOrResponse)) {
    $message = t('Flickr error @error_id: %flickr_error', array(
      '@error_id' => $messageOrResponse['code'],
      '%flickr_error'=> $messageOrResponse['message'],
    ));
  }
  else {
    $message = $messageOrResponse;
  }

  if (user_access('administer flickr')) {
    drupal_set_message($message, 'error');
  }
  watchdog('flickr', $message, array(), WATCHDOG_WARNING);
}

/**
 * @param $nid
 *   nsid of the group whose photos you want
 *
 * @return
 *   response from the flickr method flickr.photosets.getList
 *   (http://www.flickr.com/services/api/flickr.photosets.getList.html)
*/
function flickr_get_group_photos($nsid, $page = 1, $other_args = array()){
  $args = array (
    'group_id' => $nsid,
    'page' => $page,
  );

  //set per_page to flickr module default if it is not specified in $other_args
  if (!isset($other_args['per_page'])) {
    $args['per_page'] = variable_get('flickr_photos_per_page', 20);
  }

  $response = flickr_request('flickr.groups.pools.getPhotos', array_merge($args, $other_args));
  if ($response) {
    return $response['photos'];
  }
  return FALSE;
}