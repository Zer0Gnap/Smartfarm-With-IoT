<?php
require_once("config.php");
$temp = $_GET['Temp'];
$humi = $_GET['Humi'];
$soil = $_GET['Soil'];
$date_lo = $_GET['Date_log'];
$time_lo = $_GET['Time_log'];
$de = $_GET['De'];
$w = $_GET['W'];
$f = $_GET['F'];
$sqL = "INSER INTO temp (temp, humi, soil, date_log, time_log, device_id, water, fert) VALUES ('$temp','$humi', '$soil', '$date_lo', '$time_lo', '$de', '$w', '$f')";
$sql_query = mysql_query($sql);
if ($sql_query) {
    echo "Complete";
} else {
    echo "Error";
}
?>
