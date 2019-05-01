<?php
require './conn.php';
$de = $_GET['Device'];
$lux = $_GET['Lux'];
$date_lo = $_GET['Date_log'];
$time_lo = $_GET['Time_log'];
$q = $db->prepare('INSERT INTO `light_log`(`id`, `device_id`, `lux`, `date_log`, `time_log`) VALUES (NULL, :de, :l, :da, :t)');
$q->bindParam(':de', $de);
$q->bindParam(':l', $lux);
$q->bindParam(':da', $date_lo);
$q->bindParam(':t', $time_lo);
if ($q->execute()) {
    echo "Complete";
} else {
    echo "Error";
}
?>
