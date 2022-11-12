#include<WiFi.h>
#include "time.h"
#include <WebServer.h>
#include <WiFiManager.h>
#include "BluetoothSerial.h" 
BluetoothSerial ESP_BT ; 

const int pinB = 5;
const int pinW = 18;
const int pinG = 16;
const int pinV = 4;
const int pinR = 0;
const int freq = 5000;
const byte chB = 0;
const byte chW = 1;
const byte chG = 2;
const byte chV = 3;
const byte chR = 4;
const byte resolution = 8;
int timezone = 7 * 3600; //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; //กำหนดค่า Date Swing Time
int valR;
int valB;
int valW;
int valG;
int valV;
String val;
String rec ;
WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello ALL esp32  !");
}

void setup() {
  ESP_BT.begin("Esp32");
  Serial.begin(115200);
  ledcSetup(chB, freq, resolution); // ตั้งค่า LED PWM ช่อง 0 ความถี่ 5000mhz ความละเอียด 8bit หรือค่า 0-255
  ledcSetup(chW, freq, resolution);
  ledcSetup(chG, freq, resolution);
  ledcSetup(chV, freq, resolution);
  ledcSetup(chR, freq, resolution);
  ledcAttachPin(pinB, chB);// กำหนดขา led ที่ต้องการควบคุม
  ledcAttachPin(pinW, chW);// กำหนดขา led ที่ต้องการควบคุม
  ledcAttachPin(pinG, chG);// กำหนดขา led ที่ต้องการควบคุม
  ledcAttachPin(pinV, chV);// กำหนดขา led ที่ต้องการควบคุม
  ledcAttachPin(pinR, chR);// กำหนดขา led ที่ต้องการควบคุม
  WiFiManager wifiManager;
  Serial.println("Connecting...");
  if (!wifiManager.startConfigPortal("All")) {
    Serial.println("NO");
    delay(3000);
    ESP.restart();   
    delay(5000);
  }
  server.on("/", handleRoot);
  server.begin();
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //ดึงเวลาจาก Server
  Serial.println("\nLoading time");
  while (!time(nullptr)) {
  Serial.print("*");
  delay(1000);
  }
}

void loop() {
  server.handleClient();
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  int hour = p_tm->tm_hour;
  int mins = p_tm->tm_min;
  float tim  = hour + (mins/60);
  
  if(ESP_BT.available()){
    rec = ESP_BT.readStringUntil('\n');
    Serial.println(rec);
    String sub_S = rec.substring(0,1);
    Serial.println(sub_S);
    int len = rec.length();
    val = rec.substring(1,len);
    delay(10);
   if (rec == "modeAuto"){
    Serial.println("modeAuto" );
      if(1<=tim<5.583){
      ledcWrite(chB, 0);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 0);
      ledcWrite(chR, 0);
    }
    if(5.583<=tim<6){
      ledcWrite(chB, 0);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 0);
      ledcWrite(chR, 0);
    }
    if(6<=tim<10){
      ledcWrite(chB, 114.75);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 114.75);
      ledcWrite(chR, 0);
    }
    if(10<=tim<0.9167){
      ledcWrite(chB, 255);
      ledcWrite(chW, 102);
      ledcWrite(chG, 38.25);
      ledcWrite(chV, 255);
      ledcWrite(chR, 0);
    }
    if(12.55<=tim<18.25){
     ledcWrite(chB, 255);
      ledcWrite(chW, 178.5);
      ledcWrite(chG, 63.75);
      ledcWrite(chV, 100);
      ledcWrite(chR, 0);
    }
    if(18.25<=tim<21.45){
     ledcWrite(chB, 255);
      ledcWrite(chW, 76.5);
      ledcWrite(chG, 25.5);
      ledcWrite(chV, 255);
      ledcWrite(chR, 0);;
    }
    if(21.45<=tim<23.15){
      ledcWrite(chB, 255);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 255);
      ledcWrite(chR, 0);
    }
    if(23.15<=tim<=23.59){
      ledcWrite(chB, 191.25);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 191.25);
      ledcWrite(chR, 0);
    }
    else{
      ledcWrite(chB, 191.25);
      ledcWrite(chW, 0);
      ledcWrite(chG, 0);
      ledcWrite(chV, 191.25);
      ledcWrite(chR, 0);
    }
   }
  else if (rec == "modeLight"){
      Serial.println("modeLight" );    
  }
  else if (sub_S == "modeManuaul"){
      Serial.print("modeLight" ); 
      Serial.println(valR);  
      Serial.println(valV); 
      Serial.println(valB);
      Serial.println(valW);
      Serial.println(valG);
      ledcWrite(chB, valB);
      ledcWrite(chW, valW);
      ledcWrite(chG, valG);
      ledcWrite(chV, valV);
      ledcWrite(chR, valR);
  }
  else if (sub_S == "r"){
    valR = val.toInt()*2.55;

  }
  else if (sub_S == "b"){
    valB = val.toInt()*2.55;

  }
   else if (sub_S == "w"){
    valW = val.toInt()*2.55;

  }
   else if (sub_S == "g"){
    valG = val.toInt()*2.55;
 
  }
   else if (sub_S == "v"){
    valV = val.toInt()*2.55;

  }
 } 
}
