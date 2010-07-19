<?php
// $Id: test-envts.php,v 1.1 2009/07/20 17:31:57 thebuckst0p Exp $

/**
 * Example of hook_define_envts() for testing
 * (call to this file is commented out in envts.module)
 */


// test envts' hook_define_envts
function envts_define_envts() {
	// 2 dimensional
	$envts = array(
		'local' => array(
				'sandbox'=>'sandbox.site.local',
				'example'=>'example.site.local',
			),
		'dev' => array(
				'sandbox'=>'sandbox.site.com',
				'example'=>'example.site.com',
			),
	);

	// // 1 dimension
	// $envts = array(
	// 		'sandbox'=>'sandbox.site.local',
	// 		'example'=>'example.site.local',
	// 	);
	
	
	// single site
	//$envts = 'sandbox.site.local';
		
	return $envts;
}
