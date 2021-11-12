<?php

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\SMTP;

require 'vendor/autoload.php';

date_default_timezone_set("America/Santiago");

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
        fwrite($fp, date("Y/m/d H:i:s").",".strval($this->get_aforo())."\n");
        fclose($fp);
    }
}

function enviar_mail_fiebre($temp) {
    // Config para el correo
    $smtp_host =  "smtp.gmail.com";
    $smtp_port = 465;

    $smtp_user = "user@gmail.com";
    $smtp_username = "name";
    $smtp_pass = "pass";
    $smtp_dest = "dest@mail.com";

    $mail = new PHPMailer();

    $mail->isSMTP();
    //$mail->SMTPDebug = SMTP::DEBUG_SERVER;

    $mail->Host = $smtp_host;
    $mail->Port = $smtp_port;
    $mail->SMTPSecure = PHPMailer::ENCRYPTION_SMTPS;

    $mail->SMTPAuth = true;
    $mail->Username = $smtp_user;
    $mail->Password = $smtp_pass;

    $mail->setFrom($smtp_user, $smtp_username);

    //Set who the message is to be sent to
    $mail->addAddress($smtp_dest);

    //Set the subject line
    $mail->Subject = "Alerta de fiebre!";
    $mail->Body = "Ingreso de persona con fiebre\n";
    $mail->Body .= "Fecha: " . date("d/m/Y - h:i:sa") . "\n";
    $mail->Body .= "Temperatura: " . $temp . "°C";

    $mail->send();
}

?>
