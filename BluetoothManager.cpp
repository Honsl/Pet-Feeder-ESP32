#include "BluetoothManager.h"


//Android Device is the client, ESP32 server

#define SERVICE_UUID "dfb4e5ce-76f5-40b5-8cdc-d0096e2ef6be"
#define IP_ADDRESS_UUID "f4de9c04-97ed-4d88-8c10-ef418642b6df"
//Payload: {"ssid":"MyWiFi","password":"Secret123"}
class BLECallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (!value.isEmpty()) {
      Serial.print("Raw input: ");
      Serial.println(value.c_str());

     // StaticJsonDocument<256> doc;
     // DeserializationError err = deserializeJson(doc, value);
      //if (err) {
     //   Serial.println("Failed to parse JSON");
     //   return;
     // }

      // Save the credentials to memory
      WifiMemoryManager wifiMemory;
      WifiCredentials creds;
      //creds.ssid = doc["ssid"];
      //creds.password = doc["password"];
      
     // /if(wifiMemory.save(creds)){
       /// pCharacteristic->setValue("Error saving WIFI information");
      //  pCharacteristic->notify(); 
      //  return;
      //}

      WifiControlManager wifiManager;
      if(wifiManager.connect()){
      pCharacteristic->setValue(wifiManager.getIpAddress().c_str());
      pCharacteristic->notify(); 
      delay(100); //give time for message before shutting down
      //Turn off the Bluetooth - no longer needed.
      //BLEDevice::deinit(true);
      }else{
        pCharacteristic->setValue("Error connecting to WIFI");
        pCharacteristic->notify(); 
      }
     // Serial.printf("Received SSID: %s\n", creds.ssid);
    //  Serial.printf("Received Password: %s\n", creds.password);
    }
  }
  };
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    Serial.println("Client connected");
  }

  void onDisconnect(BLEServer* pServer) override {
    delay(300);
    Serial.println("Restarted advertising after disconnect");
    pServer->getAdvertising()->start();  // Resume advertising so Android can reconnect
  }
};
bool BluetoothManager::start() {

  Serial.println("Starting BLE work!");

  BLEDevice::init("Pet Feeder");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic =
    pService->createCharacteristic(IP_ADDRESS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  
  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic->setCallbacks(new BLECallbacks());
  
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  return true;
}


