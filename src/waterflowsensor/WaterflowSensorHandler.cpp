#include "WaterflowSensorHandler.h"
#include "global.h"

WaterflowSensorHandler* WaterflowSensorHandler::pSensorHandler = nullptr; // Initialize the static pointer

WaterflowSensorHandler::WaterflowSensorHandler() : calibrationFactor(1.0) {
    pSensorHandler = this;
}

void IRAM_ATTR WaterflowSensorHandler::onInterrupt() {
    if (pSensorHandler != nullptr) {
        pSensorHandler->pulseTick_++;
    }
}

void WaterflowSensorHandler::init() {
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_GPIO), onInterrupt, FALLING);
}

void WaterflowSensorHandler::setCalibrationFactor(float _calibrationFactor) {
    calibrationFactor = _calibrationFactor;
}

void WaterflowSensorHandler::updateVolumePerSec() {
    noInterrupts();
    pulsePerMin_ += pulseTick_;
    uint32_t _pulsePerSec = pulseTick_;
    pulseTick_ = 0; // pulseTick_ reset after successfully acquired
    
    // Get Flowrate
    float _volumePerSec = float(_pulsePerSec) * calibrationFactor;   // uncomment for flowrate measurement in L/s

    waterflowData_.totalVolume += _volumePerSec; // langsung ditambahkan ke volume dengan asumsi ini sudah per detik, jadi langsung dalam L saja

    interrupts();
}

void WaterflowSensorHandler::updateQueuePerMin() {
    // assumption: flow rate can only be determined after 1 minute of captured flow pulse/tick
    noInterrupts();
    uint32_t _pulsePerMin = pulsePerMin_;
    pulsePerMin_ = 0; // pulsePerMin_ reset after successfully acquired
    
    // Get Flowrate
    float _avgFlowRate = float(_pulsePerMin) * calibrationFactor;
    waterflowData_.flowRate = _avgFlowRate;
    waterflowData_.timestamp = 0; // implement timestamp here
    interrupts();

    publishQueue.push(waterflowData_);

    // empty waterflowData_
    waterflowData_.timestamp = 0;
    waterflowData_.flowRate = 0.0;
    waterflowData_.totalVolume = 0.0;

    if (publishQueue.size() > PUBLISH_QUEUE_MAX_SIZE) {
        // discard data, in the assumption that normally queue will never reach MAX size
        Serial.println("publishQueue.size MAX, data discarded");
        // give log that publishQueue.size maxed out

        // // alternative: move data to flushQueue.txt, then delete all contents from publishQueue
        // Serial.println("publishQueue.size MAX, move to flushQueue.txt");
        // // move contents of publishQueue into DataLogger to fill in flushQueue.txt
        // // ...

        // while(!publishQueue.empty()) {
        //     publishQueue.pop();
        // }
    }
}

bool WaterflowSensorHandler::isEmpty() {
    return publishQueue.empty();
}

waterflowData_t WaterflowSensorHandler::getData() {
    return publishQueue.front();
}

void WaterflowSensorHandler::dequeueData() {
    // add mutex for safety of multiple data access
    // ...
    publishQueue.pop();
}

void WaterflowSensorHandler::dummyPulse() {
    pulseTick_++;
}

void WaterflowSensorHandler::dummyPulseTask(void * pv) {
    WaterflowSensorHandler * waterflowSensorHandler = (WaterflowSensorHandler *) pv;
    
    while(1) {
        if (random(300) < 250) waterflowSensorHandler->dummyPulse();
        vTaskDelay(100);
    }
}