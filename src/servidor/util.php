<?php

function nombre_archivo() {
    return "datos--".date('Y-m-d--H-i-s').".csv";
}

$dat_filename = "actual.dat";

class ArchivoDat {
    public $nombre;
    public $ruta;

    function __construct($nombre) {
        $this->nombre = $nombre;
        //$this->ruta = $_SERVER['DOCUMENT_ROOT'] . $nombre;
        $this->ruta = $nombre;
    }

    function get_datos() {
        $linea = fgets(fopen($this->ruta, 'r'));
        return explode(":", $linea);
    }

    function set_datos($aforo, $filename) {
        $fp = fopen($this->ruta, "w");
        fwrite($fp, strval($aforo).":".$filename);
        fclose($fp);
    }

    function get_aforo() {
        return intval($this->get_datos()[0]);
    }

    function get_filename_actual() {
        return $this->get_datos()[1];
    }

    function set_aforo($num) {
        $filename = $this->get_filename_actual();
        $this->set_datos($num, $filename);
    }

    function set_file($filename) {
        $aforo = $this->get_aforo();
        $this->set_datos($aforo, $filename);
    }

    function aumentar_aforo() {
        $this->set_aforo($this->get_aforo() + 1);
    }

    function reducir_aforo() {
        $this->set_aforo($this->get_aforo() - 1);
    }

    function resetear($filename) {
        $this->set_datos(0, $filename);
    }

    function loggear_datos() {
        $fp = fopen($this->get_filename_actual(), "a");
        fwrite($fp, date("c").",".strval($this->get_aforo())."\n");
        fclose($fp);
    }
}

?>
