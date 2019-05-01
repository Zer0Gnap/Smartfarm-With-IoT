<?php
require './conn.php';
$device = $_GET['Device_id'];
$code = $_GET['Code'];
$date_lo = $_GET['Date_log'];
$time_lo = $_GET['Time_log'];
$q = $db->prepare('INSERT INTO `notice`(`id`, `device_id`, `code`, `date_log`, `time_log`) VALUES (NULL, :de, :c, :da, :t)');
$q->bindParam(':de', $device);
$q->bindParam(':c', $code);
$q->bindParam(':da', $date_lo);
$q->bindParam(':t', $time_lo);
if ($q->execute()) {
    echo "Complete";
} else {
    echo "Error";
}
?>
