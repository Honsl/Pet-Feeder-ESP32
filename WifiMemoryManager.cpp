#include "WifiMemoryManager.h"

//https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html
//https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

bool WifiMemoryManager::save(const WifiCredentials& creds) {
  prefs.begin("prefs", RW_MODE);
  bool ssidSaved = prefs.putString("ssid", creds.ssid) > 0;
  bool passSaved = prefs.putString("password", creds.password) > 0;
  prefs.end();
  return ssidSaved && passSaved;
}

WifiCredentials WifiMemoryManager::load() {
  WifiCredentials creds;
  if (hasCredentials()) {
    prefs.begin("prefs", RO_MODE);

    creds.ssid = prefs.getString("ssid");
    creds.password = prefs.getString("password");
    prefs.end();
  }
  return creds;
}

bool WifiMemoryManager::hasCredentials() {
  prefs.begin("prefs", RO_MODE);
  bool has = prefs.isKey("ssid") && prefs.isKey("password");
  prefs.end();
  return has;
}

void WifiMemoryManager::clear() {
  prefs.begin("prefs", RW_MODE);
  prefs.clear();
  prefs.end();
}
