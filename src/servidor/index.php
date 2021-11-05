<?php

require 'util.php';

$dat = new ArchivoDat($dat_filename);

if (isset($_GET['action'])) {
    switch ($_GET['action']) {
    case "nuevo":
        $fn = nombre_archivo();
        $fp = fopen($fn, "wb");
        fclose($fp);

        $dat->resetear($fn);
        break;
    case "sumar":
        $dat->aumentar_aforo();
        $dat->loggear_datos();
        break;
    case "restar":
        $dat->reducir_aforo();
        $dat->loggear_datos();
        break;
    case "fiebre":
        if (isset($_GET['t'])) {
            enviar_mail(floatval($_GET['t']));
        }
        break;
    }
}

header('Content-Type: text/html');
echo $dat->get_aforo();
?>
