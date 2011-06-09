<?php
// $Id: node.tpl.php,v 1.1 2009/06/26 00:33:39 duvien Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
?>
<script type="text/javascript" src="<?php print $directory ?>/scripts/swfobject.js"></script>
<script type="text/javascript">
  swfobject.embedSWF("<?php print $directory ?>/3d-viewer/BuildingViewer.swf", "building-viewer", "940", "600", "10.0.0", "<?php print $directory ?>/scripts/expressInstall.swf",{DaluxBuildingViewServerURL:"http://prod.dalux.dk/mmhus/output/0806/&OverviewURL=http://prod.dalux.dk/mmhus/overview/&currentLocation=2&angle=-4&angle2=0&showTopBar=false&showLog=true"},{allowFullScreen:"true",allowScriptAccess:"sameDomain",wmode: "1"});

  function gotoLocation(pos, angle1, angle2) {
    var app = thisMovie('building-viewer');

    app = getObject('building-viewer');
    if (!app) {
      app = getObject('BuildingViewer-name');
    }

    app.gotoLocation(pos, angle1, angle2);
  }

   function getObject(obj) {
       var theObj;
       if (document.all) {
           if (typeof obj == "string") {
               return document.all(obj);
           } else {
               return obj.style;
           }
       }
       if (document.getElementById) {
           if (typeof obj == "string") {
               return document.getElementById(obj);
           } else {
               return obj.style;
           }
       }
       return null;
   }

   function thisMovie(movieName) {
       // IE and Netscape refer to the movie object differently.
       // This function returns the appropriate syntax depending on the browser.

       var IE = /*@cc_on!@*/false


       if (IE) {
        return window[movieName];
       } else {
        return window.document[movieName];
       }
   }

  $(document).ready(function() {
    $('#building-viewer-nav-wrapper li a').tipsy({gravity: 's',fade: true});
  });

</script>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"><div class="node-inner">

  <?php if ($title && !$is_front): ?>
    <h1 class="title" id="page-title"><?php print $title; ?></h1>
  <?php endif; ?>

  <?php if (!$page && $title): ?>
    <h2 class="title">
      <a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($submitted or $terms): ?>
    <div class="meta">
      <?php if ($terms): ?>
        <!-- <div class="terms terms-inline"><?php print t(' in ') . $terms; ?></div> -->
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="content">
    <?php print $content; ?>
  </div>

  <div id="building-viewer-nav-wrapper">
    <ul id="building-viewer-nav">
      <li class="home"><a href="javascript:gotoLocation(1,2,1)" title="Klik for at gå til start">Gå til start</a></li>
      <li class="previous"><a href="#" title="Gå til forrige destination">Forrige destination</a></li>
      <!-- <li><a href="#" title="Vælg">Vælg ny</a></li> -->
      <li class="next last"><a href="#" title="Gå til næste destination">Næste destination</a></li>
    </ul>
    <ul id="building-viewer-menu">
      <!-- <li class="first"><a href="#" title="Menu">Menu</a></li>
      <li><a href="#" title="Hjælp">Hjælp</a></li> -->
      <li class="fullscreen last"><a href="#" title="Skift til fuldskærms visning">Fuldskærm</a></li>
    </ul>
  </div>
  <div id="building-viewer-wrapper">
    <div id="building-viewer"></div>
  </div>

</div></div> <!-- /node-inner, /node -->