<?php

/**
* Web Interface example of php-sdhumming
*
* @author		Huang Yuzhong
* @date 		2015/12/22
*/

?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>php-sdhumming-example</title>
</head>
<body>
	<h1>php-sdhumming-example</h1>
	<form action="example-web.php" method="post" enctype="multipart/form-data">
		<input type="file" name="file"/> 
		<input type="submit"/>
	</form>
	<?php
		if ($_SERVER['REQUEST_METHOD'] === 'POST') {
			$matches = SDHummingSearch($_FILES['file']['tmp_name']);

			echo('<h2>Matches:</h2>');
			echo('<table>');
			echo('<tr><th>id</th><th>info</th><th>score</th></tr>');

			foreach ($matches as $row) {
				echo("<tr><td>{$row->id}</td><td>{$row->info}</td><td>{$row->score}</td></tr>");
			}

			echo('</table>');
		}
	?>
</body>
</html>
