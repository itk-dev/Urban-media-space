<?php
// $Id: envts-block.tpl.php,v 1.1 2009/07/20 17:31:57 thebuckst0p Exp $

/**
 * Environments block template
 * very simple table showing active environment/site
 * override w/ new envts-block.tpl.php in theme
 */
?>

<table>
	<?php foreach($envts_list as $envt=>$sites): ?>
		<tr>
			<td><?php echo ($envt===$current_envt) ? "<strong>$envt</strong>" : $envt ?></td>

			<?php foreach($sites as $site=>$url): ?>
				<td>
					<a href="<?php echo envts_full_url($url) ?>/<?php echo $current_path ?>"><?php
					 	echo ($site===$current_site and $envt===$current_envt) ? "<strong>$site</strong>" : $site ?></a>
				</td>
			<?php endforeach; ?>
			
		</tr>
	<?php endforeach; ?>
</table>
