<meta charset="utf-8">
<?php

require './config.php';

if (!$logged_in) {
	header('Location: login.php');
	exit();
}

ini_set('safe_mode', false);
$apiKey = "04cb591c3d2263eb72aaaeb1259bfb87";
$cityId = "1606250";
$googleApiUrl = "http://api.openweathermap.org/data/2.5/weather?id=" . $cityId . "&lang=en&units=metric&APPID=" . $apiKey;
$url2 = "https://api.openweathermap.org/data/2.5/forecast?id=1606250&appid=04cb591c3d2263eb72aaaeb1259bfb87";


$ch = curl_init();

curl_setopt($ch, CURLOPT_HEADER, 0);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_URL, $googleApiUrl);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
curl_setopt($ch, CURLOPT_VERBOSE, 0);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
$response = curl_exec($ch);

curl_close($ch);
$data = json_decode($response);
$currentTime = time();

$ch2 = curl_init();

curl_setopt($ch2, CURLOPT_HEADER, 0);
curl_setopt($ch2, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch2, CURLOPT_URL, $url2);
curl_setopt($ch2, CURLOPT_FOLLOWLOCATION, 0);
curl_setopt($ch2, CURLOPT_VERBOSE, 0);
curl_setopt($ch2, CURLOPT_SSL_VERIFYPEER, false);
$response2 = curl_exec($ch2);

curl_close($ch2);
$data2 = json_decode($response2);

?>
<!DOCTYPE html>
<html lang="en">

<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <script src="https://netpie.io/public/netpieio/microgear.js"></script>
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <title>Smart Farm Dashboard</title>
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

	<style>
.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2196F3;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
</style>
</head>

<body onload="connect()">
  <div class="container-scroller">
    <!-- partial:partials/_navbar.html -->
    <nav class="navbar default-layout col-lg-12 col-12 p-0 fixed-top d-flex flex-row">
      <div class="text-center navbar-brand-wrapper d-flex align-items-top justify-content-center">
        <a class="navbar-brand brand-logo" href="index.html">
          <img src="images/logo.svg" alt="logo" />
        </a>
        <a class="navbar-brand brand-logo-mini" href="index.html">
          <img src="images/logo-mini.svg" alt="logo" />
        </a>
      </div>
      <div class="navbar-menu-wrapper d-flex align-items-center">
        <ul class="navbar-nav navbar-nav-right">
          <li class="nav-item dropdown">
            <div class="dropdown-menu dropdown-menu-right navbar-dropdown preview-list" aria-labelledby="messageDropdown">
              <div class="dropdown-item">
                <p class="mb-0 font-weight-normal float-left">You have 7 unread mails
                </p>
                <span class="badge badge-info badge-pill float-right">View all</span>
              </div>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <img src="images/faces/face4.jpg" alt="image" class="profile-pic">
                </div>
                <div class="preview-item-content flex-grow">
                  <h6 class="preview-subject ellipsis font-weight-medium text-dark">David Grey
                    <span class="float-right font-weight-light small-text">1 Minutes ago</span>
                  </h6>
                  <p class="font-weight-light small-text">
                    The meeting is cancelled
                  </p>
                </div>
              </a>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <img src="images/faces/face2.jpg" alt="image" class="profile-pic">
                </div>
                <div class="preview-item-content flex-grow">
                  <h6 class="preview-subject ellipsis font-weight-medium text-dark">Tim Cook
                    <span class="float-right font-weight-light small-text">15 Minutes ago</span>
                  </h6>
                  <p class="font-weight-light small-text">
                    New product launch
                  </p>
                </div>
              </a>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <img src="images/faces/face3.jpg" alt="image" class="profile-pic">
                </div>
                <div class="preview-item-content flex-grow">
                  <h6 class="preview-subject ellipsis font-weight-medium text-dark"> Johnson
                    <span class="float-right font-weight-light small-text">18 Minutes ago</span>
                  </h6>
                  <p class="font-weight-light small-text">
                    Upcoming board meeting
                  </p>
                </div>
              </a>
            </div>
          </li>
          <li class="nav-item dropdown">
            <a class="nav-link count-indicator dropdown-toggle" id="notificationDropdown" href="#" data-toggle="dropdown">
              <i class="mdi mdi-bell"></i>
              <span class="count">0</span>
            </a>
            <div class="dropdown-menu dropdown-menu-right navbar-dropdown preview-list" aria-labelledby="notificationDropdown">
              <a class="dropdown-item">
                <p class="mb-0 font-weight-normal float-left">You have 4 new notifications
                </p>
                <span class="badge badge-pill badge-warning float-right">View all</span>
              </a>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <div class="preview-icon bg-success">
                    <i class="mdi mdi-alert-circle-outline mx-0"></i>
                  </div>
                </div>
                <div class="preview-item-content">
                  <h6 class="preview-subject font-weight-medium text-dark">Application Error</h6>
                  <p class="font-weight-light small-text">
                    Just now
                  </p>
                </div>
              </a>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <div class="preview-icon bg-warning">
                    <i class="mdi mdi-comment-text-outline mx-0"></i>
                  </div>
                </div>
                <div class="preview-item-content">
                  <h6 class="preview-subject font-weight-medium text-dark">Settings</h6>
                  <p class="font-weight-light small-text">
                    Private message
                  </p>
                </div>
              </a>
              <div class="dropdown-divider"></div>
              <a class="dropdown-item preview-item">
                <div class="preview-thumbnail">
                  <div class="preview-icon bg-info">
                    <i class="mdi mdi-email-outline mx-0"></i>
                  </div>
                </div>
                <div class="preview-item-content">
                  <h6 class="preview-subject font-weight-medium text-dark">New user registration</h6>
                  <p class="font-weight-light small-text">
                    2 days ago
                  </p>
                </div>
              </a>
            </div>
          </li>
          <li class="nav-item dropdown d-none d-xl-inline-block">
            <a class="nav-link dropdown-toggle" id="UserDropdown" href="#" data-toggle="dropdown" aria-expanded="false">
              <span class="profile-text">Hello, <?php echo $login_data['name']; ?> !</span>
              <img class="img-xs rounded-circle" src="images/faces/facr99.jpg" alt="Profile image">
            </a>
            <div class="dropdown-menu dropdown-menu-right navbar-dropdown" aria-labelledby="UserDropdown">
              <a class="dropdown-item p-0">
              </a>
              <a class="dropdown-item mt-2">
                Manage Accounts
              </a>
              <a class="dropdown-item" href="./logout.php">
                Sign Out
              </a>
            </div>
          </li>
        </ul>
        <button class="navbar-toggler navbar-toggler-right d-lg-none align-self-center" type="button" data-toggle="offcanvas">
          <span class="mdi mdi-menu"></span>
        </button>
      </div>
    </nav>
    <!-- partial -->
    <div class="container-fluid page-body-wrapper">
      <!-- partial:partials/_sidebar.html -->
      <nav class="sidebar sidebar-offcanvas" id="sidebar">
        <ul class="nav">
          <li class="nav-item nav-profile">
            <div class="nav-link">
              <div class="user-wrapper">
                <div class="profile-image">
                  <img src="images/faces/facr99.jpg" alt="profile image">
                </div>
                <div class="text-wrapper">
                  <p class="profile-name"><?php echo $login_data['name']; ?></p>
                  <div>
                    <small class="designation text-muted">Admin</small>
                    <span class="status-indicator online"></span>
                  </div>
                </div>
              </div>
            </div>
          </li>
          <li class="nav-item">
            <a class="nav-link" href="dashboard.php">
              <i class="menu-icon mdi mdi-television"></i>
              <span class="menu-title">Dashboard</span>
            </a>
          </li>

        </ul>
      </nav>
      <!-- partial -->
      <div class="main-panel">
        <div class="content-wrapper">

          <div class="row">
            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/sprout.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Soil moisture</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id="getSoil"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> <em class="mdi mdi-alert-octagon mr-1" aria-hidden="true" id="data"></em></p>-->
                </div>
              </div>
            </div>
            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/temperature.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Temperature</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id="getTemp"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>
            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/dew-point.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Humidity</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id="getHum"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>
            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/barometer-gauge.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Pressure</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id = "getPres"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>


            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/water.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Water temp</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id="getWaterTemp"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>
            <div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/thin-test-tube.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Fertilizer temp</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id = "getFerTemp"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>
			<div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/light.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Light</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id = "getLight"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>
			<div class="col-xl-3 col-lg-3 col-md-3 col-sm-6 grid-margin stretch-card">
              <div class="card card-statistics">
                <div class="card-body">
                  <div class="clearfix">
                    <div class="float-left">
                      <img src="https://img.icons8.com/color/100/000000/water-element.png">
                    </div>
                    <div class="float-right">
                      <p class="mb-0 text-right">Water Level</p>
                      <div class="fluid-container">
                        <h3 class="font-weight-medium text-right mb-0" id = "getUltrasonic_water"></h3>
                      </div>
                    </div>
                  </div>
                  <!--<p class="text-muted mt-3 mb-0"> </p>-->
                </div>
              </div>
            </div>  
          </div>
          <div class="row">
            <div class="col-lg-7 grid-margin stretch-card">
              <!--weather card-->
              <div class="card card-weather">
                <div class="card-body">
                  <div class="weather-date-location">
                    <h3 id="get_day"></h3>
                    <p class="text-gray">
                      <span class="weather-date" id="get_dd"></span>
                      <span class="weather-location"><?php echo $data->name; ?>,TH</span>
                    </p>
                  </div>
                  <div class="weather-data d-flex">
                    <div class="mr-auto">
                      <h4 class="display-3"><?php echo $data->main->temp; ?>
                        <span class="symbol">&deg;</span>C</h4>
                      <p>
                        <?php echo $data->weather[0]->description; ?>
                      </p>
                    </div>
                  </div>
                </div>
                <div class="card-body p-0">
                  <div class="d-flex weakly-weather">
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[2]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
					  <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[2]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[2]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[3]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[3]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[3]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[4]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[4]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[4]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[5]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[4]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[5]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[6]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[6]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[6]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[7]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[7]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[7]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                    <div class="weakly-weather-item">
                      <p class="mb-1">
                        <?php $arr=str_split($data2->list[8]->dt_txt);
						  echo $arr[11].$arr[12].$arr[13].$arr[14].$arr[15];
						  ?>
                      </p>
                      <img
						src="http://openweathermap.org/img/w/<?php echo $data2->list[8]->weather[0]->icon; ?>.png"
						class="weather-icon" />
                      <p class="mb-0">
                        <?php echo $data2->list[8]->main->temp - 273.15; ?>°
                      </p>
                    </div>
                  </div>
                </div>
              </div>
              <!--weather card ends-->
            </div>
            <div class="col-lg-5 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h2 class="card-title text-primary mb-5">Switch Controller</h2>
                  <div class="wrapper d-flex justify-content-between">
                    <div class="side-left">
                      <p class="mb-2">Water pump</p>
                      <label class="switch">
  						<input type="checkbox" onclick="if(this.checked){wFunc(1)}else{wFunc(0)}">
						<span class="slider round"></span>
					  </label>
                    </div>
                  </div>
									<div class="wrapper d-flex justify-content-between">
				                    <div class="side-left">
				                      <p class="mb-2">Fertilizer pump</p>
				                      <label class="switch">
				  						<input type="checkbox" onclick="if(this.checked){fFunc(1)}else{fFunc(0)}">
										<span class="slider round"></span>
									  </label>
				                    </div>
				                  </div>


                  <div class="wrapper d-flex justify-content-between">
                    <div class="side-left">
                      <p class="mb-2">Light</p>
                      <label class="switch">
  						<input type="checkbox" onclick="if(this.checked){lFunc(1)}else{lFunc(0)}">
						<span class="slider round"></span>
					  </label>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-lg-6 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Temperature</h4>
                  <canvas id="lineChart_temp" style="height:250px"></canvas>
                </div>
              </div>
            </div>
            <div class="col-lg-6 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Humidity</h4>
                  <canvas id="lineChart_humi" style="height:230px"></canvas>
                </div>
              </div>
            </div>
			<div class="col-lg-6 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Soil moisture</h4>
                  <canvas id="lineChart_soil" style="height:230px"></canvas>
                </div>
              </div>
            </div>
			<div class="col-lg-6 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Water temp</h4>
                  <canvas id="lineChart_water" style="height:230px"></canvas>
                </div>
              </div>
            </div>
			<div class="col-lg-6 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Fertilizer temp</h4>
                  <canvas id="lineChart_fert" style="height:230px"></canvas>
                </div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-lg-12 grid-margin">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Data Log (6 Hours ago.)</h4>
					<?php
						$q = $db->query('select * from `temp` order by id DESC LIMIT 7')->fetchAll(PDO::FETCH_ASSOC);
					?>
                  <div class="table-d">
                    <table class="table table-bordered">
                      <thead>
                        <tr>
                          <th>
                            #
                          </th>
                          <th>
                            Temperature
                          </th>
                          <th>
                            Humidity
                          </th>
                          <th>
                            Soil
                          </th>
                          <th>
                            Water Temp
                          </th>
                          <th>
                            Fert Temp
                          </th>
                        </tr>
                      </thead>
                      <tbody>

						  <?php
						  	$counter = 1;
						  	//foreach ($q as $row) {
							for ($i = 0; $i < 6; $i++){

                        echo '<tr>';

                          echo '<td class="font-weight-medium">' . $counter . '</td>';

                          if(intval($q[$i]['temp']) < intval($q[$i+1]['temp'])) {
							  echo '<td class="text-success">' . $q[$i]['temp'] . '&#176;C
                            <i class="mdi mdi-arrow-down">'.'</i>'
                          .'</td>';
						  }
						  else if(intval($q[$i]['temp']) > intval($q[$i+1]['temp'])){
							  echo '<td class="text-danger">' . $q[$i]['temp'] . '&#176;C
                            <i class="mdi mdi-arrow-up">'.'</i>'
                          .'</td>';
						  }
						  else{
							  echo '<td>' . $q[$i]['temp'] . '&#176;C</td>';
						  }


						  if(intval($q[$i]['humi']) < intval($q[$i+1]['humi'])) {
							  echo '<td class="text-success">' . $q[$i]['humi'] . '%
                            <i class="mdi mdi-arrow-down">'.'</i>'
                          .'</td>';
						  }
						  else if(intval($q[$i]['humi']) > intval($q[$i+1]['humi'])){
							  echo '<td class="text-danger">' . $q[$i]['humi'] . '%
                            <i class="mdi mdi-arrow-up">'.'</i>'
                          .'</td>';
						  }
						  else{
							  echo '<td>' . $q[$i]['humi'] . '%</td>';
						  }


							if(intval($q[$i]['soil']) < intval($q[$i+1]['soil'])) {
							  echo '<td class="text-success">' . $q[$i]['soil'] . '%
                            <i class="mdi mdi-arrow-down">'.'</i>'
                          .'</td>';
						  }
						  else if(intval($q[$i]['soil']) > intval($q[$i+1]['soil'])){
							  echo '<td class="text-danger">' . $q[$i]['soil'] . '%
                            <i class="mdi mdi-arrow-up">'.'</i>'
                          .'</td>';
						  }
						  else{
							  echo '<td>' . $q[$i]['soil'] . '%</td>';
						  }

						if(intval($q[$i]['water']) < intval($q[$i+1]['water'])) {
							  echo '<td class="text-success">' . $q[$i]['water'] . '%
                            <i class="mdi mdi-arrow-down">'.'</i>'
                          .'</td>';
						  }
						  else if(intval($q[$i]['water']) > intval($q[$i+1]['water'])){
							  echo '<td class="text-danger">' . $q[$i]['water'] . '%
                          <i class="mdi mdi-arrow-up">'.'</i>'
                          .'</td>';
						  }
						  else{
							  echo '<td>' . $q[$i]['water'] . '%</td>';
						  }

						if(intval($q[$i]['fert']) < intval($q[$i+1]['fert'])) {
							  echo '<td class="text-success">' . $q[$i]['fert'] . '%
                            <i class="mdi mdi-arrow-down">'.'</i>'
                          .'</td>';
						  }
						  else if(intval($q[$i]['fert']) > intval($q[$i+1]['fert'])){
							  echo '<td class="text-danger">' . $q[$i]['fert'] . '%
                            <i class="mdi mdi-arrow-up">'.'</i>'
                          .'</td>';
						  }
						  else{
							  echo '<td>' . $q[$i]['fert'] . '%</td>';
						  }

                        echo '</tr>';
							 $counter++;
							}
							?>
                      </tbody>
                    </table>
                  </div>
                </div>
              </div>
            </div>
          </div>

        </div>
        <!-- content-wrapper ends -->
        <!-- partial:partials/_footer.html -->
        <footer class="footer">
          <div class="container-fluid clearfix">
            <span class="text-muted d-block text-center text-sm-left d-sm-inline-block">Copyright © 2019
              Smartfarmkusrc.lnw.mn. All rights reserved. Powered by <a href="http://www.bootstrapdash.com/" target="_blank">Bootstrapdash</a>.Icon pack by icons8</span>
          </div>
        </footer>
        <!-- partial -->
      </div>
      <!-- main-panel ends -->
    </div>
    <!-- page-body-wrapper ends -->
  </div>
  <!-- container-scroller -->

  <!-- plugins:js -->
  <script src="vendors/js/vendor.bundle.base.js"></script>
  <script src="vendors/js/vendor.bundle.addons.js"></script>
  <!-- endinject -->
  <!-- Plugin js for this page-->
  <!-- End plugin js for this page-->
  <!-- inject:js -->
  <script src="js/off-canvas.js"></script>
  <script src="js/misc.js"></script>
  <script>
$(function() {
  /* ChartJS
   * -------
   * Data and config for chartjs
   */
  'use strict';
  var data = {
    labels: ["6", "5", "4", "3", "2", "1"],
    datasets: [{
      label: '# of Votes',
      data: [<?php  echo $q[5]['temp'];?>,<?php  echo $q[4]['temp'];?>,<?php  echo $q[3]['temp'];?>,<?php  echo $q[2]['temp'];?>,<?php  echo $q[1]['temp'];?>,<?php  echo $q[0]['temp'];?> ],
      backgroundColor: [
        'rgba(255, 99, 132, 0.2)',
        'rgba(54, 162, 235, 0.2)',
        'rgba(255, 206, 86, 0.2)',
        'rgba(75, 192, 192, 0.2)',
        'rgba(153, 102, 255, 0.2)',
        'rgba(255, 159, 64, 0.2)'
      ],
      borderColor: [
        'rgba(255,99,132,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(153, 102, 255, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderWidth: 1
    }]
  };

  var data2 = {
    labels: ["6", "5", "4", "3", "2", "1"],
    datasets: [{
      label: '# of Votes',
      data: [<?php  echo $q[5]['humi'];?>,<?php  echo $q[4]['humi'];?>,<?php  echo $q[3]['humi'];?>,<?php  echo $q[2]['humi'];?>,<?php  echo $q[1]['humi'];?>,<?php  echo $q[0]['humi'];?> ],
      backgroundColor: [
        'rgba(0, 204, 102, 0.2)',
        'rgba(54, 162, 235, 0.2)',
        'rgba(255, 206, 86, 0.2)',
        'rgba(75, 192, 192, 0.2)',
        'rgba(153, 102, 255, 0.2)',
        'rgba(255, 159, 64, 0.2)'
      ],
      borderColor: [
        'rgba(0,204,0,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(153, 102, 255, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderWidth: 1
    }]
  };

  var data3 = {
    labels: ["6", "5", "4", "3", "2", "1"],
    datasets: [{
      label: '# of Votes',
      data: [<?php  echo $q[5]['soil'];?>,<?php  echo $q[4]['soil'];?>,<?php  echo $q[3]['soil'];?>,<?php  echo $q[2]['soil'];?>,<?php  echo $q[1]['soil'];?>,<?php  echo $q[0]['soil'];?> ],
      backgroundColor: [
        'rgba(204, 153, 0, 0.2)',
        'rgba(54, 162, 235, 0.2)',
        'rgba(255, 206, 86, 0.2)',
        'rgba(75, 192, 192, 0.2)',
        'rgba(153, 102, 255, 0.2)',
        'rgba(255, 159, 64, 0.2)'
      ],
      borderColor: [
        'rgba(153,102,51,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(153, 102, 255, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderWidth: 1
    }]
  };

  var data4 = {
    labels: ["6", "5", "4", "3", "2", "1"],
    datasets: [{
      label: '# of Votes',
      data: [<?php  echo $q[5]['water'];?>,<?php  echo $q[4]['water'];?>,<?php  echo $q[3]['water'];?>,<?php  echo $q[2]['water'];?>,<?php  echo $q[1]['water'];?>,<?php  echo $q[0]['water'];?> ],
      backgroundColor: [
        'rgba(51, 204, 255, 0.2)',
        'rgba(54, 162, 235, 0.2)',
        'rgba(255, 206, 86, 0.2)',
        'rgba(75, 192, 192, 0.2)',
        'rgba(153, 102, 255, 0.2)',
        'rgba(255, 159, 64, 0.2)'
      ],
      borderColor: [
        'rgba(0,204,255,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(153, 102, 255, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderWidth: 1
    }]
  };

  var data5 = {
    labels: ["6", "5", "4", "3", "2", "1"],
    datasets: [{
      label: '# of Votes',
      data: [<?php  echo $q[5]['fert'];?>,<?php  echo $q[4]['fert'];?>,<?php  echo $q[3]['fert'];?>,<?php  echo $q[2]['fert'];?>,<?php  echo $q[1]['fert'];?>,<?php  echo $q[0]['fert'];?> ],
      backgroundColor: [
        'rgba(204, 153, 255, 0.2)',
        'rgba(54, 162, 235, 0.2)',
        'rgba(255, 206, 86, 0.2)',
        'rgba(75, 192, 192, 0.2)',
        'rgba(153, 102, 255, 0.2)',
        'rgba(255, 159, 64, 0.2)'
      ],
      borderColor: [
        'rgba(204,102,255,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(153, 102, 255, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderWidth: 1
    }]
  };

  var options = {
    scales: {
      yAxes: [{
        ticks: {
          beginAtZero: true
        }
      }]
    },
    legend: {
      display: false
    },
    elements: {
      point: {
        radius: 0
      }
    }

  };


  var browserTrafficData = {
    datasets: [{
      data: [20, 20, 10, 30, 20],
      backgroundColor: [
        'rgba(255,99,132,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(75, 192, 117, 1)',
        'rgba(255, 159, 64, 1)'
      ],
      borderColor: [
        'rgba(255,99,132,1)',
        'rgba(54, 162, 235, 1)',
        'rgba(255, 206, 86, 1)',
        'rgba(75, 192, 192, 1)',
        'rgba(75, 192, 117, 1)',
        'rgba(255, 159, 64, 1)'
      ],
    }],

    // These labels appear in the legend and in the tooltips when hovering different arcs
    labels: [
      'Firefox',
      'Safari',
      'Explorer',
      'Chrome',
      'Opera Mini'
    ]
  };
  // Get context with jQuery - using jQuery's .get() method.

  if ($("#lineChart_temp").length) {
    var lineChartCanvas = $("#lineChart_temp").get(0).getContext("2d");
    var lineChart = new Chart(lineChartCanvas, {
      type: 'line',
      data: data,
      options: options
    });
  }

  if ($("#lineChart_humi").length) {
    var lineChartCanvas = $("#lineChart_humi").get(0).getContext("2d");
    var lineChart = new Chart(lineChartCanvas, {
      type: 'line',
      data: data2,
      options: options
    });
  }
  if ($("#lineChart_soil").length) {
    var lineChartCanvas = $("#lineChart_soil").get(0).getContext("2d");
    var lineChart = new Chart(lineChartCanvas, {
      type: 'line',
      data: data3,
      options: options
    });
  }
  if ($("#lineChart_water").length) {
    var lineChartCanvas = $("#lineChart_water").get(0).getContext("2d");
    var lineChart = new Chart(lineChartCanvas, {
      type: 'line',
      data: data4,
      options: options
    });
  }
  if ($("#lineChart_fert").length) {
    var lineChartCanvas = $("#lineChart_fert").get(0).getContext("2d");
    var lineChart = new Chart(lineChartCanvas, {
      type: 'line',
      data: data5,
      options: options
    });
  }

});
</script>
  <!-- endinject -->
  <!-- Custom js for this page-->
  <script src="js/dashboard.js"></script>
  <!-- End custom js for this page-->
  <script>
      const APPID     = 'SmartFarm29467';
      const APPKEY    = '77vgrLmQXPZWWGN';
      const APPSECRET = 'FFUJ36crE92y5KttB1OKJBXcE';
			const ALIAS = "webapp";

      var microgear = Microgear.create({
      key: APPKEY,
      secret: APPSECRET,
			alias : ALIAS
      });

    	microgear.on('message',function(topic,msg) {
    		//document.getElementById("data").innerHTML = topic+msg;
			if (topic == "/SmartFarm29467/Soil Meter") {
               document.getElementById("getSoil").innerHTML = msg + ' %';
             }
            else if (topic == "/SmartFarm29467/Temperature") {
               document.getElementById("getTemp").innerHTML = msg+' &#176;C';
             }
            else if (topic == "/SmartFarm29467/Pressure") {
               document.getElementById("getPres").innerHTML = msg + ' hPa';
             }
              else if (topic == "/SmartFarm29467/Humidity") {
                document.getElementById("getHum").innerHTML = msg + ' %';
              }
              else if (topic == "/SmartFarm29467/Water_temp") {
                document.getElementById("getWaterTemp").innerHTML = msg+' &#176;C';
              }
              else if (topic == "/SmartFarm29467/Fertilizer_temp") {
                document.getElementById("getFerTemp").innerHTML = msg+' &#176;C';
              }
			  else if (topic == "/SmartFarm29467/Light") {
                document.getElementById("getLight").innerHTML = msg+' Lux';
              }
			  else if (topic == "/SmartFarm29467/Ultrasonic_water") {
	            document.getElementById("getUltrasonic_water").innerHTML = msg+' %';
    	}});

    	microgear.on('connected', function() {
    		//document.getElementById("data").innerHTML = "Now I am connected with netpie...";
			microgear.subscribe("/Soil Meter");
            microgear.subscribe("/Temperature");
            microgear.subscribe("/Pressure");
            microgear.subscribe("/Humidity");
            microgear.subscribe("/Water_temp");
            microgear.subscribe("/Fertilizer_temp");
						microgear.subscribe("/Light");
						microgear.subscribe("/Ultrasonic_water");
//			microgear.subscribe("/Fertilizer_temp");
//    		setInterval(function() {
//    		},5000);
    	});

    	microgear.on('present', function(event) {
    		console.log(event);
    	});

    	microgear.on('absent', function(event) {
    		console.log(event);
    	});

    	function connect() {
          microgear.connect(APPID);
        }
</script>
<script>
	function wFunc(a) {
		if(a === 1){
			microgear.publish("/wpump","1");
		} else if(a === 0) {
			microgear.publish("/wpump","0");
		}
	}
	function fFunc(a) {
		if(a === 1){
			microgear.publish("/fpump","1");
		} else if(a === 0) {
			microgear.publish("/fpump","0");
		}
	}
	function lFunc(a) {
		if(a === 1){
  			microgear.publish("/Set_togg_light","1");
		} else if(a === 0) {
			microgear.publish("/Set_togg_light","0");
		}
	}
</script>
<script>
  var d = new Date();
  var weekday = new Array(7);
  weekday[0] = "Sunday";
  weekday[1] = "Monday";
  weekday[2] = "Tuesday";
  weekday[3] = "Wednesday";
  weekday[4] = "Thursday";
  weekday[5] = "Friday";
  weekday[6] = "Saturday";

  var month = new Array();
  month[0] = "January";
  month[1] = "February";
  month[2] = "March";
  month[3] = "April";
  month[4] = "May";
  month[5] = "June";
  month[6] = "July";
  month[7] = "August";
  month[8] = "September";
  month[9] = "October";
  month[10] = "November";
  month[11] = "December";
  var n = weekday[d.getDay()];
  var dd = d.getDate() + ' ' +month[d.getMonth()] + ' ' + d.getFullYear() + ' ';
  document.getElementById("get_day").innerHTML = n;
  document.getElementById("get_dd").innerHTML = dd;
</script>
</body>

</html>
