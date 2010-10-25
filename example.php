<?php

require_once("chromatica.lib.inc");

$image = new chromaticaImage();
//$image->addLayer(new chromaticaLayer("letters/c.jpg"),CHROMATICA_TOP_LEFT,0,0);
$image->addLayer(new chromaticaLayer("letters/h.png"),CHROMATICA_TOP_LEFT,100,0);
//$image->addLayer(new chromaticaLayer("letters/r.jpg"),CHROMATICA_TOP_LEFT,200,0);
//$image->addLayer(new chromaticaLayer("letters/o.jpg"),CHROMATICA_TOP_LEFT,300,0);

//$image->addLayer(new chromaticaLayer("letters/m.jpg"),400,0);
//$image->addLayer(new chromaticaLayer("letters/a.jpg"),500,0);
//$image->addLayer(new chromaticaLayer("letters/t.jpg"),600,0);
//$image->addLayer(new chromaticaLayer("letters/i.jpg"),700,0);
//$image->addLayer(new chromaticaLayer("letters/c.jpg"),800,0);
//$image->addLayer(new chromaticaLayer("letters/a.jpg"),900,0);

$image->addLayer(new chromaticaText("Copyright lol",CHROMATICA_WHITE),CHROMATICA_BOTTOM_RIGHT,50,50);
$image->crunch("img.png",1000,100);
?>
<body bgcolor="#FFCC99">
	<img src="img.png"/>
</body>
