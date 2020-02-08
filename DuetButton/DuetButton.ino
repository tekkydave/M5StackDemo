#include <WiFi.h>
#include <HTTPClient.h>
#include <M5Stack.h>
#include <ArduinoJson.h>
#include "DuetUtils.h"

// ArduinoJson Library must be V5

const char* wifiESSID = "n0t0r10us";
const char* wifiPassword = "carloscarbobbymicky!!!";
String duetIP = "192.168.1.104";
String duetPassword = "B1rthday01";

void setup() {
  M5.begin(true, true, false, false);

  M5.Lcd.clear(BLUE);
  M5.Lcd.setTextColor(WHITE, BLUE);
  M5.Lcd.setTextSize(2);
  Serial.begin(115200);

  //connectToWiFi();

    if (connectToWiFi()) {
      duetConnect(duetIP, duetPassword);
    }
}

void loop() {
  M5.update();
  
    duetStatus(duetIP);

    if (M5.BtnA.wasReleasefor(200)) {
      M5.Lcd.print('A');
      duetGCode(duetIP, "M117 Hello from M5Stack");
    }
    
  delay(500);
}


//--------------------------------------------------------------------------------------------
// Connect to WiFi
//--------------------------------------------------------------------------------------------
boolean connectToWiFi() {

  if (WiFi.status() == WL_CONNECTED)    // Already Connected
    return true;
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiESSID, wifiPassword);

  Serial.print("\n\nConnecting to WiFi... ");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\n\n\nConnected to: ");
  Serial.print(WiFi.SSID());
  Serial.print(". IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\n------------------------------------------------\n");
  
  return true;
}
//--------------------------------------------------------------------------------------------




