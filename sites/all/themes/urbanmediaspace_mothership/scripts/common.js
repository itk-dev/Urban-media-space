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
    // Set variables
    var sbClose = $('#sb-nav-close');
    var sbTitle = $('#sb-title');

    // Add the title from the close link to Drupal's t() function
    sbClose.attr('title',Drupal.t('Close'));

    if(sbTitle) {
      // Add a span to sbTitle with Shadowbox close link
      // Don't add the text directly to the <a> tag, there is a IE7/8 issue with "filter: .. AlphaImageLoader .."
      sbTitle.append('<span id="sb-nav-label">' + sbClose.attr('title') + '</span>').click(function() {Shadowbox.close()});
      // Append sbTitle
      sbTitle.append(sbClose);
    }
  }

  // Call the function when Shadowbox opens
  Shadowbox.options.onOpen = moveCloseLink;

});