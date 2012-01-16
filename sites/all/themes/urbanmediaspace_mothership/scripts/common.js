$(document).ready(function() {

  // Add example to top search form
  // Get search form input
  $('#search-block-form .form-item input.form-text')
    // Add example function
    .example(
      // Get input field
      $('#block-search-0 h2.title')
      // Get the title of the input field
      .text()
  );
    
  // Add example to mailchimp newsletter block  
  var mailchimpBlock = $('.block-mailchimp');
  var mailchimpLabel = mailchimpBlock.find('label');
  
  // Hide label
  $(mailchimpLabel).hide();
   
  // Add example function
  $(mailchimpBlock).find('input.form-text').example(Drupal.t('Enter email address'));

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

  // Make sure Shadowbox is defined
  if (typeof Shadowbox != 'undefined') {
    // Call the function when Shadowbox opens
    Shadowbox.options.onOpen = moveCloseLink;
  }

});