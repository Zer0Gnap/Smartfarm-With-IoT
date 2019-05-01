<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <script src="https://cdn.netpie.io/microgear.js"></script>
        <title>NETPIE on HTML5 by MacTanomsup</title>
        <style>
            p  {
                font-size: 160%;
            }
        </style>
    </head>
    <body>
        <center>
         <p id="data"></p>

        </center>
    <script>
      const APPID     = 'SmartFarm29467';
      const APPKEY    = '77vgrLmQXPZWWGN';
      const APPSECRET = 'FFUJ36crE92y5KttB1OKJBXcE';

      var microgear = Microgear.create({
      key: APPKEY,
      secret: APPSECRET,
      alias : "myhtml"         /*  optional  */
      });

    	microgear.on('message',function(topic,msg) {
    		document.getElementById("data").innerHTML = msg;
    	});

    	microgear.on('connected', function() {
    		microgear.setAlias('htmlgear');    /* alias can be renamed anytime with this function */
    		document.getElementById("data").innerHTML = "Now I am connected with netpie...";
    		setInterval(function() {
    			microgear.chat("htmlgear","Hello from myself at "+Date.now());
    		},5000);
    	});

    	microgear.on('present', function(event) {
    		console.log(event);
    	});

    	microgear.on('absent', function(event) {
    		console.log(event);
    	});

    	microgear.connect(APPID);
    </script>
    </body>
</html>
