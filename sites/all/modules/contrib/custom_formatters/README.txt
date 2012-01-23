The Custom Formatters module allows users to easily create custom CCK Formatters
without the need to write a custom module. Custom Formatters can then be
exported as Features or Drupal API Formatters.

Custom Formatters was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark
- http://twitter.com/Decipher


Features
--------------------------------------------------------------------------------

* Two different editor modes:
  * Basic: A HTML based editor with Token support.
  * Advanced: A PHP based editor with support for multiple fields and multiple
    values.
* Support for:
  * CCK fields.
  * CCK Fieldgroups.
  * CCK 3.x Multigroups.
  * Display Suite fields.
  * Views.
* Exportable as:
  * Drupal API formatter via:
    * Custom Formatters export interface.
  * Custom Formatters exportable via:
    * Custom Formatters export interface.
    * Features module.
* Live preview (requires Devel generate module).
* Integrates with:
  * Features module - Adds dependent Custom Formatters (from Views or Content
      types) to Feature.
  * Insert module - Exposes Custom Formatters to the Insert module.
  * Libraries API module and the EditArea javascript library - Adds real-time
      syntax highlighting.


Required Modules
--------------------------------------------------------------------------------

* Content Construction Kit (CCK)  - http://drupal.org/project/cck
* Token - http://drupal.org/project/token


Recommended Modules
--------------------------------------------------------------------------------

* Devel (includes Devel generate) - http://drupal.org/project/devel
* Libraries API - http://drupal.org/project/libraries


Usage
--------------------------------------------------------------------------------

Custom Formatters can be managed on the 'Custom Formatters'
overview page: 'Administer > Site configuration > Custom Formatters'.
http://[www.yoursite.com/path/to/drupal]/admin/build/formatters

More information on usage, including tips & tricks, can be found in help:
http://[www.yoursite.com/path/to/drupal]/admin/help/custom_formatters


EditArea - Real-time syntax highlighting
--------------------------------------------------------------------------------

The EditArea javascript library adds real-time syntax highlighting, to install
it follow these steps:

1. Download and install the Libraries API module.
    http://drupal.org/project/libraries

2. Download the EditArea library and extract and move it into your libraries
   folder as 'editarea' (eg. sites/all/libraries/editarea).
    http://sourceforge.net/projects/editarea/files/EditArea/EditArea%200.8.2/editarea_0_8_2.zip/download


Upgrading
--------------------------------------------------------------------------------

Custom Formatters 1.2 adds the requirement of the Token module, you MUST install
and enable the module if you have not already done so or you will run into
issues.

And as always, be sure to run update.php after updating Custom Formatters.
http://[www.yoursite.com/path/to/drupal]/update.php


Developers
--------------------------------------------------------------------------------

Please refer to DEVELOPERS.txt for information on provided improved support for
your modules defined CCK fields with the Custom Formatters module.
