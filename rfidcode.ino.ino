#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

/* WIFI */
const char* ssid = "OPPO A6x 5G 414f";
const char* password = "btyd6956";

String serverName = "http://10.152.76.176:5000/add";
 
/* OLED */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* RFID */
#define SS_PIN 13
#define RST_PIN 14
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

MFRC522 rfid(SS_PIN, RST_PIN);

/* STUDENTS */
struct Student{
  String uid;
  String name;
};

Student students[4] = {
  {"13 98 09 DA", "Dharshika"},
  {"7A 7D 8B 3F", "Leela"},
  {"0A 35 5A 19", "Mukitha"},
  {"1C 59 12 22", "Priya Dharshini"}
};

void setup(){

  Serial.begin(115200);

  /* OLED */
  Wire.begin(21,22);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Attendance System");
  display.println("Scan Card");
  display.display();

  /* RFID */
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();

  /* WIFI */
  WiFi.begin(ssid,password);

  Serial.print("Connecting WiFi");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  /* TIME */
  configTime(19800,0,"pool.ntp.org");
}

void loop(){

  if(!rfid.PICC_IsNewCardPresent()) return;
  if(!rfid.PICC_ReadCardSerial()) return;

  String uid="";

  for(byte i=0;i<rfid.uid.size;i++){
    if(rfid.uid.uidByte[i] < 0x10) uid+="0";
    uid+=String(rfid.uid.uidByte[i],HEX);
    uid+=" ";
  }

  uid.toUpperCase();
  uid.trim(); // important

  Serial.println("UID : "+uid);

  bool found = false;

  for(int i=0;i<4;i++){

    String storedUID = students[i].uid;
    storedUID.trim();

    if(uid.equalsIgnoreCase(storedUID)){

      found = true;

      String name = students[i].name;
      Serial.println("Name : "+name);

      /* GET TIME */
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        Serial.println("Time Failed");
      }

      char dateStr[20];
      char timeStr[20];

      strftime(dateStr,20,"%Y-%m-%d",&timeinfo);
      strftime(timeStr,20,"%H:%M:%S",&timeinfo);

      if(WiFi.status()==WL_CONNECTED){

        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type","application/json");

        String json = "{\"name\":\""+name+"\",\"uid\":\""+uid+"\",\"date\":\""+dateStr+"\",\"time\":\""+timeStr+"\"}";

        int response = http.POST(json);

        Serial.print("HTTP Code: ");
        Serial.println(response);

        String payload = http.getString();
        Serial.println("Response: " + payload);

        // 🔥 Already Marked Check
        if(payload.indexOf("already_marked") >= 0){

          display.clearDisplay();
          display.setCursor(0,0);
          display.println(name);
          display.println("Already Marked");
          display.display();

          delay(1000);

          display.clearDisplay();
          display.setCursor(0,0);
          display.println("Scan Card");
          display.display();

          http.end();
          return; // VERY IMPORTANT
        }

        // ✅ Normal Success
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(name);
        display.println("Attendance Marked");
        display.display();

        http.end();
      }

      delay(1000);

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Scan Card");
      display.display();
    }
  }

  // ❌ Unknown Card
  if(!found){
    Serial.println("Invalid Card");

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Invalid Card");
    display.display();

    delay(1000);

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Scan Card");
    display.display();
  }

  rfid.PICC_HaltA();
}