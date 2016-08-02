<html>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<?php
if ( $_GET['well'] == 'jarroseplustardmonvieux' )
{
	later();
}
elseif ( $_GET['well'] == 'jarrosemaintenantmonvieux' )
{
	done();
}
else { }

function later()
{
	$text = fopen('later.data', 'r+');
	ftruncate($text, 0);
	$valeur = 'Dernière vérification le '.date("d/m/Y").' à '.date("H:i:s").'.';
	fputs($text, $valeur);
	fclose($text);
}
function done()
{
	$text = fopen('done.data', 'r+');
	ftruncate($text, 0);
	$valeur = 'Dernier arrosage le '.date("d/m/Y").' à '.date("H:i:s").'.';
	fputs($text, $valeur);
	fclose($text);
}

?>
<body>

</body>
</html>

