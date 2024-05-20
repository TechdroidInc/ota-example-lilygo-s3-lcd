#include <Arduino.h>
#include <WiFi.h>
#include <esp32FOTA.hpp>


esp32FOTA FOTA(BOARD, VERSION, false, true);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* manifestUrl = OTA_URL;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println(WiFi.localIP());
}

void checkForUpdate() {
    bool updatedNeeded = FOTA.execHTTPcheck();
    Serial.print("-- Update Needed: ");    
    Serial.println(updatedNeeded);      
    if (updatedNeeded)
    {
      FOTA.execOTA();
    }
}


void setup()
{

  Serial.begin(MONITOR_SPEED);

  delay(3000);
  Serial.println("========= Start ==========");

  // Configure OTA
  FOTA.setManifestURL(manifestUrl);
  FOTA.useDeviceId(true);
  FOTA.printConfig();
  FOTA.setExtraHTTPHeader("x-hardware", BOARD);
  FOTA.setExtraHTTPHeader("x-current-version", VERSION);
  FOTA.setExtraHTTPHeader("x-hardware-uuid", OTA_HW_UUID);
  FOTA.setUpdateFinishedCb([](int partition, bool restart_after) {
    // do stuff after update
    if (restart_after)
    {
      ESP.restart();
    }
  });

  // Conntect to wifi
  initWiFi();

  // check for update
  checkForUpdate();

}


void loop()
{   
    // check for update from time to time
    checkForUpdate();

    delay(30000);
}
