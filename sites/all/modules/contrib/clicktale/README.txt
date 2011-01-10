Clicktale
--------------------------

Requires: 
 - Drupal 6.x
 - PHP Integrations version 0.9 (included)
 - A ClickTale account (free or paid)


The ClickTale module offers support for the ClickTale tracking engine (www.clicktale.net).
It allows the user to create different configurations and blacklist/whitelist pages, 
ip-addresses and ranges. 
Pages and IP-ranges can be specified using a regex to allow the user
to match a range of IP addresses or pages with the same parent with one rule.

The ClickTale module requires the PHP Integration Module to be installed inside the clicktale
directory and a valid ClickTale account. Free ClickTale accounts can be used for testing purposes
but have limitations.

When enabled, the module adds several pages to the administration menu. These are 
grouped under /admin/settings/clicktale.

The module will look for the PHP Integration Module in /sites/all/libraries, and will place
a "clicktale" directory in the files directory. The Cache files created by the PHP Integration
Module will be placed in this directory and will be automatically deleted after pull.
In case of multisites the PHP Integration module can be placed in /sites/all/libraries OR
in your subsite libary folder at /sites/%subsite%/libraries. The Cache and Logs will be placed
in /sites/%subsite%/files/clicktale.

Problems have been reported with PHP Integrations versions higher than 0.9.
If the Cache-folder remains empty while the ClickTale module is running you might be running
a version of PHP Integrations higher than 0.9.
A compressed version of the PHP Integrations module might be included in this package.

******************************************************************************************************

Where to get the ClickTale ID and project number ?
	The ClickTale ID and project number are given with your ClickTale code.
	If your code should be for example "ClickTale(1874, 1, 'www03');", your ClickTale ID would be "1874" and
	project number 'www03'. The '1' stands for a '100%' of visitors to be tracked.

	
******************************************************************************************************

I can't see any hits on my ClickTale account.
	PHP Integrations module caches the pages in a Cache folder, this should be created in your Drupal file 
	directory. If it's not, please check your status screen or recent logs to look for a reason.
	If the Cache folder is empty, it is possible that you are using a version of PHP Integration Module
	higher than 0.9. If there are files inside the Cache folder they will be pulled by ClickTale servers
	and everything should work fine. This can take a while though.
	
******************************************************************************************************

What is the use of the PHP Integration module ?
	The PHP Integration module caches the page output and stores it on the harddisk in Cache files. 
	These files are then pulled by the ClickTale servers so the pageviews will be visible in ClickTale.
	Without the PHP Integration module every page you check Heatmaps or Clickmaps for would look like the 
	version of the page at that specific time, which means specific user content or content related to the
	users status would not be visible. With PHP Integrations you see the content as it was shown to the user.

*******************************************************************************************************

How do the configurations work ?
	The ClickTale module has the ability to work with different configuration settings. Each configuration 
	has its own set of rules to decide wether to count a pageview or not.
	
	To configure a configuration set, navigate to /admin/settings/clicktale/configurations, check the configuration
	you want to edit and click 'Configure'.
	
	It is possible to only track a number of user roles. If a user role is not checked, ClickTale will NOT count
	hits from any user in that role.
	
	The filters for pages and IP addresses have a blacklist/whitelist mode and support simple RegEx codes.
	Some examples:
	
		Count all pages except administration pages
		Page mode: blacklist
		admin/*
		
		Only count hits from non-logged in users and in /page/to/discard and its subpages
		Page mode: whitelist
		page/to/discard*
		Only tick off 'anonymous user'
		
		Count all hits except those from a local network.
		IP mode: blacklist
		192.168.*.*
		
		Count all hits except those from certain IP addresses on the local network.
		IP mode: blacklist
		192.168.0.(100|101|105)
	
*******************************************************************************************************

How can I see if ClickTale is counting ?
	By enabling the processing log you can check wether or not the ClickTale module is counting the pageviews.
	This log will also show you if one of the rules you set up in the configuration matched the current pageview.
	Use this to set up the configuration, but do not leave it on afterwards!
	
	If ClickTale is counting in the processing log but the hits are not showing up in the ClickTale website, 
	the problem will most likely be either the file permissions or account settings.
	
	To check if the file permissions are OK, check the Cache directory to see if cache files are being rendered.
	You'll find this directory in /sites/default/files/clicktale/Cache or /sites/%subsite%/files/clicktale/Cache.
	
	If cache files are generated but the hits are not showing up in your ClickTale account, the problem most likely
	lies with your account settings. Check wether the ClickTale ID is correct, your ClickTale ratio is 1 and your
	ClickTale project number is the one specified for that account.


