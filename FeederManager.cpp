#include "FeederManager.h"

bool FeederManager::checkFoodLevel() {

  return false;
}

bool FeederManager::feed(String type) {

  return false;
}

//Set the motor to "Zero" postion
bool FeederManager::setup() {
  SCSCL sc;
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  sc.pSerial = &Serial1;
  delay(1000);
  int Pos = sc.ReadPos(1);
  Serial.print("Feeder POS: ");
  Serial.println(Pos,DEC);
  if (Pos != 0) {
    sc.WritePos(1, 0, 0, 1500);  //Servo(ID1) moves at max speed=1500, moves to position=0.
  }


  return true;
}