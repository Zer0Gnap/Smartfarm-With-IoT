<!DOCTYPE html>
<html lang="en">

<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <title>Smart Farm Login</title>
  <!-- plugins:css -->
  <link rel="stylesheet" href="vendors/iconfonts/mdi/css/materialdesignicons.min.css">
  <link rel="stylesheet" href="vendors/css/vendor.bundle.base.css">
  <link rel="stylesheet" href="vendors/css/vendor.bundle.addons.css">
  <!-- endinject -->
  <!-- plugin css for this page -->
  <!-- End plugin css for this page -->
  <!-- inject:css -->
  <link rel="stylesheet" href="css/style.css">
  <!-- endinject -->
  <link rel="shortcut icon" href="images/favicon.png" />
</head>

<body>
  <div class="container-scroller">
    <div class="container-fluid page-body-wrapper full-page-wrapper auth-page">
      <div class="content-wrapper d-flex align-items-center auth auth-bg-1 theme-one">
        <div class="row w-100">
          <div class="col-lg-4 mx-auto">
            <div class="auto-form-wrapper">
				<p class="card-title">Smart Farm Login</p>
              <?php

              require './config.php';

              if ($logged_in) {
              	header('Location: index.php');
              	exit();
              }

              if ($_POST) {

              	$user = $_POST['user'];
              	$pass = $_POST['pass'];

              	if (empty($user) || empty($pass)) {
              		echo 'FILL ALL PLZ';
              	} else {

              		$pass = sha1($pass);

              		$q = $db->prepare('select `id` from `account` where `username` = :u and `password` = :p');

              		$q->bindParam(':u', $user);
              		$q->bindParam(':p', $pass);

              		if ($q->execute()) {

              			if ($q->rowCount() == 1) {
              				//pass
              				$_SESSION['login'] = $user;
              				header('Location: dashboard.php');
              			} else {
              				//fail
              				echo 'Wrong';
              			}

              		} else {
              			echo 'DB ERROR!';
              		}

              	}

              }

              ?>

              <form action method="post">
                <div class="form-group">
                  <label class="label">Username</label>
                  <div class="input-group">
                    <input type="text" class="form-control" placeholder="Username" name="user">
                    <div class="input-group-append"><span class="input-group-text"> <em class="mdi mdi-check-circle"></em> </span></div>
                  </div>
                </div>
                <div class="form-group">
                  <label class="label">Password</label>
                  <div class="input-group">
                    <input type="password" class="form-control" placeholder="*********" name="pass">
                    <div class="input-group-append"><span class="input-group-text"> <em class="mdi mdi-check-circle"></em> </span></div>
                  </div>
                </div>
                <div class="form-group">
                  <button class="btn btn-primary submit-btn btn-block">Login</button>
                </div>
                <div class="form-group d-flex justify-content-between">

              </form>
            </div>
            <p class="text-primary">	copyright © 2019 Bootstrapdash. All rights reserved.</p>
          </div>
        </div>
      </div>

      <!-- content-wrapper ends -->
    </div>
    <!-- page-body-wrapper ends -->
  </div>
  <!-- container-scroller -->
  <!-- plugins:js -->
  <script src="../../vendors/js/vendor.bundle.base.js"></script>
  <script src="../../vendors/js/vendor.bundle.addons.js"></script>
  <!-- endinject -->
  <!-- inject:js -->
  <script src="../../js/off-canvas.js"></script>
  <script src="../../js/misc.js"></script>
  <!-- endinject -->
</body>

</html>
