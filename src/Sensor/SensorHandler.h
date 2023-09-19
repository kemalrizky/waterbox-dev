#pragma once

#include "Global.h"
#include <Wire.h>
#include "Internet\MqttHandler.h"

#define FLOW_SENSOR 15

class SensorHandler {
    public:
     volatile byte pulseCount;
     unsigned long lastRead;
     unsigned long lastPublish; 

     float flowRate;
     float volume;
     float calibrationFactor;
     void setup();
     void read();
     void publish();
     SensorHandler();
};



void IRAM_ATTR pulseCounter();

extern SensorHandler sensorHandler;