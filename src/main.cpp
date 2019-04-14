#include <Arduino.h>

#include "config.h"
#include "service.h"

MyowareBLEConnection *conn;
int i = 0;

struct ConnectionHandler : public MyowareBLEConnection::EventHandler
{
  void on_change_trigger_threshold(uint8_t threshold) override
  {
    DEBUG_PRINTLN("Received calibration threshold update");
    DEBUG_PRINTLN(threshold);
  }

  void on_reset_calibrated_min_max()
  {
    DEBUG_PRINTLN("Received reset calibration command");
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
  conn = new MyowareBLEConnection("E2MMyowareDeviceHR", new ConnectionHandler());
  Serial.println("Starting BLE work!");
  conn->init();
}

void loop()
{
  conn->put_telemetry_values(i, i + 1);
  conn->tick();
  i++;
  delay(1000);
}
