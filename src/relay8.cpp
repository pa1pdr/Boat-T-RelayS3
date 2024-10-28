#include "relay8.h"
#include <Wire.h>
#include "sensesp_app.h"

/*
static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "relay0": { "title": "Relay0", "type": "boolean", "description": "Set the relay on when restarting" }
        "relay1": { "title": "Relay1", "type": "boolean", "description": "Set the relay on when restarting" }
    }
  })###";



 
void RelayChannel::get_configuration(JsonObject &root) {
   char tag[7];
   sprintf (tag,"relay%d",relay_index_);
   root[tag] = (digitalRead (gpioMap[relay_index_]) == HIGH);
   debugI ("Read %s from relay %d",(gpioMap[relay_index_]==HIGH)?"HIGH":"LOW",relay_index_);
   debugI ("Root[%s] => %s",tag,root);
}


String RelayChannel::get_config_schema() { return FPSTR(SCHEMA); }

bool RelayChannel::set_configuration(const JsonObject &config) {
  char tag[7];
  sprintf (tag,"relay%d",relay_index_);
  String expected[] = {tag};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
        debugW ("Key %s not found in %s",tag,config);
        return false;
    }
  }
  bool res = config[tag];
  debugD ("Fetched %s from configuration",res?"true":"false");
  set_input (res);
  return true;
}
*/

bool Relay8::begin() {
    for (int i = 0; i < NUM_RELAYS; i++) {
        relay_channels[i] = new RelayChannel(i);
    }

    return true;
}



RelayChannel *Relay8::get_relay(int index) {
    if (index >= 0 && index < NUM_RELAYS) {
        return relay_channels[index];
    } else {
        return NULL;
    }
}


void RelayChannel::start() {
    output = false;
    notify();
    debugD ("Setting GPIO %d to OUTPUT", gpioMap[relay_index_]);
    pinMode (gpioMap[relay_index_],OUTPUT);
    ReactESP::app->onRepeat(5000, [this]() {
        this->notify();
    });
}

void RelayChannel::set_input(bool new_value, uint8_t input_channel) {
    debugI("Relay set_input %d = %s", relay_index_, new_value?"ON":"OFF");
    debugI("Setting PIN  %d to %s", gpioMap[relay_index_], new_value?"HIGH":"LOW");
    digitalWrite (gpioMap[relay_index_], new_value?HIGH:LOW);
    emit(new_value);
}