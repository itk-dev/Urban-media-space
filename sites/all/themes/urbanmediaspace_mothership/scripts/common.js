$(document).ready(function() {  

  // Add example to top search form
  // Get search form input
  $('#fuzzysearch-box-form .form-item input.form-text')
    // Add example function
    .example(
      // Get input field
      $('#block-fuzzysearch-0 h2.title')
      // Get the title of the input field
      .text()
  );

  // Hijack carousel pager link
  $('.views-slideshow-ddblock-cycle-urbanmediaspace a.pager-link').click(function() {
    location.href = $(this).attr('href');
  });

  // Add opacity hover effect on footer logos
  $('#footer-logos div a')
  .css({
    'opacity': .25,
    'display': 'inline-block'
  })
  .mouseover(function() {
    $(this).css('opacity',1);
  })
  .mouseout(function() {
    $(this).css('opacity',.25);
  })

});