$(document).ready(function() {
    
   var viewerSettings = Drupal.settings.viewer3d;
   swfobject.embedSWF(viewerSettings['flash_location']+'/swf_viewer/BuildingViewer.swf', "building-viewer", "940", "600", "10.0.0", viewerSettings['flash_location']+"/scripts/expressInstall.swf",{width:'1045',DaluxBuildingViewServerURL:"http://prod.dalux.dk/mmhus/output/0806/&OverviewURL=http://prod.dalux.dk/mmhus/overview/&currentLocation=8&angle=4.54&angle2=6.10&showTopBar=true&showLog=true"},{allowFullScreen:"true",allowScriptAccess:"sameDomain",wmode: "1"});

   alert(viewerSettings['flash_location']+'/swf_viewer/BuildingViewer.swf');

  $('#building-viewer-nav-wrapper li a').tipsy({gravity: 's',fade: true});

  function gotoLocation(pos, angle1, angle2) {
    var app = thisMovie('building-viewer');

    app = getObject('building-viewer');
    if (!app) {
      app = getObject('BuildingViewer-name');
    }

    app.gotoLocation(pos, angle1, angle2);
  }

   function getObject(obj) {
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

   function thisMovie(movieName) {
       // IE and Netscape refer to the movie object differently.
       // This function returns the appropriate syntax depending on the browser.

       var IE = /*@cc_on!@*/false


       if (IE) {
        return window[movieName];
       } else {
        return window.document[movieName];
       }
   }
});


