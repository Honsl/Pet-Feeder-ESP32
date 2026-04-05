#include "pgmspace.h"
#include "FeederInfo.h"
#include "WifiMemoryManager.h"

// https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html
// https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

bool WifiMemoryManager::saveWifiCreds(const WifiCredentials &creds)
{
  prefs.begin("prefs", RW_MODE);
  bool ssidSaved = prefs.putString("ssid", creds.ssid) > 0;
  bool passSaved = prefs.putString("password", creds.password) > 0;
  prefs.end();
  return ssidSaved && passSaved;
}

WifiCredentials WifiMemoryManager::loadWifiCreds()
{
  WifiCredentials creds;
  if (hasCredentials())
  {
    prefs.begin("prefs", RO_MODE);

    creds.ssid = prefs.getString("ssid");
    creds.password = prefs.getString("password");
    prefs.end();
  }
  return creds;
}

bool WifiMemoryManager::hasCredentials()
{
  prefs.begin("prefs", RO_MODE);
  bool has = prefs.isKey("ssid") && prefs.isKey("password");
  prefs.end();
  return has;
}

bool WifiMemoryManager::saveFeederInfo(const String name, const uint32_t left, const uint32_t right)
{
  prefs.begin("feeder", RW_MODE);
  bool name_saved = prefs.putString("name", name) > 0;
  bool left_saved = prefs.putUInt("feeder_left", left) > 0;
  bool right_saved = prefs.putUInt("feeder_right", right) > 0;
  prefs.end();

  return name_saved && left_saved && right_saved;
}

bool WifiMemoryManager::saveFeederLevel(const uint32_t left, const uint32_t right)
{
  prefs.begin("feeder", RW_MODE);
  bool left_saved = prefs.putUInt("feeder_left", left) > 0;
  bool right_saved = prefs.putUInt("feeder_right", right) > 0;
  prefs.end();

  return left_saved && right_saved;
}

Feeder WifiMemoryManager::loadFeederInfo()
{
  prefs.begin("feeder", RO_MODE);
  bool has = prefs.isKey("name") && prefs.isKey("feeder_left") && prefs.isKey("feeder_right");
  Feeder feeder;
  if (has)
  {

    feeder.name = prefs.getString("name");
    feeder.levelLeft = prefs.getUInt("feeder_left");
    feeder.levelRight = prefs.getUInt("feeder_right");
    prefs.end();
  }
  return feeder;
}

void WifiMemoryManager::clearFeederInfo()
{
  prefs.begin("feeder", RW_MODE);
  prefs.clear();
  prefs.end();
}

bool WifiMemoryManager::saveScheduleInfo(const std::vector<Schedule> &list)
{
  // Always remove any saved schedules before saving the new ones
  clearScheduleInfo();
  prefs.begin("schedules", RW_MODE);

  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();

  for (Schedule schedule : list)
  {
    JsonObject obj = array.createNestedObject();
    obj["id"] = schedule.UUID;
    obj["side"] = schedule.side;
    obj["amount"] = schedule.amount;
    obj["hour"] = schedule.hour;
    obj["minute"] = schedule.minute;
  }
  String jsonString;
  serializeJson(doc, jsonString);
  bool savedSchedules = prefs.putString("scheduleList", jsonString);
  prefs.end();
  refreshSchedule();
  return savedSchedules > 0;
}

std::vector<Schedule> WifiMemoryManager::loadScheduleInfo()
{
  Preferences prefs;
  std::vector<Schedule> scheduleList;

  prefs.begin("schedules", RO_MODE); // read-only mode
  if (!prefs.isKey("scheduleList"))
  {
    return scheduleList;
  }
  String jsonString = prefs.getString("scheduleList", "[]");
  prefs.end();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error)
  {
    Serial.print("Failed to parse schedule JSON: ");
    Serial.println(error.c_str());
    return scheduleList; // return empty list
  }

  JsonArray array = doc.as<JsonArray>();
  for (JsonObject obj : array)
  {
    Schedule s;
    s.UUID = obj["id"].as<String>();
    s.side = obj["side"].as<int>();
    s.amount = obj["amount"].as<int>();
    s.hour = obj["hour"].as<int>();
    s.minute = obj["minute"].as<int>();
    scheduleList.push_back(s);
  }

  return scheduleList;
}

void WifiMemoryManager::clearScheduleInfo()
{
  prefs.begin("schedules", RW_MODE);
  prefs.clear();
  prefs.end();
}

void WifiMemoryManager::clearWifiInfo()
{
  prefs.begin("prefs", RW_MODE);
  prefs.clear();
  prefs.end();
}
