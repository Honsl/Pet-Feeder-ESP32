// FeederInfo.h
#ifndef FEEDER_INFO_H
#define FEEDER_INFO_H

#include <vector>  
#include <Arduino.h>

struct Feeder {
  String name;
  String status;
  String ipAddress;
  float levelLeft;
  float levelRight;
};

struct Schedule {
  String UUID;
  String side;
  int amount;
  int hour;
  int minute;
};

void refreshSchedule();
extern std::vector<Schedule> scheduleList;
#endif