#include <Arduino.h>

#include "config.h"
#include "service.h"

MyowareBLEConnection *conn;
int i = 0;

struct ConnectionHandler : public MyowareBLEConnection::EventHandler
{
  void on_receive_calibration_threshold(int threshold) override
  {
    DEBUG_PRINTLN("Received calibration threshold update");
    DEBUG_PRINTLN(threshold);
  }

  void on_connected() override
  {
    DEBUG_PRINTLN("Connected :)");
  }

  void on_disconnected() override
  {
    DEBUG_PRINTLN("Disconnected :(");
  }

  void on_start_advertising() override
  {
    DEBUG_PRINTLN("Advertising :o");
  }
};

void setup()
{
  Serial.begin(115200);
  conn = new MyowareBLEConnection("E2MMyowareDeviceHR");
  conn->set_event_listener(new ConnectionHandler());
  Serial.println("Starting BLE work!");
  conn->init();
}

void loop()
{
  conn->put_telemetry_value(i, i + 1);
  conn->tick();
  i++;
  delay(1000);
}
