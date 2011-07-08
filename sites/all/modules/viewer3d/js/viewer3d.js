$(document).ready(function() { 
  var viewerSettings = Drupal.settings.viewer3d;
  swfobject.embedSWF(viewerSettings['flash_location']+'/swf_viewer/BuildingViewer.swf',
                     "building-viewer",
                     "940",
                     "666",
                     "10.0.0",
                     viewerSettings['flash_location']+"/scripts/expressInstall.swf",
                     {width:'940',DaluxBuildingViewServerURL:viewerSettings['data']+'&OverviewURL='+viewerSettings['overviewURL']+'&currentLocation='+viewerSettings['currentLocation']+'&angle='+viewerSettings['angle']+'&angle2='+viewerSettings['angle2']+'&showTopBar='+viewerSettings['showTopBar']+'&showLog='+viewerSettings['showLog']},
                     {allowFullScreen:"true", allowScriptAccess:"sameDomain", wmode: "opaque"});

  // Configure qtip, see: http://craigsworks.com/projects/qtip/docs/
  $('#building-viewer-nav-wrapper li a').qtip({
      position: {
      corner: {
         target: 'topMiddle',
         tooltip: 'bottomMiddle'
      },
      adjust: {
        y: -5
      }
    },
    style: {
      name: 'dark',
      background: '#333',
      fontSize: '11px',
      border: {
        radius: 3,
        width: 0
      },
      tip: {
        corner: 'bottomMiddle',
        size: { x: 7, y: 5 }
      }
    }
  });

  // Hide information on load
  $('#building-viewer-point-information').hide();

  // Set rute information.
  var route = Drupal.settings.viewer3d_route;
  
  // Set current point, its update by view3dLocationChanged event
  viewer3d_current_point = viewerSettings['currentLocation'];

  // Set prev and next
  $('#building-viewer-nav .previous').click(function() {
    if (route[viewer3d_current_point].prev.jump) {
      viewer3dGotoLocationDefaultDirection(route[viewer3d_current_point].prev.pid);
    }
    else {
      viewer3dFlyToLocation(route[viewer3d_current_point].prev.pid);
    }
    return false;
  });
  $('#building-viewer-nav .next').click(function() {
    if (route[viewer3d_current_point].next.jump) {
      viewer3dGotoLocationDefaultDirection(route[viewer3d_current_point].next.pid);
      return false;
    }
    else {
      viewer3dFlyToLocation(route[viewer3d_current_point].next.pid);
    }

  });

  // Help link
  $('#building-viewer-nav .help').click(function() {
    viewerToggleHelp();
    return false;
  });

  // Bind to starting point title link.
  $('#building-viewer-point-title a').click(function() {
    if (viewer3d_click) {
      viewer3d_click = false;
      view3dLoadInfoBox($(this).attr('href'))
      return false;
    }
    return false;
  });

  // Close information link
  $('a.building-viewer-close').click(function() {
    $(this).parent().fadeOut();
    viewerToggleOverlay();
    return false;
  });

  // Set tooltip on point placeholder.
  // Configure qtip, see: http://craigsworks.com/projects/qtip/docs/
  $('#building-viewer-point-tipsy [title]').qtip({
      position: {
      corner: {
         target: 'topMiddle',
         tooltip: 'bottomMiddle'
      },
      adjust: {
        y: -5
      }
    },
    style: {
      name: 'dark',
      background: '#333',
      fontSize: '11px',
      border: {
        radius: 3,
        width: 0
      },
      tip: {
        corner: 'bottomMiddle',
        size: { x: 7, y: 5 }
      }
    }
  });
});

/**********************
 * API IMPLEMENTATION *
 **********************/
function viewer3dGotoLocation(pos, angle, angle2) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.gotoLocation(pos, angle, angle2);
}

function viewer3dGetObject(obj) {
  var theObj;
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

function viewer3dGotoLocationDefaultDirection(pos) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.gotoLocationDefaultDirection(pos);
}

function viewer3dFlyToLocation(pos) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.flyToLocation(pos);
}

function viewer3dCreateJumpPoint(jumpName, pos, angle1, angle2) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.createJumpPoint(jumpName,pos, angle1, angle2);
}


function viewer3dCreateLoadPoint(jumpName, url) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.createLoadPoint(jumpName,url);
}

function viewer3dSetPointLabel(point, label) {
  var app = viewer3dMovie('BuildingViewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
        return;
    }
  }
  app.setPointLabel(point,label);
}

function viewer3dMovie(movieName) {
  // IE and Netscape refer to the movie object differently.
  // This function returns the appropriate syntax depending on the browser.
  var IE = /*@cc_on!@*/false

  if (IE) {
    return window[movieName];
  } else {
    return window.document[movieName];
  }
}
/*****************************
 * END OF API IMPLEMENTATION *
 *****************************/

/****************************
 * MOVIE CALLBACK FUNCTIONS *
 ****************************/
function view3dLocationChanged(id) {
  viewer3d_current_point = id;

  var viewerSettings = Drupal.settings.viewer3d;
  $.get(viewerSettings['path'] + '/ajax/title/' + id, function(data) {
    data = Drupal.parseJson(data);
    view3dUpdateTitle(data.value);
  });
}

function view3DLoaded() {
  viewerToggleHelp();
}

function view3dPointClicked(id, x, y) {

}

function view3dMoved() {

}

function view3dMouseOverPoint(id, x, y, height) {
  // Move point
  $('#building-viewer-point-tipsy').css('left', (x - 8) + 'px').css('top', (y - 10) + 'px');

  // Get point title.
  var viewerSettings = Drupal.settings.viewer3d;
  $.get(viewerSettings['path'] + '/ajax/title/' + id + '/0', function(data) {
    data = Drupal.parseJson(data);
    var point = $('#building-viewer-point-tipsy a');
    point.attr('title', data.value);
  });
}

function view3dMouseOutPoint(id) {

}
/***********************************
 * END OF MOVIE CALLBACK FUNCTIONS *
 ***********************************/

/*****************************
 * IMPLEMENTATION OF HELPERS *
 *****************************/
function view3dUpdateTitle(title) {
  // Update title.
  $('#building-viewer-point-title').html(title);

  // Bind click to the new link.
  $('#building-viewer-point-title a').click(function() {
    if (viewer3d_click) {
      viewer3d_click = false;
      view3dLoadInfoBox($(this).attr('href'))
      return false;
    }
    return false;
  });

  // Remove old information.
  $('#building-viewer-point-information .building-viewer-point-inner').html('');
}

function view3dLoadInfoBox(href) {
  // Make ajax call to get extended information information about the point.
  $.get(href, function(data) {
    data = Drupal.parseJson(data);

    $('#building-viewer-point-information .building-viewer-point-inner').html(data.value);

    // Toggle overlay
    viewerToggleOverlay();

    // Show the element
    $('#building-viewer-point-information').fadeIn();
  });
}

// Used to prevent dlb click.
var viewer3d_click = true;

// Toggle overlay, opt = show, hide
function viewerToggleOverlay(opt) {
  $('.building-viewer-overlay')
  .css('opacity', .5)
  .toggle('fast', function() {
    viewer3d_click = true;
  });
}

function viewerToggleHelp() {
  viewerToggleOverlay();
  $('#building-viewer-help').toggle();
}

/******************
 * END OF HELPERS *
*******************/
