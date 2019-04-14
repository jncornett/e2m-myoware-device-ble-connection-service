#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <BLECharacteristic.h>

struct MyowareBLEConnection
{
  struct EventHandler
  {
    virtual void on_receive_calibration_threshold(int threshold) {}
    virtual void on_connected() {}
    virtual void on_disconnected() {}
    virtual void on_start_advertising() {}
  };

  MyowareBLEConnection(const char *device_name) : device_name(device_name),
                                                  event_handler(new EventHandler()) {}

  void init();
  void tick();
  void set_event_listener(EventHandler *);
  void put_telemetry_value(short left, short right);

  const char *device_name = nullptr;
  EventHandler *event_handler = nullptr;
  bool is_connected = false;
  bool prev_is_connected = false;
  int prev_calibration_threshold = 0;
  BLECharacteristic *chrHeartRateMeasurement = nullptr;
  BLECharacteristic *chrBodySensorLocation = nullptr;
  BLECharacteristic *chrHeartRateControlPoint = nullptr;
};

#endif
