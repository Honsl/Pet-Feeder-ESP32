#ifndef FEEDER_MANAGER_H
#define FEEDER_MANAGER_H

#include <Arduino.h>
#define S_RXD 18
#define S_TXD 19

#include <SCServo.h>

class FeederManager {


public:
  
  bool checkFoodLevel();
  bool feed(String types);
  bool setFeedTimes(String types);
  
};

#endif