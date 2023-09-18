#pragma once

#include "Global.h"
#include <Wire.h>
#include "Internet\MqttHandler.h"

#define FLOW_SENSOR 15

class SensorHandler {
    public:
     float flowRate;
     float volume;
     float calibrationFactor;
     void setup();
     void read();
     void publish();
     SensorHandler();
};

volatile byte pulseCount;
unsigned long lastRead;
unsigned long lastPublish;

void IRAM_ATTR pulseCounter();

extern SensorHandler sensorHandler;