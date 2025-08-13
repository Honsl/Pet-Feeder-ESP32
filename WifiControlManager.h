#ifndef WIFI_CONTROL_MANAGER_H
#define WIFI_CONTROL_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WifiCredentials.h>
#include <FeederInfo.h>


class WifiControlManager {

private:
  WebServer server;
public:
  WifiControlManager();  // Constructor
  bool connect();
  bool connected();
  void reconnect();
  void handleClient();
  void handleFeedCommand();
  void handleStatusCommand();
  void handleScheduleCommand();
  String getIpAddress() const;
};

#endif