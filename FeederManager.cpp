#include "FeederManager.h"

bool FeederManager::checkFoodLevel() {

  return false;
}

bool FeederManager::feed(String type) {
  SCSCL sc;
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  sc.pSerial = &Serial1;
  delay(1000);
  sc.WritePos(1, 0, 0, 1500);//Servo(ID1) moves at max speed=1500, moves to position=0.
  delay(500);
  return false;
}


bool FeederManager::setFeedTimes(String type) {

  return false;
}