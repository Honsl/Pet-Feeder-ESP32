#include "FeederManager.h"
#include "TOF_Sense.h"



bool FeederManager::checkFoodLevel() {

  return false;
}

bool FeederManager::checkSafeToMove() {
  TOF_Active_Decoding();
  Serial.println("DISTANCE:");
  Serial.println(TOF_0.dis);
  if (TOF_0.dis_status == 1 && TOF_0.dis >= 49) {
    return true;
  }
  return false;
}

bool FeederManager::feed() {
  // if not safe to movew stop motor
  if (!checkSafeToMove()) {
    //set the motor to current postion to stop movement
    sc.WritePos(1, sc.ReadPos(1), 0, 500);
    moving=false;
    return false;
  }
  //if the side is 0(random) select a side
  if (side == 0) {
    side = (random(2) == 0) ? 1 : 2;  // random 1 or 2
  }
  u16 position = (side == 1) ? 0 : 1024;

  if (!moving && abs(sc.ReadPos(1) - position) > 26) {
    sc.WritePos(1, position, 0, 100);
    moving = true;
  }
 
  //if the scoop has been set to a hopper, then move back to dispense
  if (moving && abs(sc.ReadPos(1) - position) < 26) {
    //move scoop to dispence
    sc.WritePos(1, 512, 0, 100);
    moving = false;
    feeding = false;
  }

  return false;
}

//Set the motor to "Zero" postion
bool FeederManager::setup() {
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  TOF_UART.begin(921600, SERIAL_8N1, TOF_RX_PIN, TOF_TX_PIN);
  sc.pSerial = &Serial1;
  delay(1000);
  int Pos = sc.ReadPos(1);
  Serial.print("Feeder POS: ");
  Serial.println(Pos, DEC);

  sc.WritePos(1, 512, 0, 500);  //Servo(ID1) moves at max speed=1500, moves to position=511 which is middle.

  int ID = sc.Ping(1);
  if (ID != -1) {
    Serial.print("Servo ID:");
    Serial.println(ID, DEC);
    delay(100);
  } else {
    Serial.println("Ping servo ID error!");
    delay(2000);
  }

  return true;
}