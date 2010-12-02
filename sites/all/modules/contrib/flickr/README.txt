Name:
----
    flickr http://drupal.org/project/flickr
    (Unfinished/Incomplete)

Requirements:
------------
This module requires Drupal 6.X and a Flickr API key.

Overview:
--------
Flickr adds a filter to allow photos from http://flickr.com to be
integrated into your site via flickr's API. There are several Flickr blocks
that you can enable as well.

Installation and configuration:
------------------------------
1.) Simply extract the download package in your modules directory '/modules'
    or '/sites/all/modlues'.
2.) Enable the module in 'admin/build/modules/'.
3.) Configure the API Key '/admin/settings/flickr'.
4.) Add the flickr filter to one of your input formats by following
the configure link 'admin/settings/filters'.
5.) Allow permissions '/admin/user/access'.

Usage:
------
The filter format is: [flickr-photo:id=230452326,size=s] and 
[flickr-photoset:id=72157594262419167,size=m]

The size parameter can be one of the following:
  * s - small square 75x75
  * t - thumbnail, 100 on longest side
  * m - small, 240 on longest side
  * - - medium, 500 on longest side
  * b - large, 1024 on longest side
  * o - original image

