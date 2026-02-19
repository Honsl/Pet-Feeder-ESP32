#ifndef FEEDER_MANAGER_H
#define FEEDER_MANAGER_H

#include <Arduino.h>
#include "TOF_Sense.h"
#define S_RXD 18
#define S_TXD 19

#include <SCServo.h>

class FeederManager {


public:

  bool checkFoodLevel();
  bool checkSafeToMove();
  bool feed();
  bool setup();
  int side;
  int amount;
  bool feeding = false;
  bool moving = false;

private:
  SCSCL sc;
};

#endif