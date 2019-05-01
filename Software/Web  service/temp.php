<?php
require './conn.php';
$temp = $_GET['Temp'];
$humi = $_GET['Humi'];
$soil = $_GET['Soil'];
$date_lo = $_GET['Date_log'];
$time_lo = $_GET['Time_log'];
$de = $_GET['De'];
$w = $_GET['W'];
$f = $_GET['F'];
$q = $db->prepare('INSERT INTO `temp`(`id`, `device_id`, `temp`, `humi`, `soil`, `water`, `fert`, `date_log`, `time_log`) VALUES (NULL, :de, :te, :h, :s, :w, :f, :da, :t)');
$q->bindParam(':te', $temp);
$q->bindParam(':h', $humi);
$q->bindParam(':s', $soil);
$q->bindParam(':da', $date_lo);
$q->bindParam(':t', $time_lo);
$q->bindParam(':de', $de);
$q->bindParam(':w', $w);
$q->bindParam(':f', $f);
if ($q->execute()) {
    echo "Complete";
} else {
    echo "Error";
}
?>
