<?php

class chromaticaLayer{
	public $file;
	public $x;
	public $y;
	
	public function __construct($file,$x = 0,$y = 0){
		$this->file = $file;
		$this->x = $x;
		$this->y = $y;
	}
	public function getGDObject(){
		if(file_exists($this->file)){
			$data = file_get_contents($this->file);
			echo strlen($data)." bytes\n";
		}else{
			echo "Cannot load {$this->file}... Q.Qn";
		}
	}
}
class chromaticaImage{
	private $layers;
	
	public function addLayer($chromaticaLayer,$offset_x,$offset_y){
		$chromaticaLayer->x = $offset_x;
		$chromaticaLayer->y = $offset_y;
		$this->layers[] = $chromaticaLayer;
	}
	public function crunch(){
		print_r($this->layers);
		foreach($this->layers as $layer){
			$oLayer = $layer->getGDObject();
		}
	}
}
