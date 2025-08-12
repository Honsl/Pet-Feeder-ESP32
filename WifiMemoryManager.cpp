#include "FeederInfo.h"
#include "WifiMemoryManager.h"

//https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html
//https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

bool WifiMemoryManager::saveWifiCreds(const WifiCredentials& creds) {
  prefs.begin("prefs", RW_MODE);
  bool ssidSaved = prefs.putString("ssid", creds.ssid) > 0;
  bool passSaved = prefs.putString("password", creds.password) > 0;
  prefs.end();
  return ssidSaved && passSaved;
}

WifiCredentials WifiMemoryManager::loadWifiCreds() {
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

bool WifiMemoryManager::saveScheduleInfo(const std::vector<Schedule>& list) {
  prefs.begin("schedules", RW_MODE);
  bool savedCount = prefs.putUInt("count", list.size()) > 0;
  bool savedSchedules = true;

  for (size_t i = 0; i < list.size(); i++) {
    String key = "schedule_" + String(i);
    String value = list[i].UUID + "|" + list[i].side + "|" + String(list[i].amount) + "|" + list[i].time;
    savedSchedules &= prefs.putString(key.c_str(), value) > 0;
  }

  prefs.end();
  return savedCount && savedSchedules;
}

std::vector<Schedule> WifiMemoryManager::loadScheduleInfo() {
  std::vector<Schedule> list;
  prefs.begin("schedules", RO_MODE);
  uint32_t count = prefs.getUInt("count", 0);
  for (uint32_t i = 0; i < count; i++) {
    String key = "schedule_" + String(i);
    String value = prefs.getString(key.c_str(), "");
    if (value.length() > 0) {
      Schedule s;
      int idx1 = value.indexOf('|');
      int idx2 = value.indexOf('|', idx1 + 1);
      int idx3 = value.indexOf('|', idx2 + 1);

      s.UUID = value.substring(0, idx1);
      s.side = value.substring(idx1 + 1, idx2);
      s.amount = value.substring(idx2 + 1, idx3).toInt();
      s.time = value.substring(idx3 + 1);

      list.push_back(s);
    }
  }

  prefs.end();
  return list;
}

void WifiMemoryManager::clearScheduleInfo() {
  prefs.begin("schedules", RW_MODE);
  prefs.clear();
  prefs.end();
}


void WifiMemoryManager::clearWifiInfo() {
  prefs.begin("prefs", RW_MODE);
  prefs.clear();
  prefs.end();
}
