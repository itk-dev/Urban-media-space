// Add example to top search form

$(document).ready(function() {  
  // Get search form input
  $('#edit-search-block-form-1')
    // Add example function
    .example(
      // Get label
      $('#edit-search-block-form-1-wrapper label')
      // Get search label text
      .text()
  );

  // Hijack carousel pager link
  $('.views-slideshow-ddblock-cycle-urbanmediaspace a.pager-link').click(function() {
    location.href = $(this).attr('href');
  });

  // Add opacity hover effect on footer logos
  $('#footer-logos div a')
  .css('opacity',.25)
  .show()
//  .hoverIntent(
//    function() {
//      $(this).fadeTo('fast',1);
//    },
//    function() {
//      $(this).fadeTo('fast',.25);
//    }
//   );
  .mouseover(function() {
    $(this).css('opacity',1);
  })
  .mouseout(function() {
    $(this).css('opacity',.25);
  })

});