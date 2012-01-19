$(document).ready(function() {

  var timelineList = $('.timeline-row-list');
  
  var timelineConfig = {
    over: function() {
      if ($(timelineList).not('.active')) {
        $(this).addClass('active');
      }
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {      
      $(this).removeClass('active');
      
      if (!timelineList.find('li.col').hasClass('sticky')) {
        timelineList.animate({
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
      timelineList.animate({
        paddingBottom: '35px'
      }, 100, function() {
        // Animation complete.
              
        // Only hide the column if is  not sticky.
        if (!timelineList.find('li.col').hasClass('sticky')) {
          // Add active class.
          thisItem.addClass('active');

          thisItem.find('ul').css('display', 'block');
        }
      });
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {

      // Remove active class.
      $(this).removeClass('active');
      
      if (!$(this).hasClass('sticky')) {        
        $(this).find('ul').css('display', 'none');        
      }
    }
  }
  
  // Make list item sticky.
  timelineList.find('li').click(function() {
    if ($(this).hasClass('.sticky')) {
      $(this).removeClass('sticky');      
    } else {
      $(this).addClass('sticky');
    }    
  });

  // Width of timeline nav wrapper.
  var timelineListWidth = timelineList.width();
  
  timelineList.find('ul').each(function(index) {
    submenuPos = $(this).position().left - ($(this).width()/2 - timelineList.width()/2);

    if (submenuPos<=0) {
      // If the submenu is position negative
      // position is 0 pixels from left
      $(this).css('left',0);
    } else if (submenuPos+$(this).width()>timelineListWidth) {
      // If the submenu is positionen to far to the right
      // reset left and position it 0 pixels from right
      $(this).css({'left': 'auto', 'right': 0});
    } else {
      // If the submenu fits inside the wrapper
      // position it centered on the active menu item
      $(this).css('left',submenuPos);
    }

  });

  // Add hoverIntent to menu items
  timelineList.find('li').hoverIntent(config);

  // Add hoverIntent to timeline list.
  $(timelineList).hoverIntent(timelineConfig);

});
