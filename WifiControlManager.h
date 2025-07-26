#ifndef WIFI_CONTROL_MANAGER_H
#define WIFI_CONTROL_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WifiCredentials.h>

class WifiControlManager {

private:
  WebServer server;
public:
  WifiControlManager();  // Constructor
  bool connect();
  void reconnect();
  void handleClient();
  void handleCommand();
  String getIpAddress() const;
};

#endif