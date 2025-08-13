#include <FeederInfo.h>
#include "WifiMemoryManager.h"

std::vector<Schedule> scheduleList;

void refreshSchedule() {
  WifiMemoryManager wifiMemory;
  scheduleList = wifiMemory.loadScheduleInfo();
  Serial.println("Schedule refreshed from memory.");
}