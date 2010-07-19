$Id: readme.txt,v 1.1 2009/07/20 17:31:57 thebuckst0p Exp $

Envts (Environments) module
by Ben Buckman (thebuckst0p) @ EchoDitto
ben@echoditto.com

This module was created to fill the need for *two-dimensional* environment/host management.
	* What's an environment?
		An environment is a development or server location where a full Drupal build 
		(possibly including multiple hosts) is set up. In our work, we tend to have 
		1) a primary 'dev' environment (site.dev.ourwork.com), 
		2) local environments for each developer (site.dev.local), 
		3) a staging environment (staging.site.com),
		4) a live site (www.site.com), etc.
		But each of these can have multiple hosts, making the number of actual hosts to track and 
		toggle grow exponentially.
		The domain_access module does a good job of tracking hosts, but in only one environment.
	
		
It's lightweight, has no UI (yet), but simply allows other modules to define environments with a hook.
(Anyone is welcome to create a UI for the module, since my time is limited!)

TERMINOLOGY USED:
 'envt': a code environment with multiple sites, such as 'dev', 'live', 'local'
 'site': a site NAME, possibly used in multiple environments, e.g. 'primary', 'secondary'
 'host': the domain of a site, e.g. www.site.com or site.dev.local, 
		 preferably matching the /sites/ folder and $base_root (without http:// prefix)

** Created hook_define_envts(): Allow other modules to define site groups **
	
	modules can implement hook_define_envts() in 3 ways:
		1) return 2-dimensional array of environments/hosts, such as
			function hook_define_envts() {
				return array(
					'live' => array(
							'site1'=>'www.site1.com',
							'site2'=>'www.site2.com',
						),
					'dev' => array(
							'site1'=>'dev.site1.com',
							'site2'=>'dev.site2.com',
						),
			  	);
			}
			(environments and sites will be keyed as in returned array)

		2) return 1-dimensional array of hosts, such as
			function hook_define_envts() {
				return array(
					'site1'=>'www.site1.com',
					'site2'=>'www.site2.com',
				);
			}
			(environments will then be keyed with name of passing module, and sites as in returned array)

		3) return single host, such as
			function hook_define_envts() {
				return 'www.site.com';
			}
			(environments will then be keyed by module, and site keyed as 'default')


	PREFERABLY the site keys (site1, site2 in the example) would be the same for each environment,
	so toggling between environments would be easy with envts_switch_envt();
	
** The module also creates a block with a simple table showing defined and current environment and site.

** Please submit feedback to ben@echoditto.com.
** Enjoy!

