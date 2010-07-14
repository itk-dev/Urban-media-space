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
  })

});