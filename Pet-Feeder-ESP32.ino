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

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -14400;         // EDT = UTC -4 hours
const long updateInterval = 86400000;           // 24 hours in milliseconds
const unsigned long syncCheckInterval = 30000;  // Check sync every 60 seconds
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, updateInterval);

String lastSyncDate = "";

unsigned long alarmTriggeredAt = 0;
unsigned long lastSyncCheck = 0;
bool alarmActive = false;



void setup() {
  Serial.begin(115200);
  //Delay to allow setup of serial
  delay(3000);
  Serial.print("Start: ");

  WifiMemoryManager wifiMemory;
  
  WifiCredentials loadedCreds = wifiMemory.loadWifiCreds();

  // if cannot connect to wifi or no credentials, turn on the Bluetooth
  if (loadedCreds.ssid.length() == 0 || !wifiManager.connect()) {
    //  Serial.println("START BLE");
    bleManager.start();
  } else {
    timeClient.begin();
    timeClient.update();

    lastSyncDate = timeClient.getFormattedTime().substring(0, 10);  // YYYY-MM-DD
  }
}

void loop() {

  if (wifiManager.connected()) {
    wifiManager.handleClient();
    timeClient.update();  // Will only sync if 24 hours have passed
  }
  //if there is a schedule set
  if (!scheduleList.empty()) {
    if (millis() - lastSyncCheck >= syncCheckInterval) {
      String currentTime = timeClient.getFormattedTime();
      lastSyncCheck = millis();
      int currentHour = timeClient.getHours();
      int currentMinute = timeClient.getMinutes();

      for (Schedule list : scheduleList) {
        if (currentHour == list.hour && currentMinute == list.minute && !alarmActive) {
          Serial.println("Alarm Triggered: It's 11:00 AM!");
          alarmTriggeredAt = millis();
          alarmActive = true;
        }
      }
      Serial.println("Current Time: " + currentTime);
    }
  } else {
    //set the motor to "Zero" position
    FeederManager feeder;
    //feeder.setup();
  }

  if (alarmActive && millis() - alarmTriggeredAt >= 60000) {
    alarmActive = false;  // Reset after 60 seconds
  }
}
