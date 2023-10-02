#pragma once

#include "Global.h"
#include <Wire.h>

#define FLOW_SENSOR 15

class SensorHandler {
    public:
     SensorHandler(float _calibrationFactor);
     void setup();
     void readFlowrate();
     void calculateVolume();
     float getFlowrate();
     float getVolume();

     bool isFlowrateRead;

    private:
     static SensorHandler* instance;
     static void IRAM_ATTR pulseCounter();

     volatile byte pulseCount;
     unsigned long lastRead;
     unsigned long lastPublish; 
     
     float flowRate;
     float volume;
     float calibrationFactor;
};