// BluetoothManager.h
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include "WifiMemoryManager.h"
#include "WifiControlManager.h"
#include "WifiCredentials.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>   
#include <Arduino.h>


class BluetoothManager {


public:
  bool start();

};

#endif