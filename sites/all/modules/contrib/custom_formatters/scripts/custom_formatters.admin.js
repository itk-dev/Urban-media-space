(function($) {
  Drupal.behaviors.customFormattersAdmin = function(context) {
    // EditArea real-time syntax highlighter.
    if (typeof editAreaLoader !== 'undefined' && !$('#edit-code').hasClass('editarea-processed')) {
      $('#edit-code').addClass('editarea-processed');
      syntax = $('#edit-code').attr('class').match(/syntax-(\w+)\b/m);
      editAreaLoader.init({
        id: 'edit-code',
        syntax: syntax[1],
        start_highlight: true,
        allow_resize: "y",
        allow_toggle: false,
        toolbar: "*",
        word_wrap: false,
        language: "en",
        replace_tab_by_spaces: 2,
        change_callback: 'customFormattersEAUpdate',
      });
    }

    // Make sure '#edit-code' gets updated before we preview the formatter.
    $('#edit-preview').bind('mouseover', function() {
      customFormattersEAUpdate('edit-code')
    });
  }

  // Update '#edit-code' with the EditArea code.
  customFormattersEAUpdate = function(editor_id) {
    $('#' + editor_id).val(editAreaLoader.getValue(editor_id));
  }
})(jQuery);
