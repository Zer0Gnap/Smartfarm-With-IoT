const bodyParser = require('body-parser')
const request = require('request')
const express = require('express')

const mysql = require('mysql');
const MicroGear = require('microgear');

const APPID  = "SmartFarm29467";
const KEY    = "77vgrLmQXPZWWGN";
const SECRET = "FFUJ36crE92y5KttB1OKJBXcE";

let tem_msg = `Undi`
let hum_msg = `Undi`
var shuff = 0

var con = mysql.createConnection({
  host: "edu1.pathosting.com",
  user: "smartfarmk_db",
  password: "Smartfarm1234",
  database: "smartfarmk_db"
});

var microgear = MicroGear.create({
    key : KEY,
    secret : SECRET
});

microgear.on('connected', function() {
    console.log('Connected...');
    microgear.setAlias("Nodejs");
    microgear.subscribe("/Temperature");
    microgear.subscribe("/Humidity");

});

const app = express()
const port = process.env.PORT || 3000
const HEADERS = {
	'Content-Type': 'application/json',
	'Authorization': 'Bearer d8qK4eURB3iyjc/69bvmamvfsXg1Gai6YPuD8xaXMOlRDkoAUI4q3YXNmozn5AaFuCTmdvHhUSBmHSFzEiC7c2iDQaNGYPLlRBZze3xu6++klVSLThVf0K75Dyu7kgv7X4v6l+sE+d3QmPzAzoMOKAdB04t89/1O/w1cDnyilFU='
}

var pIn = 0 , pOut = 0;

app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
// Push
app.get('/webhook', (req, res) => {
	// push block
	res.sendStatus(200);
})

microgear.on('message', function(topic,body) {
  if(topic == '/SmartFarm29467/Temperature' && shuff == 0)
  {
    tem_msg = body.toString('utf8')
    var values = parseFloat(tem_msg)
    shuff = 1
  }
  else if(topic == '/SmartFarm29467/Humidity' && shuff == 1) {
    hum_msg = body.toString('utf8')
    shuff = 0
  }
});

microgear.on("present", function(event) {
  if(event.type == "online" || event.type == "offline"){
    notify_curl("\nDevice : " + event.alias + " Statis : " + event.type)
  }
});

// Reply
app.post('/webhook', (req, res) => {
	// reply block
	console.log(req.body.events);
	let reply_token = req.body.events[0].replyToken;
  let bc = JSON.stringify(req.body.events[0]);
	if(req.body.events[0].type == "message" && req.body.events[0].message.text == "my_device")
	{
		reply(reply_token,bc)
	}
	else if(req.body.events[0].type == "beacon")
	{
    if(req.body.events[0].beacon.type == "enter")
		{
        microgear.publish("/check","1");
        let msg = `Air Tempurature = ` + tem_msg + ` C\n` + `Air Humidity = ` + hum_msg + ` %`;
        reply(reply_token , msg);
		}
		else {
      reply(reply_token , "See you next time");
		}
	}
  else if(req.body.events[0].message.text.slice(0, 11) == "set soil = "){
    microgear.publish("/Set_Soil",req.body.events[0].message.text.slice(11),true);
    dbUpdate('SOILSETTING', req.body.events[0].message.text.slice(11))
    dbInsert('SOILSETTING', req.body.events[0].message.text.slice(11))
    microgear.publish("/set_state","1");
    reply(reply_token , "Soil moisture setting successful");
  }
  // Warn
  else if(req.body.events[0].message.text.slice(0, 16) == "set temp warn = "){
    microgear.publish("/Set_Warn_Temp",req.body.events[0].message.text.slice(16),true);
    dbUpdate('TEMPWSETTING', req.body.events[0].message.text.slice(16))
    dbInsert('TEMPWSETTING', req.body.events[0].message.text.slice(16))
    microgear.publish("/set_state","1");
    reply(reply_token , "Temperature warnning setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 16) == "set humi warn = "){
    microgear.publish("/Set_Warn_Humi",req.body.events[0].message.text.slice(16),true);
    dbUpdate('HUMIWSETTING', req.body.events[0].message.text.slice(16))
    dbInsert('HUMIWSETTING', req.body.events[0].message.text.slice(16))
    microgear.publish("/set_state","1");
    reply(reply_token , "Humidity warnning setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 16) == "set soil warn = "){
    microgear.publish("/Set_Warn_Soil",req.body.events[0].message.text.slice(16),true);
    dbUpdate('SOILWSETTING', req.body.events[0].message.text.slice(16))
    dbInsert('SOILWSETTING', req.body.events[0].message.text.slice(16))
    microgear.publish("/set_state","1");
    reply(reply_token , "Soil moisture warnning setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 22) == "set water temp warn = "){
    microgear.publish("/Set_Warn_Water_Temp",req.body.events[0].message.text.slice(22),true);
    dbUpdate('WATERWSETTING', req.body.events[0].message.text.slice(22))
    dbInsert('WATERWSETTING', req.body.events[0].message.text.slice(22))
    microgear.publish("/set_state","1");
    reply(reply_token , "Water temperature warnning setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 21) == "set fert temp warn = "){
    microgear.publish("/Set_Warn_Fert_Temp",req.body.events[0].message.text.slice(21),true);
    dbUpdate('FERTWSETTING', req.body.events[0].message.text.slice(21))
    dbInsert('FERTWSETTING', req.body.events[0].message.text.slice(21))
    microgear.publish("/set_state","1");
    reply(reply_token , "Fertilizer temperature warnning setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 12) == "set light = "){
    microgear.publish("/Set_light",req.body.events[0].message.text.slice(12),true);
    dbUpdate('LIGHTSETTING', req.body.events[0].message.text.slice(12))
    dbInsert('LIGHTSETTING', req.body.events[0].message.text.slice(12))
    microgear.publish("/set_state","1");
    reply(reply_token , "Light setting successful");
  }
  else if(req.body.events[0].message.text.slice(0, 11) == "set fert = "){
    microgear.publish("/Set_fert",req.body.events[0].message.text.slice(11),true);
    dbUpdate('LIGHTSETTING', req.body.events[0].message.text.slice(11))
    dbInsert('LIGHTSETTING', req.body.events[0].message.text.slice(11))
    microgear.publish("/set_state","1");
    reply(reply_token , "Fertilizer setting successful");
  }
  else if(req.body.events[0].message.text == "menu"){
    reply(reply_token , "Setting Value Menu\n set soil = XX - XX\n set light = XX - XX\n set fert = dd - hh - mm\nNotification Menu\n  set temp warn = XX.XX or off\n  set humi warn = XX or off\n  set soil warn = XX\n  set water temp warn = XX.XX or off\n  set fert temp warn = XX.XX or off");
  }
  else if(req.body.events[0].message.text == "test"){
    push("TESO op")
  }
  else {
    reply(reply_token , "Try agian");
  }
	res.sendStatus(200);
})

function dbInsert(table_name, value){
  con.connect(function(err) {
    if (err) throw err;
    console.log("Connected!");
    var sql = "INSERT INTO " + table_name + " (value) VALUES ('" + value + "')";
    con.query(sql, function (err, result) {
      if (err) throw err;
      console.log("1 record inserted");
    });
  });
}

function dbUpdate(table_name, value){
  con.connect(function(err) {
    if (err) throw err;
    console.log("Connected!");
    var sql = "UPDATE " + table_name + " SET value = '" + value + "' WHERE id = 0";
    con.query(sql, function (err, result) {
      if (err) throw err;
      console.log("1 record inserted");
    });
  });
}

function push(msg) {
	let body = JSON.stringify({
	// push body
	to: 'Ub5e38f5a3e025c99bd8585516c15d8fb',
	messages: [
		{
			type : 'text',
			text : msg
		}
	]
  })
  curl('push',body)
}

function reply(reply_token, msg) {
	let body = JSON.stringify({
	// reply body
	replyToken: reply_token,
	messages: [
		{
			type : 'text',
			text : msg
		}
	]
  })
  curl('reply',body)
}

function curl(method, body) {
	request.post({
		url: 'https://api.line.me/v2/bot/message/' + method,
		headers: HEADERS,
		body: body
	}, (err, res, body) => {
		console.log('status = ' + res.statusCode)
	})
}

function notify_curl(msg) {
	request.post({
		url: 'https://notify-api.line.me/api/notify/',
		headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
    },
    auth: {
      bearer: 'ns9QoURN1e20zRi2zsOKRNKyf92KxXGXRm80Hn232eO', //token
    },
    form: {
      message: msg, //ข้อความที่จะส่ง
    },
  }, (err, res, body) => {
		console.log('status = ' + res.statusCode)
	})
}

app.listen(port, () => {
    console.log(`Our app is running on port ${ port }`);
})

microgear.on('closed', function() {
    console.log('Closed...');
});

microgear.connect(APPID);
