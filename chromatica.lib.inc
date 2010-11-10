<?php
define("CHROMATICA_TOP_LEFT",		1);
define("CHROMATICA_TOP_RIGHT",		1);
define("CHROMATICA_BOTTOM_LEFT",	1);
define("CHROMATICA_BOTTOM_RIGHT",	1);

define("CHROMATICA_WHITE",		"FFFFFF");
define("CHROMATICA_RED",		"FF0000");
define("CHROMATICA_GREEN",		"00FF00");
define("CHROMATICA_BLUE",		"0000FF");
class chromaticaLayer{
	public $file;
	public $x;
	public $y;
	public $corner;
	public $width;
	public $height;
	public $GDObject;
	
	public function __construct($file,$corner = CHROMATICA_TOP_LEFT,$x = 0,$y = 0){
		$this->file = $file;
		$this->corner;
		$this->x = $x;
		$this->y = $y;
	}
	public function getGDObject(){
		if(!file_exists($this->file)){
			die("Cannot load {$this->file}... File is missing Q.Q\n");
		}
		$size = getimagesize($this->file);
		$this->width = $size[0];
		$this->height = $size[1];
		switch($size['mime']){
			case 'image/jpeg':
				$this->GDObject = imagecreatefromjpeg($this->file);
				return $this->GDObject;
				break;
			case 'image/png':
				$this->GDObject = imagecreatefrompng($this->file);
				return $this->GDObject;
				break;
			default:
				return false;			
		}

	}
}

class chromaticaText{
	public $x;
	public $y;
	public $corner;
	public $width;
	public $height;
	public $GDObject;
	public function __construct($text,$colour,$corner = CHROMATICA_TOP_LEFT,$x = 0,$y = 0){
	}
	public function getGDObject(){
	
	}
}

class chromaticaImage{
	private $layers;
	private $canvas;
	
	public $meta;
	public $outputFormat = 'png';
	
	public function __construct(){
		$this->canvas['Height'] = 0;
		$this->canvas['Width'] = 0;
	}
	public function addLayer($chromaticaLayer,$corner,$offset_x,$offset_y){
		$chromaticaLayer->x = $offset_x;
		$chromaticaLayer->y = $offset_y;
		$chromaticaLayer->corner = $corner;
		$this->layers[] = $chromaticaLayer;
	}
	private function saveAndReload(){
		$tmpImg = "/tmp/chromatica.".microtime(true).".img.tmp";
		@unlink($tmpImg);
		imagepng($this->canvas['image'], $tmpImg);
		$this->canvas['image'] = imagecreatefrompng($tmpImg);
		@unlink($tmpImg);
		imagesavealpha($this->canvas['image'], true);
		$this->canvas['Width'] 		= imagesx($this->canvas['image']);
		$this->canvas['Height'] 	= imagesy($this->canvas['image']);
	}
	public function scale($x,$y){
		$scaled = imagecreatetruecolor($x,$y);
		imagecopyresampled(
			$scaled,
			$this->canvas['image'],
			0,0,0,0,
			$x,
			$y,
			$this->canvas['Width'],
			$this->canvas['Height']
		);
		$this->canvas['image'] = $scaled;
		$this->saveAndReload();
		
		
	}
	public function crunch($outputFile = false){
		//Load all the image objects
		foreach($this->layers as $layer){
			$layer->getGDObject();
		}
		//calculate the max Canvas width & height
		foreach($this->layers as $layer){
			if($layer->x + $layer->width >= $this->canvas['Width']){
				$this->canvas['Width'] 		= $layer->x + $layer->width;
			}
			if($layer->y + $layer->height >= $this->canvas['Height']){
				$this->canvas['Height'] 	= $layer->y + $layer->height;
			}
		}
		
		//Create the canvas...
		$this->canvas['image'] = imagecreatetruecolor($this->canvas['Width'], $this->canvas['Height']);
		imagealphablending($this->canvas['image'],false);
		imagesavealpha($this->canvas['image'], true);
		$trans_colour = imagecolorallocatealpha($this->canvas['image'], 0, 0, 0, 127);
		imagefill($this->canvas['image'], 0, 0, $trans_colour);
		
		//Merge all the other layers into it
		foreach($this->layers as $layer){
			if($layer->GDObject != null){
				#imagecopymerge($this->canvas['image'],$layer->GDObject,$layer->x,$layer->y,0,0,$layer->width,$layer->height,100);  
				imagecopy(
					$this->canvas['image'],
					$layer->GDObject,
					$layer->x,
					$layer->y,
					0,
					0,
					$layer->width,
					$layer->height
					);
				//Hack to make layers save ontop of each other correctly.
				$tmpImg = "/tmp/chromatica.".time().".img.tmp";
				@unlink($tmpImg);

				imagepng($this->canvas['image'], $tmpImg);
				$this->canvas['image'] = imagecreatefrompng($tmpImg);
				imagesavealpha($this->canvas['image'], true);
				
			}
		}
		if($outputFile !== FALSE){
			//If the file already exists, destroy it	
			if(file_exists($outputFile)){
				unlink($outputFile);
			}
			//Output the new file.
			$this->saveToFile($outputFile,$this->canvas['image']);
			$this->meta = getimagesize($outputFile);
		
			return file_get_contents($outputFile);
		}else{
			$tmpImg = "/tmp/chromatica.".time().".img.tmp";
			@unlink($tmpImg);
			imagepng($this->canvas['image'], $tmpImg);
			$tmpImgData = file_get_contents($tmpImg);
			$this->meta = getimagesize($tmpImg);
			$this->canvas['image'] = imagecreatefrompng($tmpImg);
			@unlink($tmpImg);
			imagesavealpha($this->canvas['image'], true);
			return $tmpImgData;
		}
	}
	public function saveToFile($file,$im = false){
		if($im === false){
			$im = $this->canvas['image'];
		}
		$this->canvas['Width'] 		= imagesx($this->canvas['image']);
		$this->canvas['Height'] 	= imagesy($this->canvas['image']);
		switch($this->outputFormat){
			case 'jpg':
			case 'jpeg':
				imagejpeg($im, $file);
				break;
			case 'png':
			default:
				imagepng($im, $file);
		}
			
	}
	
	public function scaleAndCrunch($file,$x,$y){
		$this->scale($x,$y);
		$this->saveToFile($file);
	}
}


