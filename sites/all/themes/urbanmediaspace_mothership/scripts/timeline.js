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
          height: '23px'
        }, 250, function() {
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
        height: '53px'
      }, 250, function() {
        // Animation complete.
        if (!timelineList.find('li.col').hasClass('sticky')) {
          thisItem.find('ul').fadeIn();
        }
      });
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {      
      if (!$(this).hasClass('sticky')) {        
        $(this).find('ul').hide();        
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

  // Add hoverIntent to menu items
  timelineList.find('li').hoverIntent(config);

  // Add hoverIntent to timeline list.
  $(timelineList).hoverIntent(timelineConfig);

});
