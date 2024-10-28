#include <Arduino.h>
#include <EEPROM.h>
#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/signalk/signalk_listener.h"
#include "sensesp/signalk/signalk_value_listener.h"
#include "sensesp/sensors/digital_output.h"
#include "sensesp/transforms/threshold.h"
#include "sensesp/transforms/lambda_transform.h"
#include "relay8.h"
#include "sensesp/system/pwm_output.h"
#include "sensesp/sensors/sensor.h"

//#undef  LED_BUILTIN
//#define LED_BUILTIN GPIO_NUM_23

#define ESP_BTNR  GPIO_NUM_19    // could connect a swith here to reset to SENSESP defaults
#define hostname "relay0"

// Inspired by https://github.com/JohnySeven/Boat-T-RelayS3
// This is for the 8 relay board: http://www.chinalctech.com/cpzx/Programmer/Relay_Module/540.html
// Good description here: https://werner.rothschopf.net/microcontroller/202208_esp32_relay_x8_en.htm
// LOOK HERE FOR FLASHING GUIDE
// https://community.home-assistant.io/t/shall-i-throw-away-a-usb-less-esp32-with-an-integrated-8-relay-module/690121/17

reactesp::ReactESP app;
Relay8 relay;
bool volatile button_down = false;

void initialize_relay_channel(String sk_path, String config, int index) {
  String name = "Relay#" + String(index);
  RelayChannel *channel = relay.get_relay(index);
  channel->connect_to(new SKOutputBool(sk_path, config, new SKMetadata("bool", "Relay# " + String(index))));
  channel->connect_to(new UIOutput<bool>(name));
  auto button = new UIButton("Toggle relay " + String(index), name, false);
  button->attach([channel]()
                 { channel->set_input(true); });

  auto listener = new BoolSKPutRequestListener(sk_path);
  listener->connect_to(channel);
}

// SensESP builds upon the ReactESP framework. Every ReactESP application
// defines an "app" object vs defining a "main()" method.
void setup() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  Serial.begin(115200);
  // A small arbitrary delay is required to let the
  // serial port catch up
  delay(100);
  Debug.setSerialEnabled(false);
#endif


    // connect to WiFi & SignalK server
    SensESPAppBuilder builder;
    sensesp_app = (&builder)
                      // Set a custom hostname for the app.
                      ->set_hostname(hostname)
                      ->enable_ota("mypassword")
//                      ->set_wifi (_SSID,_PASSWD)
                      ->enable_wifi_signal_sensor()
                      ->enable_free_mem_sensor()
                      ->enable_ip_address_sensor()
                      ->set_button_pin (ESP_BTNR)    // reset and clear button
                      ->get_app();
                

  debugI("Initializing LilyGO T-Relay...");

  if (!relay.begin())
  {
    debugI("Failed to initialize Relay8!");
  }


  initialize_relay_channel("electrical.switches.relay0", "/sk/relay0", 0);
  initialize_relay_channel("electrical.switches.relay1", "/sk/relay1", 1);
  initialize_relay_channel("electrical.switches.relay2", "/sk/relay2", 2);
  initialize_relay_channel("electrical.switches.relay3", "/sk/relay3", 3);
  initialize_relay_channel("electrical.switches.relay4", "/sk/relay4", 4);
  initialize_relay_channel("electrical.switches.relay5", "/sk/relay6", 5);
  initialize_relay_channel("electrical.switches.relay6", "/sk/relay6", 6);
  initialize_relay_channel("electrical.switches.relay7", "/sk/relay7", 7);

  sensesp_app->start();

  app.onRepeat(1000, []() {
    auto status = sensesp_app->get_system_status_controller()->get();

  });
}

void loop() {
  app.tick();
}