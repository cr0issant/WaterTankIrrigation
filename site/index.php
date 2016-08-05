<html>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<style>
h1
{
	font-weight: bold;
	font-size: 10vw;
	text-align: center;
}
p
{
	font-weight: bold;
	font-size: 7vw;
}
input
{
	font-weight: bold;
	font-size: 7vw;
}

input[type="radio"]{display:none;}
input[type="radio"] + label::before{
	content:"";
	display:inline-block;
	width:4vw;
	height:4vw;
	background:white;
	border-radius:4vw;
}
input[type="radio"]:checked + label::before{
	content:"";
	display:inline-block;
	width:5vw;
	height:5vw;
	background:limegreen;
	border-radius:5vw;
}

.bouton_submit{
	background:white;
	border-radius:2vw;
	
}
 
.bouton_submit:hover{
	background:limegreen;
	border-radius:2vw;
}
 
.bouton_submit:active{
	background:limegreen;
	border-radius:2vw;
}

body {
    background:url(notrepotager.jpg);
    background-size:cover;
    margin:0;
}
</style>

<?php

function automatique()
{
	$json = fopen('time.php', 'r+');
	ftruncate($json, 0);
	fputs($json, '{"heure":<?php echo date("H");?>}');
	fclose($json);
}
function desactiver()
{
	$json = fopen('time.php', 'r+');
	ftruncate($json, 0);
	fputs($json, '{"heure":26}');
	fclose($json);
}
function force()
{
	$json = fopen('time.php', 'r+');
	ftruncate($json, 0);
	fputs($json, '{"heure":19}');
	fclose($json);
}

if ( $_POST['arrosage'] == 'auto' )
{
	automatique();
}
elseif ( $_POST['arrosage'] == 'desac' )
{
	desactiver();
}
elseif ( $_POST['arrosage'] == 'force' )
{
	force();
}
else { }


$json = file_get_contents('./time.php');
$json_data = json_decode($json);
//echo 'Il est '.$json_data->heure.'h.';
?>
<body>
<h1>POTAGER</h1>
<p>Choix du mode d'arrosage</p>

<form action="index.php" method="post">

<p><input type="radio" name="arrosage" value="auto" id="auto" 
<?php 
	if ($json_data->heure != 19 || $json_data->heure != 26) { echo 'checked="checked"';} 
?> 
/> <label for="auto"> Automatique ( 19h )</label></p>

<p><input type="radio" name="arrosage" value="desac" id="desac" 
<?php 
	if ($json_data->heure == 26 ) { echo 'checked="checked"'; } 
?>
/> <label for="desac"> Désactiver</label></p>

<p><input type="radio" name="arrosage" value="force" id="force" 
<?php 
	if ($json_data->heure == 19 ) {echo 'checked="checked"';} 
?>
/> <label for="force"> Arrosage !!!</label></p>

<p><input type="submit" class="bouton_submit" value="Valider" /></p>
</form>

<?php 
	$text = fopen('later.data', 'r+');
	$text = fgets($text);
	fclose($text);
	echo '<p>' . $text. '</p>';

	$text = fopen('done.data', 'r+');
	$text = fgets($text);
	fclose($text);
	echo '<p>' . $text. '</p>';
?> 

</body>
</html>

