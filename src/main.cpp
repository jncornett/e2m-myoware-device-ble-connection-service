/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SERVICE_UUID "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID "7772E5DB-3868-4112-A1A9-F2669D106BF3"

bool deviceConnected = false;
bool prevDeviceConnected = false;

BLECharacteristic *pCharacteristic = NULL;

struct DeviceConnectionCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *)
  {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer *)
  {
    deviceConnected = false;
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("E2MMyowareDeviceMidi");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new DeviceConnectionCallbacks());

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

  pCharacteristic = pService->createCharacteristic(
      BLEUUID(CHARACTERISTIC_UUID),
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_WRITE_NR);
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void handle_update_connection_state()
{
  if (deviceConnected && !prevDeviceConnected)
  {
    Serial.println("Device connected :)");
  }
  else if (!deviceConnected && prevDeviceConnected)
  {
    Serial.println("Device disconnected :(");
  }
  prevDeviceConnected = deviceConnected;
}

void loop()
{
  handle_update_connection_state();
  if (deviceConnected)
  {
    uint8_t midiPacket[] = {
        0x80, // header
        0x80, // timestamp, not implemented
        0x00, // status
        0x3c, // 0x3c == 60 == middle c
        0x00  // velocity
    };
    midiPacket[2] = 0x90;
    midiPacket[4] = 127;
    pCharacteristic->setValue(midiPacket, 5);
    pCharacteristic->notify();
    delay(500);
    return;
  }
  delay(2000);
}