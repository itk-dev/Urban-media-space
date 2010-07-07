// $Id: nodepicker.js,v 1.1.2.5 2010/06/03 06:15:59 blixxxa Exp $

var instanceId = null;

/**
 * Define WYSIWYG API plugin.
 */
Drupal.wysiwyg.plugins.nodepicker = {

  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function(node) {
    return ($(node).is('a.nodepicker-link'));
  },

  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
    if (data.format == 'html') {
      nodepicker_dialog_open(instanceId);
    }
  },

  /**
   * Replace all [nodepicker] tags with links.
   * URL == Title == Link text
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/\[nodepicker\=\=([^\[\]]+)\]/g, function(orig, match) {
      var node = {}
      return nodepicker_decode(match, node);
    });
    return content;
  },

  /**
   * Replace links with [nodepicker] tags in content upon detaching editor.
   */
  detach: function(content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>'); // No .outerHTML() in jQuery :(
    $('a.nodepicker-link', $content).each(function(node) {
      var $self = $(this);
      var inlineTag = '[nodepicker==' + $self.attr("href") + '==' + encodeURIComponent($self.attr("title")) + '==' + encodeURIComponent($self.text()) + ']';
      $(this).replaceWith(inlineTag);
    });
    return $content.html();
  }
};

/**
 * Open jQuery UI dialog box.
 */
function nodepicker_dialog_open(instance) {
  instanceId = instance;
  $("body").append("<div id='nodepicker_dialog'><iframe id='nodepicker_dialog_contents' src='"+Drupal.settings.wysiwyg.plugins.drupal.nodepicker.url+"' width='602' height='446' frameborder='0' /></div>");
  $("#nodepicker_dialog").dialog({ modal: true, height: 476, width: 602, title: Drupal.t('Link to existing content'), beforeclose: function(event, ui) { nodepicker_dialog_close(); } }).show();
}

/**
 * Close jQuery UI dialog box.
 */
function nodepicker_dialog_close() {
  $("#nodepicker_dialog").remove();
}

/**
 * Decode an [nodepicker] tag and wait for response.
 */
function nodepicker_decode(tag, node) {
  var response = null;
  
  $.ajax({
      type: "POST",
      url: Drupal.settings.basePath + 'index.php?q=nodepicker/ajax&tag=[nodepicker==' + encodeURIComponent(tag) + ']',
      contentType: "application/json; charset=utf-8",
      dataType: "json",
      data: {},
      cache: true,
      async: false,
      success: function(data, textStatus) {
        response = data;
      }
  });
  return '<a class="nodepicker-link" href="'+response.href+'" title="'+decodeURIComponent(response.title)+'">'+decodeURIComponent(response.link_text)+'</a>';
}