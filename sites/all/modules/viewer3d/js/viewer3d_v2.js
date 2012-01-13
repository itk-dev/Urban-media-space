
var viewer3dObj;

function viewer3d() {
  // Settings varibles.
  this.settings = '';
  this.route = '';
  this.currentPoint = '';
  this.titlesLoaded = false;

  // The flash viewer.
  this.viewer = '';

  /**
   * Init the flash viewer and load basic settings. Most be the first function
   * to be called after object creation.
   */
  this.init = function() {
    // Get information from Drupal.
    this.settings = Drupal.settings.viewer3d;
    this.route = Drupal.settings.viewer3d_route;

    // Set start point to current point.
    this.currentPoint = this.settings['currentLocation'];

    // Load the flash onto the page.
    swfobject.embedSWF(this.settings['flash_location']+'/swf_viewer/BuildingViewer.swf',
                     "building-viewer",
                     "940",
                     "666",
                     "10.0.22",
                     this.settings['flash_location']+"/scripts/expressInstall.swf",
                     {width:'940',DaluxBuildingViewServerURL:this.settings['data']+'&OverviewURL='+this.settings['overviewURL']+'&currentLocation='+this.settings['currentLocation']+'&angle='+this.settings['angle']+'&angle2='+this.settings['angle2']+'&showTopBar='+this.settings['showTopBar']+'&showLog='+this.settings['showLog']+'&informationPoints='+this.settings['infoPoints']+'&markerSize='+this.settings['markerSize']+'&markerMinSize='+this.settings['markerMinSize']+'&rotationSpeed='+this.settings['rotationSpeed']+'&mouseOutsideFlash=true'},
                     {allowFullScreen:"true", allowScriptAccess:"sameDomain", wmode: 'opaque'});
  };

  /**
   * Helper function that gets the flash application.
   */
  this.getApp = function() {
    if (this.viewer == '') {
      var app = this.getMovie('building-viewer');
      if (!app) {
        app = this.getObject('building-viewer');
        if (!app) {
          throw "Flash application not found";
        }
      }
      // Found it, so store it for the next time.
      this.viewer = app;
    }
    return this.viewer;
  }

  /**
   * Helper function that tries to get the flash object by tag.
   */
  this.getObject = function(obj) {
    if (document.all) {
      if (typeof obj == "string") {
        return document.all(obj);
      } else {
        return obj.style;
      }
    }
    if (document.getElementById) {
      if (typeof obj == "string") {
        return document.getElementById(obj);
      } else {
        return obj.style;
      }
    }
    return null;
  }

  /**
   * Helper function that tries to get the flash movie.
   */
  this.getMovie = function(movieName) {
    // IE and Netscape refer to the movie object differently.
    // This function returns the appropriate syntax depending on the browser.
    var IE = /*@cc_on!@*/false
    if (IE) {
      return window[movieName];
    } else {
      return window.document[movieName];
    }
  }

  /**
   * Goto the next point in the route selected in the backend configuration in
   * Drupal.
   */
  this.nextPoint = function() {
    if (this.route[this.currentPoint].next.jump) {
      this.gotoLocationDefaultDirection(this.route[this.currentPoint].next.pid);
    }
    else {
      this.flyToLocation(this.route[this.currentPoint].next.pid);
    }
  }

  /**
   * Goto the previous point in the route selected in the backend configuration
   * in Drupal.
   */
  this.prevPoint = function()  {
    if (this.route[this.currentPoint].prev.jump) {
      this.gotoLocationDefaultDirection(this.route[this.currentPoint].prev.pid);
    }
    else {
      this.flyToLocation(this.route[this.currentPoint].prev.pid);
    }
  }

  /**
   * Toogle drakend overlay.
   */
  this.toggleOverlay = function() {
    $('.building-viewer-overlay')
      .css('opacity', .5)
      .toggle(50, function() {});
  }

  /**
   * Toogle help overlay.
   */
  this.toggleHelp = function() {
    $('#building-viewer-help').toggle();
    this.toggleOverlay();
  }

  /**
   * Load point titles into the flash model.
   */
  this.loadLabels = function() {
    if (!this.titlesLoaded) {
      var titles = this.settings.titles;
      var ids = [];
      var strs = [];
      for ( id in titles ) {
        ids.push(id);
        strs.push(titles[id]);
      }
      this.getApp().setLabels(ids, strs);
      this.titlesLoaded = true;
    }
  }

  /**
   * Takes snapshot of the current image displayed in viewer and send it back to
   * the server, which encodes it to an downloadable png image.
   */
  this.snapshot = function() {
    var imageBase64 = this.getApp().snapshot();
    $.post(this.settings.path + '/download', {data: imageBase64}, function(data) {
      window.location = data;
    });
  }

  /**
   * Goto a point and look in the direction defined by the angle parameteres.
   */
  this.gotoLocation = function(pos, angle, angle2) {
    this.getApp().gotoLocation(pos, angle, angle2);
  }

  /**
   * Goto a point and look in the default direction, which is defined in the
   * point editor.
   */
  this.gotoLocationDefaultDirection = function(pos) {
    this.getApp().gotoLocationDefaultDirection(pos);
  }

  /**
   * Fly to a point (animation) requires that there are a connection between the
   * current point and the fly-to-point.
   */
  this.flyToLocation = function(pos) {
    this.getApp().flyToLocation(pos);
  }

  /**
   * Rotate to the default direction for the current point.
   */
  this.rotateToDefaultDirection = function() {
    this.getApp().rotateToDefaultDirection();
  }

  /**
   * Creates a new jump point, which we are not using.
   */
  this.createJumpPoint = function (jumpName, pos, angle1, angle2) {
    this.getApp().createJumpPoint(jumpName, pos, angle1, angle2);
  }

  /**
   * Creates a new jump point, but loads the view to the point when selected. We
   * do not use this function.
   */
  this.createLoadPoint = function(jumpName, url) {
    this.getApp().createLoadPoint(jumpName, url);
  }

  /**
   * Set the label for a given point.
   **/
  this.setPointLabel = function(point, label) {
    this.getApp().setPointLabel(point, label);
  }

  /**
   * Gets the url for jumping into the viewer at the current point.
   **/
  this.getUrl = function() {
    //document.getElementById('urlText').value = this.getApp().getUrl();
  }

  /**
   * Use the output for getUrl() to goto the point specified by the url.
   **/
  this.gotoUrl = function(){
    //this.getApp().gotoUrl(document.getElementById('urlText').value);
  }

  /**
   * Start recording of a movie. A movie is just a list of point ids, that are
   * animated through in order
   **/
  this.recordMovie = function() {
    this.getApp().recordMovie();
  }

  /**
   * Stop the recording of the movie.
   **/
  this.stopRecordingMovie = function() {
    this.getApp().stopRecordingMovie();
  }

  /**
   * Returns a url as a string, of the movie url.
   **/
  this.getMovieUrl = function() {
    //document.getElementById('movieUrlText').value = this.getApp().getMovieUrl();
  }

  /**
   * 	Plays the movie url returned from getMovieUrl, only the parameter part of
   * 	the url is used. When the movie has finished playing, the
   * 	view3dMovieCompleted callback is called.
   **/
  this.playMovieUrl = function() {
    //document.getElementById('movieStatus').innerHTML = 'Playing';
    //this.getApp().playMovieUrl(document.getElementById('movieUrlText').value);
  }

  /**
   * Stop playing a movie. This can only be done after playMovieUrl has been
   * called.
   **/
  this.stopMovie = function(){
    this.getApp().stopMovie();
  }

  /**
   * 	Resumes a movie, after stopMovie has been called.
   **/
  this.resumeMovie = function(){
    this.getApp().resumeMovie();
  }
}


/************************************************************
 *
 * When the document is load add the flash and set-up event
 * listners for the top menu etc.
 *
 ************************************************************/
$(document).ready(function() {
  // Create the viewer object and init the flash.
  viewer3dObj = new viewer3d();
  viewer3dObj.init();

  // Hide information overlay on load.
  $('#building-viewer-point-information').hide();

  // Add event listners to the next and previous buttons.
  $('#building-viewer-nav .previous').click(function() {
    viewer3dObj.prevPoint();
    return false;
  });
  $('#building-viewer-nav .next').click(function() {
    viewer3dObj.nextPoint();
    return false;
  });

  // Full screen (not used yet)
  $('#building-viewer-full-screen').click(function() {
    $('#building-viewer-outer-wrapper').css('z-index',99);
    
    Shadowbox.open({
        content:    $('#building-viewer-outer-wrapper').html(),
        player:     "html",
        title:      "3D viewer",
        height:     $(window).height(),
        width:      $(window).width()
    }); 

    return false;
  });

  // Help link
  $('#building-viewer-nav .help').click(function() {
    viewer3dObj.toggleHelp();
    return false;
  });
  
  // Help link
  $('.start-3dviewer').click(function() {
    viewer3dObj.toggleHelp();
    return false;
  });

  // Close information link
  $('a.building-viewer-close').click(function() {
    $(this).parent().fadeOut();
    viewer3dObj.toggleOverlay();
    return false;
  });

  // Close overlay on click
  $('.building-viewer-overlay').click(function() {
    $('#building-viewer-point-information').fadeOut();
    $('#building-viewer-help').hide();
    viewer3dObj.toggleOverlay();
  });
  
  $('#building-viewer-nav li.menu').click(function() {
    return false;
  });  

  $('.download a').click(function(e) {
    viewer3dObj.snapshot();
    e.preventDefault();
    return false;
  });
    
});

/****************************
 * MOVIE CALLBACK FUNCTIONS *
 ****************************/
function view3dLocationChanged(id) {
  // Update current point id, which is used to keep track of route information.
  viewer3dObj.currentPoint = id;
}

function view3DLoaded() {
  // Set labels for infopoints and tooltips.
  viewer3dObj.loadLabels();

  // Only show helper text, if it's the first time the viewer is shown.
  if (!$.cookie('viewer3d_help')) {
    viewer3dObj.toggleHelp();
  }
  $.cookie('viewer3d_help', 1);
}

function view3dInfoClicked(id) {
  viewer3dObj.rotateToDefaultDirection();
}

function view3dRotationCompleted(id) {
  // Info box, if requested else do ...
  //if (viwer3dInfoShow) {
    var viewerSettings = Drupal.settings.viewer3d;
    var href = viewerSettings.path + '/ajax/info/' + id;
    
    // Lookup the local cache.
    var info = jQuery.data(document.body, "info_"+id);
    if (info) {
      $('#building-viewer-point-information .building-viewer-point-inner').html(info);
      viewer3dObj.toggleOverlay();
      $('#building-viewer-point-information').fadeIn();
    }
    else {
      // Make ajax call to get extended information information about the point.
      $.get(href, function(data) {
        data = Drupal.parseJson(data);
        $('#building-viewer-point-information .building-viewer-point-inner').html(data.value);
        viewer3dObj.toggleOverlay();
        $('#building-viewer-point-information').fadeIn();
        jQuery.data(document.body, 'info_'+id, data.value);
      });
    }
  //}
}

function view3dMouseOverPoint(id, x, y, height) {}
function view3dOverviewMouseOverPoint(id, x, y, height) {}
function view3dPointClicked(id, x, y) {}
function view3dMoved() {}
function view3dMouseOutPoint(id) {}

/***********************************
 * END OF MOVIE CALLBACK FUNCTIONS *
 ***********************************/
