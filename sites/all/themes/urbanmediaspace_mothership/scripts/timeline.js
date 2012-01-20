$(document).ready(function() {

  var timeline = $('.timeline-row-list');
  
  var timelineConfig = {
    over: function() {
      if ($(timeline).not('.active')) {
        $(this).addClass('active');
      }
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {      
      $(this).removeClass('active');
      
      if (!timeline.find('li.col').hasClass('sticky')) {
        timeline.animate({
          paddingBottom: '0'
        }, 100, function() {
          // Animation complete.
        });        
      }
    }
  }

  // hoverIntent configuration
  var config = {
    over: function() {
      thisItem = $(this);
      timeline.animate({
        paddingBottom: '35px'
      }, 100, function() {
        // Animation complete.
              
        // Only hide the column if is  not sticky.
        if (!timeline.find('li.col').hasClass('sticky')) {
          // Add active class.
          thisItem.addClass('active');

          thisItem.find('ul').fadeIn('fast');
        }
      });
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {

      // Remove active class.
      $(this).removeClass('active');
      
      if (!$(this).hasClass('sticky')) {        
        $(this).find('ul').fadeOut();
      }
    }
  }
   
  // Make list item sticky.
  timeline.find('li').click(function() {
    if ($(this).hasClass('.sticky')) {
      $(this).removeClass('sticky');      
    } else {
      $(this).addClass('sticky');
    }    
  });

  // Width of timeline nav wrapper.
  var timelineWidth = timeline.width();
  
  timeline.find('ul').each(function() {       
    
    listIndent = 19; // Used for indenting the items.   
    listWidth = $(this).width(); // The width of the current list.       
    listInitPos = $(this).parent('li').position().left; // The current lists initial position    
    listPos = listInitPos - listWidth/2 + listIndent; // The new position of the list.
       
    if (listPos<=0) {
      // If the submenu position is negative
      // position it to the left.
      $(this).css('left','17px');
    } else if (listPos+listWidth>timelineWidth) {
      // If the submenu is positionen to far to the right
      // reset left and position it 0 pixels from right
      $(this).css({'left': 'auto', 'right': '17px'});
    } else {
      // If the submenu fits inside the wrapper
      // position it centered on the active menu item
      $(this).css('left',listPos);
    }

  });

  // Add hoverIntent to menu items
  timeline.find('li').hoverIntent(config);

  // Add hoverIntent to timeline list.
  $(timeline).hoverIntent(timelineConfig);

});
