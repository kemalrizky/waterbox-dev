#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include <Wire.h>
#include <queue>
#include "../Time/TimeHandler.h"
#include "../Global.h"

#define FLOW_SENSOR_GPIO 15
#define PUBLISH_QUEUE_MAX_SIZE 5

struct waterflowData_t {
    long timestamp = 0;
    float flowRate = 0.0;
    float totalVolume = 0.0;
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
     void dequeueData();

     void dummyPulse();
     static void dummyPulseTask(void *);

     long lastReadVolume = 0;
     long readVolumeInterval = READ_VOLUME_INTERVAL_MS;

     long lastReadFlow = 0;
     long readFlowInterval = READ_FLOW_INTERVAL_MS;

    private:
     static WaterflowSensorHandler* pSensorHandler;
     static void IRAM_ATTR onInterrupt();

     std::queue<waterflowData_t> publishQueue;
    //  std::queue<JSON_t> publishQueue; // change to JSON object type @kemal
     waterflowData_t waterflowData_;
     volatile unsigned int pulseTick_;
     volatile unsigned int pulsePerMin_;

     float calibrationFactor;
};

#endif