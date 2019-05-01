<?php
$host = 'localhost';
$dbname = 'smartfarmk_db';
$username = 'smartfarmk_db';
$password = 'Smartfarm1234';

session_start();

try {
	$db = new PDO('mysql:host=' . $host . ';dbname=' . $dbname, $username, $password);
} catch (PDOException $e) {
	exit($e->getMessage());
}
?>
