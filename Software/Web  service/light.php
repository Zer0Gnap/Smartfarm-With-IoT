<?php
require './conn.php';
$status = $_GET['Status'];
$date_lo = $_GET['Date_log'];
$time_lo = $_GET['Time_log'];
$q = $db->prepare("INSERT INTO `light`(`id`, `status`, `date`, `time`) VALUES (NULL,:s, :d, :t)");
$q->bindParam(':s', $status);
$q->bindParam(':d', $date_lo);
$q->bindParam(':t', $time_lo);
if ($q->execute()) {
    echo "Complete";
} else {
    echo 'error';
}
?>
