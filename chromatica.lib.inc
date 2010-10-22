<?php

class chromaticaLayer{
	public $file;
	public $x;
	public $y;
	public $width;
	public $height;
	public $GDObject;
	
	public function __construct($file,$x = 0,$y = 0){
		$this->file = $file;
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
class chromaticaImage{
	private $layers;
	private $canvas;
	
	public function __construct(){
		$this->canvas['Height'] = 0;
		$this->canvas['Width'] = 0;
	}
	public function addLayer($chromaticaLayer,$offset_x,$offset_y){
		$chromaticaLayer->x = $offset_x;
		$chromaticaLayer->y = $offset_y;
		$this->layers[] = $chromaticaLayer;
	}
	public function crunch($outputFile){
		//print_r($this->layers);
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
		imagesavealpha($this->canvas['image'], true);
		
		//Merge all the other layers into it
		foreach($this->layers as $layer){
			imagecopymerge($this->canvas['image'],$layer->GDObject,$layer->x,$layer->y,0,0,$layer->width,$layer->height,100);  
		}
		
		//print_r($this->canvas);
		
		//If the file already exists, destroy it		
		if(file_exists($outputFile)){
			unlink($outputFile);
		}
		//Output the new file.
		imagepng($this->canvas['image'], $outputFile);
	}
}
