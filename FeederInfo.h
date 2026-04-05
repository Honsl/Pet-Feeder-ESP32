// FeederInfo.h
#ifndef FEEDER_INFO_H
#define FEEDER_INFO_H

#include <vector>
#include <Arduino.h>

struct Feeder
{
  String name;
  String status;
  String ipAddress;
  // food level min:110mm max:90mm
  uint32_t levelLeft;
  uint32_t levelRight;
};

struct Schedule
{
  String UUID;
  int side;
  int amount;
  int hour;
  int minute;
};

void refreshSchedule();
extern std::vector<Schedule> scheduleList;
#endif