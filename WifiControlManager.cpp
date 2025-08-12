#include "WifiControlManager.h"
#include "WifiMemoryManager.h"

WifiControlManager::WifiControlManager()
  : server(80) {
}

String WifiControlManager::getIpAddress() const {
  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.localIP().toString();
  }
  return "Not connected";
}

bool WifiControlManager::connected() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  return false;
}

void WifiControlManager::reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.reconnect();
  }
}

bool WifiControlManager::connect() {
  WifiMemoryManager wifiMemory;
  //Make sure that there is ssid and password saved before trying to connect
  if (wifiMemory.hasCredentials()) {
    //Get the ssid and password credentials
    WifiCredentials creds = wifiMemory.loadWifiCreds();
    String ssid = creds.ssid;
    String password = creds.password;
    // Connect to WiFi
    WiFi.begin(ssid.c_str(), password.c_str());
    //check the connection
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
      Serial.print(".");
      delay(100);
    }
    //If the connection has failed
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nConnection failed!");
      return false;
    }
    Serial.println(ssid);
    Serial.println(password);
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/feed", HTTP_GET, [this]() {
      this->handleFeedCommand();
    });
    server.on("/setSchedule", HTTP_GET, [this]() {
      this->handleScheduleCommand();
    });
    server.on("/status", HTTP_GET, [this]() {
      this->handleStatusCommand();
    });
    server.begin();  // Start the server
    Serial.println("\nServer Started");
  } else {
    return false;
  }

  return true;
}
void WifiControlManager::handleClient() {
  server.handleClient();
}
//http://<ESP32_IP>/command?cmd=SET_MODE&value=3
void WifiControlManager::handleFeedCommand() {

  if (server.hasArg("portion")) {
    String portions = server.arg("portion");

    Serial.print("Received command:Portion ");
   

    String response = "{\"foodLevel1\":\"OK\",\"foodLevel2\":\"test\"}";

    server.send(200, "application/json", response);
    return;
  }
  Serial.print("Error command ");
  server.send(400);
}
void WifiControlManager::handleScheduleCommand() {

    Serial.print("Received command:Schedule ");


    String response = "{\"leftFeeder\":\"50\",\"rightFeeder\":\"70\"}";

    server.send(200, "application/json", response);
    return;

}
void WifiControlManager::handleStatusCommand() {

    Serial.print("Received command:Status ");


    String response = "{\"leftFeeder\":\"50\",\"rightFeeder\":\"70\"}";

    server.send(200, "application/json", response);
    return;

}