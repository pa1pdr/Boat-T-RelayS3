#pragma once
#include <Arduino.h>
#include "sensesp/signalk/signalk_put_request_listener.h"
#include "sensesp/system/valueconsumer.h"
#include "sensesp/system/valueproducer.h"
#include "sensesp/system/startable.h"



using namespace sensesp;

#define NUM_RELAYS  8

class RelayChannel : public Startable,
 //                    public Configurable,
                     public BoolProducer,
                     public BoolConsumer {
    public:
/*
         RelayChannel (int index,String description="",int sort_order = 1000) : Configurable ([index] {return "/relay" + String(index);}(),description,sort_order) {
            relay_index_ = index;
            debugD ("Setting GPIO %d to OUTPUT", gpioMap[index]);
            pinMode (gpioMap[index],OUTPUT);
            load_configuration();
        }


        void get_configuration(JsonObject &root) override;
        String get_config_schema() override;
        bool set_configuration(const JsonObject &config) override;
*/
        RelayChannel(int index) {
            relay_index_ = index;
        }

        void start() override;
        //virtual String get_configuration (JsonObject &root) override;
        void set_input(bool new_value, uint8_t input_channel = 0) override;
    private:
        int relay_index_ = 0;
        // relays connected to GPIO32, GPIO33, GPIO25, GPIO26, GPIO27, GPIO14, GPIO12 and GPIO13
        int gpioMap[NUM_RELAYS] = {GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};

};

class Relay8 {
    public:
        bool begin();
        RelayChannel *get_relay(int index);

 
    private:
        RelayChannel *relay_channels[NUM_RELAYS];
    
};