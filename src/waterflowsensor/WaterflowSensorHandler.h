#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include <Wire.h>
#include <queue>

#define FLOW_SENSOR_GPIO 15
#define PUBLISH_QUEUE_MAX_SIZE 5

struct waterflowData_t {
    long timestamp;
    float flowRate;
    float totalVolume;
};

class WaterflowSensorHandler {
    public:
     WaterflowSensorHandler();
     void init();
     void setCalibrationFactor(float);
     void updateVolumePerSec();
     void updateQueuePerMin();

     bool isEmpty();
     waterflowData_t getData();
     void popData();

     void dummyPulse();
     static void dummyPulseTask(void *);

     long lastReadVolume = 0;
     long readVolumeInterval = 1000;

     long lastReadFlow = 0;
     long readFlowInterval = 60000;

    private:
     static WaterflowSensorHandler* pSensorHandler;
     static void IRAM_ATTR onInterrupt();

     std::queue<waterflowData_t> publishQueue;
     waterflowData_t waterflowData_;
     volatile unsigned int pulseTick_;
     volatile unsigned int pulsePerMin_;

     float calibrationFactor;
};

#endif