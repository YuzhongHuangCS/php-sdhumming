<?php

/**
* Command Line Interface demo of php-sdhumming
*
* @author		Huang Yuzhong
*/

switch ($GLOBALS['argc']) {
	case 2:
		var_dump(SDHummingSearch($GLOBALS['argv'][1]));
		break;
	case 3:
		var_dump(SDHummingLoadModel($GLOBALS['argv'][2]);
		var_dump(SDHummingSearch($GLOBALS['argv'][1]));
		break;
	case 4:
		var_dump(SDHummingLoadModel($GLOBALS['argv'][2], $GLOBALS['argv'][3]));
		var_dump(SDHummingSearch($GLOBALS['argv'][1]));
		break;
	default:
		$filename = $GLOBALS['argv'][0];
		echo("Usage: php {$filename} <audio file> [model file] [info file]\n");
		echo("Example: php {$filename} test.wav\n");
		break;
}

?>