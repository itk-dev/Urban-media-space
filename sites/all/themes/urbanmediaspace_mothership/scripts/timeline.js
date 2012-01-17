$(document).ready(function() {

  var timelineConfig = {
    over: function() {
      if ($('timeline-row:not(.active)')) {
        $(this).addClass('active');
      }
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {      
      $(this).removeClass('active');
      
      if (!$('.timeline-row li.col').hasClass('sticky')) {
        $('.timeline-row').animate({
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
      $('.timeline-row').animate({
        height: '53px'
      }, 250, function() {
        // Animation complete.
        if (!$('.timeline-row li.col').hasClass('sticky')) {
          $('ul',thisItem).fadeIn();
        }
      });
    },
    timeout: 750, // set timeout in milliseconds
    out: function() {      
      if (!$(this).hasClass('sticky')) {        
        $('ul',$(this)).hide();        
      }
    }
  }
  
  // Make list item sticky.
  $('.timeline-row li').click(function() {
    if ($(this).hasClass('.sticky')) {
      $(this).removeClass('sticky');      
    } else {
      $(this).addClass('sticky');
    }    
  });

  // Add hoverIntent to menu items
  $('.timeline-row li').hoverIntent(config);

  // Add hoverIntent to timeline list.
  $('.timeline-row').hoverIntent(timelineConfig);

});
