#ifndef WIFI_MEMORY_MANAGER_H
#define WIFI_MEMORY_MANAGER_H

#define RW_MODE false
#define RO_MODE true

#include <Arduino.h>
#include <Preferences.h>
#include <WifiCredentials.h>

class WifiMemoryManager {
private:
  Preferences prefs;

public:

  bool save(const WifiCredentials& creds);
  WifiCredentials load();
  bool hasCredentials();
  void clear();
};

#endif