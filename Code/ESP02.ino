/* 
 *  
 * Smartfarm with IOT
 * Exploratory Project in Computing 03603299
 * Pirun Arunrungrasmi          6030300768
 * Raveepan Rueangkiattiwong    6030300873
 * Wichaivit Pattaramongkolchai 6030300946
 * 
 * 
*/
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MicroGear.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TridentTD_LineNotify.h>
#include <time.h>
#include <EEPROM.h>
#include <Ultrasonic.h>
/*
   EEPROM Addess
   0 = soilMin
   1 = soilMax
   2 = soilWarnMax
   3 = tempWarnMax
   4 = humiWarnMax
   5 = waterWarnMax
   6 = fertiWarnMax
   7 = countFertDay
   8 = fertDD
   9 = fertHH
   10 = fertMM
   13 = wpumpweb
   14 = fpumpweb
*/

/*Line Nofity*/
#define LINE_TOKEN "ns9QoURN1e20zRi2zsOKRNKyf92KxXGXRm80Hn232eO"

/*Wifi AP Mode*/
#define WIFI_AP_NAME "02ESP32 AP mode"
#define WIFI_AP_PASS "123456789"
WiFiServer server(80);
String header;

/*Wifi STA Mode*/
String mySSID = ""; //default
String myPSWD = "";
//String mySSID = "No Internet Access 2.4G"; // P
//String myPSWD = "AnTiB0tStr0ng#";
//String mySSID = "Redmi"; //Bank
//String myPSWD = "wichaivit29467";

/*Debug Noise*/
float dcBias = 1.0;
float simulatedVal = 0.0;
float filterConstant = 0.95;

/*Dallas*/
#define ONE_WIRE_BUS_1 25
#define ONE_WIRE_BUS_2 26
OneWire oneWire_in(ONE_WIRE_BUS_1);
OneWire oneWire_out(ONE_WIRE_BUS_2);
DallasTemperature sensor_inhouse(&oneWire_in);
DallasTemperature sensor_outhouse(&oneWire_out);

/* Min-Max Control*/
int soilMin, soilMax, soilWarnMax, countFertDay, fertDD, fertHH, fertMM, wpumpweb, fpumpweb;
float tempWarnMax, humiWarnMax, waterWarnMax, fertiWarnMax;
String water_temp, ferti_temp;
int map_low = 3527, map_high = 1577;

/*OLED Display*/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const unsigned char PROGMEM farmicon[] = {
  0x200, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc1, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x1f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe1, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x87, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x43, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x61, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x21, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x10, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x10, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x08, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x08, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x04, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x04, 0xf8, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0xe0, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0xc0, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x80, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x21, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x60, 0x83, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x70, 0x43, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x70, 0x21, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0xf8, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0xfc, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0xfe, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xc7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x81, 0x6e, 0x3c, 0x5f, 0x02, 0x0f, 0x15, 0x6e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x71, 0x91, 0x02, 0x64, 0x03, 0xe0, 0x99, 0x91, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x09, 0x11, 0x3e, 0x44, 0x02, 0x0f, 0x91, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x09, 0x11, 0x42, 0x44, 0x02, 0x10, 0x91, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x09, 0x11, 0x42, 0x44, 0x02, 0x10, 0x91, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf1, 0x11, 0x3e, 0x47, 0x02, 0x0f, 0x91, 0x11, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*NETPIE*/
#define APPID   "Your APPID"
#define KEY     "Your KEY"
#define SECRET  "Your SECRET KEY"
#define ALIAS   "Your NICKNAME Microgear"

"SmartFarm29467"
"77vgrLmQXPZWWGN"
"FFUJ36crE92y5KttB1OKJBXcE"
"02ESPino32"
WiFiClient client0; //Client0 use to HTML and NETPIE
MicroGear microgear(client0);
short int connectNetpie = 0, timer = 0;

/*HTML use Login WIFI STA*/
WiFiClient client1;

/*Time*/
int timezone = 7;
int dst = 0;
char ntp_server1[20] = "ntp.ku.ac.th";
char ntp_server2[20] = "fw.eng.ku.ac.th";
char ntp_server3[20] = "time.uni.net.th";

/*BME*/
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 16
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

/*Ultrasonic*/
Ultrasonic ultrasonic1(12, 13); //(trigPin , echoPin)
Ultrasonic ultrasonic2(27, 14); // (trigPin , echoPin)
int ultrasonicwater = 0;
int ultrasonicfert = 0;
/*Input Analog*/
float soilmeter = 0;
float temp_bme = 0;
float pres_bme = 0;
float alti_bme = 0;
float humi_bme = 0;
float lux = 0;

/*Hosting*/
#define host "smartfarmkusrc.lnw.mn"
short int httpPort = 80;
short int half = 99, halfwater = 0, halffart = 0;

/*Tesk*/
TaskHandle_t Task1;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  bme.begin();
  /*read EEPROM*/
  EEPROM.begin(15);
  soilMin = EEPROM.read(0);
  soilMax = EEPROM.read(1);
  soilWarnMax = EEPROM.read(2);
  tempWarnMax = EEPROM.read(3);
  humiWarnMax = EEPROM.read(4);
  waterWarnMax = EEPROM.read(5);
  fertiWarnMax = EEPROM.read(6);
  countFertDay = EEPROM.read(7);
  fertDD = EEPROM.read(8);
  fertHH = EEPROM.read(9);
  fertMM = EEPROM.read(10);
  wpumpweb = EEPROM.read(13);
  fpumpweb = EEPROM.read(14);

  /*Connect OLED Display & Logo*/
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, farmicon, 126, 62, WHITE);
  display.display();
  delay(3000);

  /*NETPIE Microgear*/
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  /*Wifi AP Mode*/
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASS);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 0);
  display.println("    WiFi Setting     \n");
  display.setTextColor(WHITE);
  display.print("1. Please connected\n   =");
  display.println(WIFI_AP_NAME);
  display.print("\n2. Open your Browser\n   =");
  display.println(WiFi.softAPIP());
  display.display();
  server.begin();
  while (mySSID == "") {
    myWeb();
  }
  WiFi.disconnect();

  /*Wifi STA Mode*/
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("WiFi ");
  display.display();

  Serial.println("SSID :" + mySSID);
  Serial.println("PASS :" + myPSWD);
  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID.c_str(), myPSWD.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.println("OK");
  display.print("NETPIE ");
  display.display();

  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
  connectNetpie = 1;
  display.println("OK");
  display.print("Time ");
  display.display();

  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  while (!time(nullptr)) {
    delay(500);
  }

  display.println("OK");
  display.display();
  LINE.setToken(LINE_TOKEN);
  LINE.notify("ทดสอบ");
  /*first input analog*/
  temp_bme = smooth(dcBias + bme.readTemperature(), filterConstant, temp_bme);
  pres_bme = smooth(dcBias + (bme.readPressure() / 100.0F), filterConstant, pres_bme);
  humi_bme = smooth(dcBias + bme.readHumidity(), filterConstant, humi_bme);
  soilmeter = map(analogRead(A0), map_low, map_high, 0, 100);
  sensor_inhouse.requestTemperatures();
  sensor_outhouse.requestTemperatures();
  water_temp = (String(sensor_inhouse.getTempCByIndex(0), 2)).c_str();
  ferti_temp = (String(sensor_outhouse.getTempCByIndex(0), 2)).c_str();
  ultrasonicwater = map(ultrasonic1.read(), 2, 400, 100, 0);
  ultrasonicfert = map(ultrasonic2.read(), 2, 400, 100, 0);

  /*GPIO*/
  pinMode(15, OUTPUT); //Water
  pinMode(02, OUTPUT);  //Fert
  digitalWrite(15, LOW);
  digitalWrite(02, LOW);
  //delay(1000);
  xTaskCreatePinnedToCore(IOT, "Task1", 10000, NULL, 0, &Task1, 0);
}

void loop() {
  if (microgear.connected()) {
    microgear.loop();
    connectNetpie = 1;
  }
  else {
    if (timer >= 5000) {
      Serial.println("connection lost, reconnect...");
      connectNetpie = 0;
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  temp_bme = smooth(dcBias + bme.readTemperature(), filterConstant, temp_bme);
  pres_bme = smooth(dcBias + (bme.readPressure() / 100.0F), filterConstant, pres_bme);
  humi_bme = smooth(dcBias + bme.readHumidity(), filterConstant, humi_bme);
  soilmeter = map(analogRead(A0), map_low, map_high, 0, 100);
  sensor_inhouse.requestTemperatures();
  sensor_outhouse.requestTemperatures();
  water_temp = (String(sensor_inhouse.getTempCByIndex(0), 2)).c_str();
  ferti_temp = (String(sensor_outhouse.getTempCByIndex(0), 2)).c_str();
  ultrasonicwater = map(ultrasonic1.read(), 2, 400, 100, 0);
  ultrasonicfert = map(ultrasonic2.read(), 2, 400, 100, 0);
  oled();
  controlDigital();
  delay(100);

}

/*OLED Display*/
void oled() {
  display.clearDisplay();
  display.setCursor(0, 0);
  if (WiFi.status() == WL_CONNECTED) {
    display.print("WiFi: OK  ");
    if (connectNetpie == 1) {
      display.println("NETPIE: OK");
    }
    else {
      display.println("NETPIE: -");
    }
    display.print(Date());
    display.print("  ");
    display.println(Time());
  }
  else {
    display.print("WiFi: -  ");
    display.println("NETPIE: -");
    display.print("0000-00-00  ");
    display.println("00:00:00");
  }
  display.println();
  display.print("Temp = ");
  display.print((String(temp_bme, 2)).c_str());
  display.println(" *C");
  display.print("Humi = ");
  display.print((String(humi_bme, 2)).c_str());
  display.println(" %");
  display.print("Soil = ");                                                                                                                                                  
  display.print((String((int)soilmeter)).c_str());
  display.println(" %");
  display.print("Water_T = ");
  display.print(water_temp);
  display.println(" *C");
  display.print("Ferti_T = ");
  display.print(ferti_temp);
  display.println(" *C");
  display.display();
}

/*Tesk*/
void IOT( void * parameter) {
  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      if (connectNetpie == 1) {
        microgear.publish("/Temperature", (String(temp_bme, 2)).c_str(), true);
        delay(100);
        microgear.publish("/Pressure", (String(pres_bme, 2)).c_str(), true);
        delay(100);
        microgear.publish("/Humidity", (String(humi_bme, 2)).c_str(), true);
        delay(100);
        microgear.publish("/Soil Meter", ((String((int)soilmeter)).c_str()), true);
        delay(100);
        microgear.publish("/Water_temp", water_temp, true);
        delay(100);
        microgear.publish("/Fertilizer_temp", ferti_temp, true);
        delay(100);
        microgear.publish("/Ultrasonic_water", ultrasonicwater, true);
        delay(100);
        microgear.publish("/Ultrasonic_fert", ultrasonicfert, true);
        delay(100);
        microgear.publish("/ButtonWater", wpumpweb, true);
        delay(100);
        microgear.publish("/ButtonFert", fpumpweb, true);
        delay(100);
      }
      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
      if (half == 99 && (newtime->tm_min == 0)) {
        if (!client1.connect(host, httpPort)) {
          Serial.println("connection failed");
        }
        else {
          half = 0;
          String url = "/temp.php?Temp=";
          //url += "25";
          url += (String(temp_bme, 2)).c_str();
          /*url += " Pres :";
            url += (String(pres_bme,2)).c_str();
            url += " Alti";
            url += (String(alti_bme,2)).c_str();
          */
          url += "&Humi=";
          url += (String(humi_bme, 2)).c_str();
          url += "&Soil=";
          url += (String((int)soilmeter)).c_str();
          url += "&Date_log=";
          url += Date();
          url += "&Time_log=";
          url += Time();
          url += "&De=";
          url += "02";
          url += "&W=";
          url += water_temp;
          url += "&F=";
          url += ferti_temp;
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n\r\n");
        }
      }
      else if (half != 99 && (newtime->tm_min != 0)) {
        half = 99;
      }
    }//end if (WiFi.status() == WL_CONNECTED)
  } //end while
}

/*NETPIE*/
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.println("Incoming message");
  msg[msglen] = '\0';
  String str = (char *)msg;
  Serial.println(str);
  //Serial.println(topic);
  String loc = (char *)topic;
  if (loc.equals("/SmartFarm29467/Set_Soil")) {
    String temp1, temp2;
    for (int i = 0; i < str.length(); i++) {
      //Serial.println(str.substring(i, i + 3));
      if (str.substring(i, i + 3) == " - ") {
        temp1 = str.substring(0, i);
        temp2 = str.substring(i + 3);
        break;
      }
    }
    Serial.print("My soil is ");
    Serial.println((char *)msg);
    Serial.println("My soil min is " + temp1);
    Serial.println("My soil max is " + temp2);
    soilMin = temp1.toInt();
    soilMax = temp2.toInt();
    EEPROM.write(0, soilMin);
    EEPROM.write(1, soilMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_Warn_Temp")) {
    if (str.equals("off")) {
      tempWarnMax = 255.00;
    } else {
      tempWarnMax = str.toFloat();
    }
    EEPROM.write(3, tempWarnMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_Warn_Soil")) {
    if (str.equals("off")) {
      soilWarnMax = 255;
    } else {
      soilWarnMax = str.toInt();
    }
    EEPROM.write(2, soilWarnMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_Warn_Water_Temp")) {
    if (str.equals("off")) {
      waterWarnMax = 255.00;
    } else {
      waterWarnMax = str.toFloat();
    }
    EEPROM.write(5, waterWarnMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_Warn_Fert_Temp")) {
    if (str.equals("off")) {
      fertiWarnMax = 255.00;
    } else {
      fertiWarnMax = str.toFloat();
    }
    EEPROM.write(6, fertiWarnMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_Warn_Humi")) {
    if (str.equals("off")) {
      humiWarnMax = 255.00;
    } else {
      humiWarnMax = str.toInt();
    }
    EEPROM.write(4, humiWarnMax);
  }
  else if (loc.equals("/SmartFarm29467/Set_fert")) {
    String temp[3];
    int q = 0;
    for (int i = 0; i < str.length();) {
      if ((char)str[i] == ' ') {
        i = i + 3;
        q++;
      }
      else {
        Serial.println(str[i] + " : " + i);
        temp[q] = temp[q] + str[i];
        i++;
      }
    }
    if (fertDD == temp[0].toInt() && fertHH == temp[1].toInt() && fertMM == temp[2].toInt()) {
      Serial.println("Repeat!");
    }
    else {
      Serial.println((char *)msg);
      Serial.println("My fertDay is " + temp[0]);
      Serial.println("My fertHH is " + temp[1]);
      Serial.println("My fertMM is " + temp[2]);
      fertDD = temp[0].toInt();
      fertHH = temp[1].toInt();
      fertMM = temp[2].toInt();
      countFertDay = 0;
      EEPROM.write(7, countFertDay);
      EEPROM.write(8, fertDD);
      EEPROM.write(9, fertHH);
      EEPROM.write(10, fertMM);
    }
  }
  else if (loc.equals("/SmartFarm29467/wpump")) {
    if (str.toInt() == 1) {
      wpumpweb = 1;
    } else {
      wpumpweb = 0;
    }
    EEPROM.write(13, wpumpweb);
  }
  else if (loc.equals("/SmartFarm29467/fpump")) {
    if (str.toInt() == 1) {
      fpumpweb = 1;
    } else {
      fpumpweb = 0;
    }
    EEPROM.write(14, fpumpweb);
  }
  EEPROM.commit();
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(ALIAS);
  if (microgear.connected()) {
    microgear.subscribe("/Set_Soil");
    microgear.subscribe("/Set_Warn_Temp");
    microgear.subscribe("/Set_Warn_Soil");
    microgear.subscribe("/Set_Warn_Humi");
    microgear.subscribe("/Set_Warn_Water_Temp");
    microgear.subscribe("/Set_Warn_Fert_Temp");
    microgear.subscribe("/Set_fert");
    microgear.subscribe("/wpump");
    microgear.subscribe("/fpump");
  }
}

/*HTML*/
int myWeb() {
  client0 = server.available();
  if (client0) {
    String currentLine = "";
    while (client0.connected()) {
      if (client0.available()) {
        char c = client0.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client0.println("HTTP/1.1 200 OK");
            client0.println("Content-type:text/html");
            client0.println("Connection: close");
            client0.println();
            client0.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><style>");
            client0.println("body{font-family: Arial,Helvetica, sans-serif;}");
            client0.println("from{border: 3px solid #f1f1f1;}");
            client0.println("input[type=text],input[type=password],select{width:100%;padding:12px 20px;margin: 8px 0;display: inline-block;border:1px solid #ccc;box-sizing:border-box;}");
            client0.println("button {background-color: #4CAF50;color: white;padding: 14px 20px;margin: 8px 0;border: none;cursor: pointer;width: 100%;}");
            client0.println("button:hover{opacity: 0.8;}");
            client0.println(".cancelbtn {width: auto;padding: 10px 18px;background-color: #f44336;}");
            client0.println(".imgcontainer {text-align: center;margin: 24px 0 12px 0;}");
            client0.println(".container {padding: 16px;}");
            client0.println("span.psw {float: right;padding-top: 16px;}");
            client0.println("@media screen and (max-width: 300px) {span.psw {display: block;float: none;}.cancelbtn {width: 100%;}}");
            client0.println("</style></head><body>");
            client0.println("<h2 style=\"text - align: center; \">ESP32 WiFi</h2>");
            client0.println("<form action=\"\">");
            client0.println("<div class=\"container\">");
            client0.println("<label for=\"ssid\"><b>SSID</b></label>");
            client0.println("<select name=\"ssid_select\">");
            int n = WiFi.scanNetworks();
            if (n == 0) {
              client0.println("<option value=\"#\">no networks found</option>");
            } else {
              client0.println("<option value=\"#\">Please select WiFi</option>");
              for (int i = 0; i < n; i++) {
                client0.print("<option value=\"");
                client0.print(WiFi.SSID(i));
                client0.print("\">");
                client0.print(WiFi.SSID(i));
                client0.println("</option>");
              }
            }
            client0.println("</select>");
            client0.println("<label for=\"psw\"><b>Password</b></label>");
            client0.println("<input type=\"password\" placeholder=\"Enter Password\" name=\"psw\">");
            client0.println("<button type=\"submit\">Login</button>");
            client0.println("</div></div></from></body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    String lmySSID = "";
    String lmyPSWD = "";
    for (int i = header.indexOf("ssid_select=") + 12; i < header.indexOf("&psw="); i++) {
      if (header[i] == '+') {
        lmySSID += " ";
      }
      else {
        lmySSID += header[i];
      }
    }
    for (int j = header.indexOf("&psw=") + 5; j < header.indexOf(" HTTP/1.1"); j++) {
      lmyPSWD += header[j];
    }
    mySSID = lmySSID;
    myPSWD = lmyPSWD;
    header = "";
    client0.stop();
  }
  return 1;
}

/*Time*/
String Time() {
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  String tmpNow = "";
  if (newtime->tm_hour < 10) {
    tmpNow += "0";
    tmpNow += String(newtime->tm_hour);
  }
  else {
    tmpNow += String(newtime->tm_hour);
  }
  tmpNow += ":";
  if (newtime->tm_min < 10) {
    tmpNow += "0";
    tmpNow += String(newtime->tm_min);
  }
  else {
    tmpNow += String(newtime->tm_min);
  }
  tmpNow += ":";
  if (newtime->tm_sec < 10) {
    tmpNow += "0";
    tmpNow += String(newtime->tm_sec);
  }
  else {
    tmpNow += String(newtime->tm_sec);
  }
  return tmpNow;
}
/*Date*/
String Date() {
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  String tmpNow = "";
  tmpNow += String(newtime->tm_year + 1900);
  tmpNow += "-";
  if (newtime->tm_mon + 1 < 10) {
    tmpNow += "0";
    tmpNow += String(newtime->tm_mon + 1);
  }
  else {
    tmpNow += String(newtime->tm_mon + 1);
  }
  tmpNow += "-";
  if (newtime->tm_mday < 10) {
    tmpNow += "0";
    tmpNow += String(newtime->tm_mday);
  }
  else {
    tmpNow += String(newtime->tm_mday);
  }
  return tmpNow;
}

/*debug noise*/
float smooth(float data, float filterVal, float smoothedVal) {
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal * filterVal);
  return smoothedVal;
}

/*Control Digital*/
void controlDigital() {
  if (water_temp.toFloat() < waterWarnMax) {
    if (wpumpweb == 1) {
      Serial.println("wpump : on");
      if ((int)soilmeter < soilMin) {
        digitalWrite(15, HIGH);
      }
      else if ((int)soilmeter > soilMax ) {
        digitalWrite(15, LOW);
      }
    }
    else {
      digitalWrite(15, LOW);
      Serial.println("wpump : off");
    }
    if (halfwater == 99) {
      LINE.notify("อุณหภูมิน้ำกลับมาปกติ");
      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
      if (!client1.connect(host, httpPort)) {
        Serial.println("connection failed");
      }
      else {
        String url = "/conn.php?Device_id=";
        //url += "25";
        url += "02";
        /*url += " Pres :";
          url += (String(pres_bme,2)).c_str();
          url += " Alti";
          url += (String(alti_bme,2)).c_str();
        */
        url += "&Code=";
        url += "Normal temperature (Water)";
        url += "&Date_log=";
        url += Date();
        url += "&Time_log=";
        url += Time();
        Serial.print("Requesting URL: ");
        Serial.println(url);
        client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");
      }
    }
    halfwater = 0;
  }
  else if (halfwater != 99 && water_temp.toFloat() > waterWarnMax) {
    Serial.println("Water !!");
    digitalWrite(00, LOW);
    LINE.notify("อุณหภูมิน้ำสูงกว่าค่าที่ตั้งไว้!");
    time_t now = time(nullptr);
    struct tm* newtime = localtime(&now);
    if (!client1.connect(host, httpPort)) {
      Serial.println("connection failed");
    }
    else {
      String url = "/conn.php?Device_id=";
      //url += "25";
      url += "02";
      /*url += " Pres :";
        url += (String(pres_bme,2)).c_str();
        url += " Alti";
        url += (String(alti_bme,2)).c_str();
      */
      url += "&Code=";
      url += "Over temperature (Water)";
      url += "&Date_log=";
      url += Date();
      url += "&Time_log=";
      url += Time();
      Serial.print("Requesting URL: ");
      Serial.println(url);
      client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");
    }
    halfwater = 99;
  }
  if (ferti_temp.toFloat() < fertiWarnMax) {
    if (fpumpweb == 1) {
      digitalWrite(02, HIGH);
      Serial.println("fpump : on");
    }
    else{
      digitalWrite(02, LOW);
      Serial.println("fpump : off");
    }
    if (halffart == 99) {
      LINE.notify("อุณหภูมิปุ๋ยกลับมาปกติ");

      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
      if (!client1.connect(host, httpPort)) {
        Serial.println("connection failed");
      }
      else {
        String url = "/conn.php?Device_id=";
        //url += "25";
        url += "02";
        /*url += " Pres :";
          url += (String(pres_bme,2)).c_str();
          url += " Alti";
          url += (String(alti_bme,2)).c_str();
        */
        url += "&Code=";
        url += "Normal temperature (Fert)";
        url += "&Date_log=";
        url += Date();
        url += "&Time_log=";
        url += Time();
        Serial.print("Requesting URL: ");
        Serial.println(url);
        client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");
      }
    }
    halffart = 0;
  }
  else if (halffart != 99 && ferti_temp.toFloat() > fertiWarnMax) {
    Serial.println("Fart !!");
    //digitalWrite(02, LOW);
    LINE.notify("อุณหภูมิปุ๋ยสูงกว่าค่าที่ตั้งไว้!");

    time_t now = time(nullptr);
    struct tm* newtime = localtime(&now);
    if (!client1.connect(host, httpPort)) {
      Serial.println("connection failed");
    }
    else {
      String url = "/conn.php?Device_id=";
      //url += "25";
      url += "02";
      /*url += " Pres :";
        url += (String(pres_bme,2)).c_str();
        url += " Alti";
        url += (String(alti_bme,2)).c_str();
      */
      url += "&Code=";
      url += "Over temperature (Fert)";
      url += "&Date_log=";
      url += Date();
      url += "&Time_log=";
      url += Time();
      Serial.print("Requesting URL: ");
      Serial.println(url);
      client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");
    }
    halffart = 99;
  }
}
