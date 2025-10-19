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
    Serial.println(ssid);
    Serial.println(password);
    // Connect to WiFi
    WiFi.disconnect(true);  // true = wipe old config
    WiFi.mode(WIFI_STA);
      delay(100);
    Serial.print("WiFi.begin with: ");
    Serial.println(ssid.c_str());
    Serial.println(password.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    //check the connection
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
      Serial.print(".");
      delay(100);
    }
    Serial.println(WiFi.status());
    //If the connection has failed
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nConnection failed!");
      return false;
    }

    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/feed", HTTP_GET, [this]() {
      this->handleFeedCommand();
    });
    server.on("/setSchedule", HTTP_POST, [this]() {
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
  String body = server.arg("plain");
  Serial.print(body);
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, body);
  JsonArray schedules = doc.as<JsonArray>();
  WifiMemoryManager wifiMemory;
  std::vector<Schedule> parseSchedules;
  for (JsonObject schedule : schedules) {
    int hour, minute;
    char meridiem[3];  // "AM" or "PM"

    sscanf(schedule["time"].as<String>().c_str(), "%d:%d %2s", &hour, &minute, meridiem);
    if (strcmp(meridiem, "PM") == 0 && hour != 12) {
      hour += 12;
    } else if (strcmp(meridiem, "AM") == 0 && hour == 12) {
      hour = 0;
    }
    Schedule feederSchedule = { schedule["id"].as<String>(), schedule["side"].as<String>(), schedule["amount"].as<int>(), hour, minute };
    parseSchedules.push_back(feederSchedule);
  }
  wifiMemory.saveScheduleInfo(parseSchedules);
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