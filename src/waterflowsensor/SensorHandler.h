#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include <Wire.h>

#define FLOW_SENSOR 15

struct WaterFlowData {
    float flowRate;
    float totalVolume;
};

class SensorHandler {
    public:
     SensorHandler();
     void init();
     void setCalibrationFactor(float);
     void readData(WaterFlowData *);

    private:
     static SensorHandler* pSensorHandler;
     static void IRAM_ATTR onInterrupt();

     volatile unsigned int pulseCount;
     float calibrationFactor;
};

#endif