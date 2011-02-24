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

  function moveCloseLink(){
    var sbClose = $('#sb-nav-close');
    var sbTitle = $('#sb-title');

    sbClose.attr('title',Drupal.t('Close'));
    sbTitle.append('<span id="sb-nav-label">' + sbClose.attr('title') + '</span>').click(function() {Shadowbox.close()});

    if(sbTitle) {
      sbTitle.append(sbClose);
    }

  }

  Shadowbox.options.onOpen = moveCloseLink;

});