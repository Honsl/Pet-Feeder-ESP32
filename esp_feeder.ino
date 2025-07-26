#include "WifiControlManager.h"
#include "WifiMemoryManager.h"
#include "WifiCredentials.h"
#include "BluetoothManager.h"

WifiControlManager wifiManager;
BluetoothManager bleManager;
void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.print("Start: ");
  
  WifiMemoryManager wifiMemory;
  WifiCredentials loadedCreds = wifiMemory.load();

  // if it cannot connect or no credentials, turn on the Bluetooth
 // if (loadedCreds.ssid.length() == 0 || !wifiManager.connect()) {
    
    bleManager.start();
    
 // }
}

void loop() {
  // put your main code here, to run repeatedly:
 // wifiManager.handleClient();
}
