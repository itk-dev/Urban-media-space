<?php // $Id: nodepicker-page.tpl.php,v 1.1 2010/03/08 12:15:02 blixxxa Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title><?php print t('Node Picker'); ?></title>
  <?php print $styles; ?>
</head>
<body>
  <div id="messages">
    <?php print $messages; ?>
  </div>
  <div id="content">
    <?php print $content ?>
  </div>
  <?php print $scripts; ?>
</body>
</html>