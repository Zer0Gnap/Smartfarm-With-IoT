var MicroGear = require('microgear');

const APPID  = "SmartFarm29467";
const KEY    = "77vgrLmQXPZWWGN";
const SECRET = "FFUJ36crE92y5KttB1OKJBXcE";

let tem_msg = 'Undi'
let hum_msg = 'Undi'

var microgear = MicroGear.create({
    key : KEY,
    secret : SECRET
});

microgear.on('connected', function() {
    console.log('Connected...');
    microgear.setAlias("Nodejs2");
    microgear.subscribe("/Temperature");
    microgear.subscribe("/Humidity");

});

microgear.on('message', function(topic,body) {
  if(topic == '/SmartFarm29467/Temperature' && tem_msg == `Undi`)
  {
    tem_msg = body.toString('utf8')
    console.log(tem_msg);
  }
  else if(topic == '/SmartFarm29467/Humidity' && hum_msg == `Undi`) {
    hum_msg = body.toString('utf8')
    console.log(hum_msg);
  }
});


microgear.on('closed', function() {
    console.log('Closed...');
});


microgear.connect(APPID);
