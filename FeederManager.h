#ifndef FEEDER_MANAGER_H
#define FEEDER_MANAGER_H

#include <Arduino.h>


class FeederManager {


public:
  
  bool checkFoodLevel();
  bool feed(String types);
  bool setFeedTimes(String types);
  
};

#endif