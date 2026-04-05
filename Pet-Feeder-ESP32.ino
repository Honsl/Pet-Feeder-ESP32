#include "WifiControlManager.h"
#include "WifiMemoryManager.h"
#include "WifiCredentials.h"
#include "FeederManager.h"
#include "BluetoothManager.h"
#include "FeederInfo.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WifiControlManager wifiManager;
BluetoothManager bleManager;
FeederManager feederManager;

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -14400;         // EDT = UTC -4 hours
const long updateInterval = 86400000;           // 24 hours in milliseconds
const unsigned long syncCheckInterval = 30000;  // Check sync every 60 seconds
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, updateInterval);

String lastSyncDate = "";

unsigned long alarmTriggeredAt = 0;
unsigned long lastSyncCheck = 0;
bool alarmActive = false;
bool ntpStarted = false;
bool BLEoff = false;


void setup() {
  Serial.begin(115200);
  //Delay to allow setup of serial
  delay(3000);
  Serial.print("Start: ");

  WifiMemoryManager wifiMemory;

  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    //if the EN button was pushed reset the WIFI info
    case ESP_RST_EXT:
      Serial.println("External Reset");
      wifiMemory.clearWifiInfo();
    case ESP_RST_POWERON:
      Serial.println("Power-on reset");
      break;
  }

  
  WifiCredentials loadedCreds = wifiMemory.loadWifiCreds();
  // if cannot connect to wifi or no credentials, turn on the Bluetooth
  if (loadedCreds.ssid.length() == 0 || !wifiManager.connect()) {
    bleManager.start();
  } else {
    wifiManager.connect();
    timeClient.begin();
    timeClient.update();
    ntpStarted = true;
    lastSyncDate = timeClient.getFormattedTime().substring(0, 10);  // YYYY-MM-DD
  }


  feederManager.setup();
}

void loop() {

  if (wifiManager.connected()) {

    if (!BLEoff) {
      BLEoff = bleManager.isDone();
    }
    wifiManager.handleClient();
    if (!ntpStarted) {
      timeClient.begin();
      ntpStarted = true;
    }
    timeClient.update();  // Will only sync if 24 hours have passed
  }
  //if there is a schedule set
  if (millis() - lastSyncCheck >= syncCheckInterval) {
    String currentTime = timeClient.getFormattedTime();
    lastSyncCheck = millis();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();

    for (Schedule list : scheduleList) {
      if (currentHour == list.hour && currentMinute == list.minute && !alarmActive) {
        alarmTriggeredAt = millis();
        alarmActive = true;
        feederManager.feeding = true;
        feederManager.side = list.side;
        feederManager.amount = list.amount;
      }
    }
    Serial.println("Current Time: " + currentTime);
  }

  if (feederManager.feeding) {

    feederManager.feed();
  }


  if (alarmActive && millis() - alarmTriggeredAt >= 60000) {
    alarmActive = false;  // Reset after 60 seconds
  }
}
