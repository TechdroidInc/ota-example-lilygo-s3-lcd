#include <Arduino.h>
#include <WiFi.h>
#include <esp32FOTA.hpp>

#include "TFT_eSPI.h"

esp32FOTA FOTA(BOARD, VERSION, false, true);

TFT_eSPI tft = TFT_eSPI();

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* manifestUrl = OTA_URL;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");

  tft.drawString("Connecting to WiFi ..", 0, 0, 2);  

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println(WiFi.localIP());
}

void updateProgressCallback(size_t progress, size_t size)
{
  int percent = ((float)progress / (float)size) * 320;

  tft.fillRect(10,170-30-20, percent , 30, TFT_WHITE);  
}

void checkForUpdate() {
    tft.drawString("Update Check           ", 0, 0, 2);
    delay(1000);
    bool updatedNeeded = FOTA.execHTTPcheck();

    Serial.print("-- Update Needed: ");    
    Serial.println(updatedNeeded);      

    if (updatedNeeded)
    {
      tft.drawString("Update required      ", 0, 0, 2);
      delay(1000);
      tft.drawString("Updating             ", 0, 0, 2);
      FOTA.execOTA();
    } else {
      tft.drawString("No Update needed     ", 0, 0, 2);  
    }
    delay(1500);
}


void setup()
{

  Serial.begin(MONITOR_SPEED);

  delay(3000);
  Serial.println("========= Start ==========");

  tft.begin();
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Starting...", 0, 0, 2);  

  tft.drawString("Version: ", 0, 40, 2);  
  tft.drawString(VERSION, 110, 40, 2);  

  // Configure OTA
  FOTA.setManifestURL(manifestUrl);
  FOTA.useDeviceId(true);
  FOTA.printConfig();
  FOTA.setProgressCb(updateProgressCallback);
  FOTA.setExtraHTTPHeader("x-hardware", BOARD);
  FOTA.setExtraHTTPHeader("x-current-version", VERSION);
  FOTA.setExtraHTTPHeader("x-hardware-uuid", OTA_HW_UUID);
  FOTA.setUpdateFinishedCb([](int partition, bool restart_after) {
    // do stuff after update
    if (restart_after)
    {
      tft.drawString("Rebooting             ", 0, 0, 2);
      delay(1000);
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

    delay(10000);
}
