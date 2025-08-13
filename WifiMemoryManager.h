#ifndef WIFI_MEMORY_MANAGER_H
#define WIFI_MEMORY_MANAGER_H

#define RW_MODE false
#define RO_MODE true

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WifiCredentials.h>
#include <FeederInfo.h>


class WifiMemoryManager {
private:
  Preferences prefs;

public:

  bool saveWifiCreds(const WifiCredentials& creds);
  WifiCredentials loadWifiCreds();
  bool hasCredentials();
  bool saveScheduleInfo(const std::vector<Schedule> &list);
  std::vector<Schedule> loadScheduleInfo();
  void clearScheduleInfo();
  void clearWifiInfo();
};

#endif