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
#include <BH1750.h>
#include <OneWire.h>
#include <MicroGear.h>
#include <TridentTD_LineNotify.h>
#include <time.h>
#include <EEPROM.h>
/*
   EEPROM Addess
   0 = soilMin
   1 = soilMax
   2 = soilWarnMax
   3 = tempWarnMax
   4 = humiWarnMax
   5 = waterWarnMax
   6 = fertiWarnMax
   11 = lightMax
   12 = lightMin
   15 = lightweb
*/

/*Line Nofity*/
#define LINE_TOKEN "ns9QoURN1e20zRi2zsOKRNKyf92KxXGXRm80Hn232eO"

/*Wifi AP Mode*/
#define WIFI_AP_NAME "01ESP32 AP mode"
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

/* Min-Max Control*/
int lightMax = 255, lightMin = 0;
float map_low = 0, map_high = 20000;
BH1750 lightMeter(0x23);

/*NETPIE*/
#define APPID   "SmartFarm29467"
#define KEY     "77vgrLmQXPZWWGN"
#define SECRET  "FFUJ36crE92y5KttB1OKJBXcE"
#define ALIAS   "01ESPino32"
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

/*Input Analog*/
float lux = 0;
boolean status = 0;
/*Hosting*/
#define host "smartfarmkusrc.lnw.mn"
short int httpPort = 80;
short int half = 99, halflight = 0, sta = 0;

int lightweb = 0;

/*Tesk*/
TaskHandle_t Task1;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  /*read EEPROM*/
  EEPROM.begin(16);
  lightMin = EEPROM.read(11);
  lightMax = EEPROM.read(12);
  lightweb = EEPROM.read(15);
  /*Connect Light meter*/
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
  delay(3000);

  /*NETPIE Microgear*/
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  /*Wifi AP Mode*/
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASS);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  while (mySSID == "") {
    myWeb();
  }
  WiFi.disconnect();

  /*Wifi STA Mode*/
  Serial.println("SSID :" + mySSID);
  Serial.println("PASS :" + myPSWD);
  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID.c_str(), myPSWD.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
  connectNetpie = 1;

  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  while (!time(nullptr)) {
    delay(500);
  }

  LINE.setToken(LINE_TOKEN);
  LINE.notify("ทดสอบ");
  /*first input analog*/
  lux = lightMeter.readLightLevel();

  /*GPIO*/
  pinMode(02, OUTPUT); //light
  digitalWrite(02, LOW);
  delay(1000);
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
  lux = lightMeter.readLightLevel();
  Serial.println(lux);
  delay(200);
  controlDigital();
}

/*Tesk*/
void IOT( void * parameter) {
  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      if (connectNetpie == 1) {
        microgear.publish("/Light", (String(lux, 2)).c_str(), true);
        delay(500);
        microgear.publish("/ButtonLight",lightweb,true);
        delay(500);
      }
      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
      if (half == 99 && (newtime->tm_min == 0)) {
        if (!client1.connect(host, httpPort)) {
          Serial.println("connection failed");
        }
        else {
          half = 0;
          String url = "/light_log.php?Device=";
          //url += "25";
          url += "01";
          url += "&Lux=";
          url += lux;
          url += "&Date_log=";
          url += Date();
          url += "&Time_log=";
          url += Time();
          /*url += "&De=";
            url += "01";*/
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
  if (loc.equals("/SmartFarm29467/Set_light")) {
    String temp1, temp2;
    for (int i = 0; i < str.length(); i++) {
      //Serial.println(str.substring(i, i + 3));
      if (str.substring(i, i + 3) == " - ") {
        temp1 = str.substring(0, i);
        temp2 = str.substring(i + 3);
        break;
      }
    }
    Serial.print("My light is ");
    Serial.println((char *)msg);
    Serial.println("My light min is " + temp1);
    Serial.println("My light max is " + temp2);
    lightMin = temp1.toInt();
    lightMax = temp2.toInt();
    EEPROM.write(11, lightMin);
    EEPROM.write(12, lightMax);
  }
  else if(loc.equals("/SmartFarm29467/Set_togg_light")){
    if (str.toInt() == 1) {
      lightweb = 1;
    } else {
      lightweb = 0;
    }
    EEPROM.write(15, lightweb);
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
    microgear.subscribe("/Set_light");
    microgear.subscribe("/Set_togg_light");
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

/*Control Digital*/
void controlDigital() {
  if (lightweb == 1) {
    if (lux < lightMin) {
      sta = 1;
      digitalWrite(02, HIGH);
      if (halflight == 99) {
        Serial.println("ON");
        time_t now = time(nullptr);
        LINE.notify("ค่า Lux ต่ำกว่าที่กำหนดไว้ = เปิดไฟ");
        if (!client1.connect(host, httpPort)) {
          Serial.println("connection failed");
        }
        else {
          struct tm* newtime = localtime(&now);
          String url = "/light.php?Status=";
          //url += "25";
          url += "1";
          url += "&Date_log=";
          url += Date();
          url += "&Time_log=";
          url += Time();
          /*url += "&De=";
            url += "01";*/
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n\r\n");
        }
      }
      halflight = 0;
    }
    else if (halflight != 99 && lux > lightMax) {
      digitalWrite(02, LOW);
      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
      sta = 0;
      LINE.notify("ค่า Lux สูงกกว่าที่กำหนดไว้ = ปิดไฟ");
      Serial.println("OFF");
      if (!client1.connect(host, httpPort)) {
        Serial.println("connection failed");
      }
      else {
        String url = "/light.php?Status=";
        //url += "25";
        url += "0";
        url += "&Date_log=";
        url += Date();
        url += "&Time_log=";
        url += Time();
        /*url += "&De=";
          url += "01";*/
        Serial.print("Requesting URL: ");
        Serial.println(url);
        client1.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");
      }
      halflight = 99;
    }
  } else {
    digitalWrite(02, LOW);
  }
}
