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
    WifiCredentials creds = wifiMemory.load();
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

    server.on("/command", HTTP_GET, [this]() {
      this->handleCommand();
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
void WifiControlManager::handleCommand() {
    // Print all arguments
  int argsCount = server.args();
  for (int i = 0; i < argsCount; i++) {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(" - Name: ");
    Serial.print(server.argName(i));
    Serial.print(", Value: ");
    Serial.println(server.arg(i));
  }
  if (server.hasArg("cmd")) {
    String command = server.arg("cmd");
    if (command == "feed") {
      if (server.hasArg("value")) {
        String value = server.arg("value");
        Serial.print("Received command:FEED ");
        Serial.println(value);

        String response = "{\"foodLevel1\":\"OK\",\"foodLevel2\":\"test\"}";

        server.send(200, "application/json", response);
        return;
      }
    } else if (command == "level") {
      String response = "{\"status\":\"Error\"\"}";
      server.send(200, "application/json", response);
      return;
    }
  }
   Serial.print("Error command ");
  server.send(400);
}