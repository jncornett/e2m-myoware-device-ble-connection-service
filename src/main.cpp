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
// #define SERVICE_UUID "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define SVC_RSC "1814"      // org.bluetooth.service.running_speed_and_cadence
#define SVC_HR "180d"       // org.bluetooth.service.heart_rate
#define SERVICE_UUID SVC_HR // org.bluetooth.service.running_speed_and_cadence
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHR_HR_HEART_RATE_MEASUREMENT "2a37" // org.bluetooth.characteristic.heart_rate_measurement
#define CHR_HR_BODY_SENSOR_LOCATION "2a37"   // org.bluetooth.characteristic.heart_rate_measurement
#define CHR_HR_HEART_RATE_CONTROL_POINT "2a39"
#define CHR_RSC_MEASUERMENT "2a53"      // org.bluetooth.characteristic.rsc_measurement
#define CHR_RSC_FEATURE "2a54"          // org.bluetooth.characteristic.rsc_feature
#define CHR_RSC_SENSOR_LOCATION "2a5d"  // org.bluetooth.characteristic.sensor_location
#define CHR_RSC_SC_CONTROL_POINT "2a55" // org.bluetooth.characteristic.sc_control_point
//#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

bool deviceConnected = false;
bool prevDeviceConnected = false;

BLECharacteristic *chrHrBodySensorLocation = NULL;
BLECharacteristic *chrHrHeartRateMeasurement = NULL;
BLECharacteristic *chrHrHeartRateControlPoint = NULL;
BLECharacteristic *chrRscMeasurement = NULL;
BLECharacteristic *chrRscFeature = NULL;
BLECharacteristic *chrRscSensorLocation = NULL;
BLECharacteristic *chrRscScControlPoint = NULL;

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

  BLEDevice::init("E2MMyowareDevicePace");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new DeviceConnectionCallbacks());

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

  chrHrHeartRateMeasurement = pService->createCharacteristic(
      BLEUUID(CHR_HR_HEART_RATE_MEASUREMENT),
      BLECharacteristic::PROPERTY_NOTIFY);
  chrHrHeartRateMeasurement->addDescriptor(new BLE2902());

  chrHrBodySensorLocation = pService->createCharacteristic(
      BLEUUID(CHR_HR_BODY_SENSOR_LOCATION),
      BLECharacteristic::PROPERTY_READ);
  chrHrBodySensorLocation->addDescriptor(new BLE2902());

  chrHrHeartRateControlPoint = pService->createCharacteristic(
      BLEUUID(CHR_HR_HEART_RATE_CONTROL_POINT),
      BLECharacteristic::PROPERTY_WRITE);
  chrHrHeartRateControlPoint->addDescriptor(new BLE2902());

  pService->start();

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID(SERVICE_UUID));
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
    // pCharacteristic->setValue(midiPacket, 5);
    // pCharacteristic->notify();
    delay(500);
    return;
  }
  delay(2000);
}
