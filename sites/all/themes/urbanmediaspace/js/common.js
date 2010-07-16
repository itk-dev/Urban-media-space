// Add example to top search form

$(document).ready(function() {  
  // Get search form input
  $('#edit-search-block-form-1')
    // Add example function
    .example(
      // Get input field
      $('#edit-search-block-form-1')
      // Get the title of the input field
      .attr('title')
  );

  // Hijack carousel pager link
  $('.views-slideshow-ddblock-cycle-urbanmediaspace a.pager-link').click(function() {
    location.href = $(this).attr('href');
  });

  // Add opacity hover effect on footer logos
  $('#footer-logos div a')
  .css('opacity',.25)
  .show()
  .mouseover(function() {
    $(this).css('opacity',1);
  })
  .mouseout(function() {
    $(this).css('opacity',.25);
  })

});