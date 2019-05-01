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

// functions

function user_data($username) {
	global $db;

	$q = $db->prepare('select * from `account` where `username` = :u');
	$q->bindParam(':u', $username);

	if ($q->execute()) {

		$r = $q->fetchAll(PDO::FETCH_ASSOC);

		if (isset($r[0])) {
			return $r[0];
		} else return false;

	} else return false;

}

// consts

$logged_in = !empty($_SESSION['login']);
$login_data = $logged_in ? user_data($_SESSION['login']) : false;

$distances = array(
	25, 50, 100, 200, 400, 800, 1500
);

$strokes = array(
	'Freestyle', 'Breastsroke', 'Butterfly', 'Backstroke'
);

$ranks = array(
	0 => array(
		'name' => 'User'
	),
	1 => array(
		'name' => 'Admin',
		'perms' => array(0 => 'show_all_user_stats')
	),
);

function check_perm($user_rank, $perm) {
	global $ranks;

	if (@in_array($perm, $ranks[$user_rank["rank"]]["perms"]))
		return true;
	return false;
}

//print_r($_SESSION);
