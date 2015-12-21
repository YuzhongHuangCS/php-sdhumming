<?php

/**
* Command Line Interface example of php-sdhumming
*
* @author		Huang Yuzhong
* @date 		2015/12/22
*/

if ($GLOBALS['argc'] === 3) {
	var_dump(SDHummingBuildModel($GLOBALS['argv'][1], $GLOBALS['argv'][2]));
} else {
	$filename = $GLOBALS['argv'][0];
	echo("Usage: php {$filename} <mid_list> <model_dir>\n");
	echo("Example: php {$filename} mid_list.txt model\n");
}

?>