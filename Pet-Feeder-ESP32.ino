#include "WifiControlManager.h"
#include "WifiMemoryManager.h"
#include "WifiCredentials.h"
#include "BluetoothManager.h"
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
    bleManager.start();
  }
  timeClient.begin();
  timeClient.update();

  lastSyncDate = timeClient.getFormattedTime().substring(0, 10);  // YYYY-MM-DD
}

void loop() {
  timeClient.update();  // Will only sync if 24 hours have passed
  if (wifiManager.connected()) {
    wifiManager.handleClient();
  }
  if (millis() - lastSyncCheck >= syncCheckInterval) {
    String currentDate = timeClient.getFormattedTime().substring(0, 10);
    lastSyncCheck = millis();
    if (currentDate != lastSyncDate) {
      Serial.println("Syncing time...");
      timeClient.forceUpdate();  // Manual sync
      lastSyncDate = currentDate;
    }
    Serial.println("Current Time: " + currentDate);
  }

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  
  if (currentHour == 11 && currentMinute == 42 && !alarmActive) {
    Serial.println("🔔 Alarm Triggered: It's 11:00 AM!");
    alarmTriggeredAt = millis();
    alarmActive = true;
  }
  if (alarmActive && millis() - alarmTriggeredAt >= 60000) {
    alarmActive = false;  // Reset after 60 seconds
  }
}
