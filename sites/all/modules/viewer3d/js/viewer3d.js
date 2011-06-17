$(document).ready(function() {
    
  var viewerSettings = Drupal.settings.viewer3d;
  swfobject.embedSWF(viewerSettings['flash_location']+'/swf_viewer/BuildingViewer.swf',
                     "building-viewer",
                     "940",
                     "600",
                     "10.0.0",
                     viewerSettings['flash_location']+"/scripts/expressInstall.swf",
                     {width:'1045',DaluxBuildingViewServerURL:viewerSettings['data']+'&OverviewURL='+viewerSettings['overviewURL']+'&currentLocation='+viewerSettings['currentLocation']+'&angle='+viewerSettings['angle']+'&angle2='+viewerSettings['angle2']+'&showTopBar='+viewerSettings['showTopBar']+'&showLog='+viewerSettings['showLog']},
                     {allowFullScreen:"true", allowScriptAccess:"sameDomain", wmode: "1"});

  $('#building-viewer-nav-wrapper li a').tipsy({gravity: 's',fade: true});
});

/**********************
 * API IMPLEMENTATION *
 **********************/
function viewer3dGotoLocation(pos, angle1, angle2) {
  var app = thisMovie('building-viewer');
  app = viewer3dGetObject('building-viewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer-name');
  }
  app.gotoLocation(pos, angle1, angle2);
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
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
    }
  }
  app.gotoLocationDefaultDirection(pos);
}

function viewer3dFlyToLocation(pos) {
  var app = viewer3dMovie('BuildingViewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
    }
  }
  app.flyToLocation(pos);
}

function viewer3dCreateJumpPoint(jumpName, pos, angle1, angle2) {
  var app = viewer3dMovie('BuildingViewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
    }
  }
  app.createJumpPoint(jumpName,pos, angle1, angle2);
}


function viewer3dCreateLoadPoint(jumpName, url) {
  var app = viewer3dMovie('BuildingViewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
    }
  }
  app.createLoadPoint(jumpName,url);
}

function viewer3dSetPointLabel(point, label) {
  var app = viewer3dMovie('BuildingViewer');
  if (!app) {
    app = viewer3dGetObject('BuildingViewer');
    if (!app) {
      app = viewer3dGetObject('BuildingViewer-name');
      if (!app)
        alert("not found");
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
function view3dLocationChanged(id) {}

function view3DLoaded(){}

function view3dPointClicked(id, x, y){}

function view3dMoved(){}
/***********************************
 * END OF MOVIE CALLBACK FUNCTIONS *
 ***********************************/
