#include "WaterflowSensorHandler.h"
#include "global.h"

WaterflowSensorHandler* WaterflowSensorHandler::pSensorHandler = nullptr; // Initialize the static pointer

WaterflowSensorHandler::WaterflowSensorHandler() : calibrationFactor(1.0) {
    pSensorHandler = this;
}

void IRAM_ATTR WaterflowSensorHandler::onInterrupt() {
    if (pSensorHandler != nullptr) {
        pSensorHandler->pulseCount_++;
    }
}

void WaterflowSensorHandler::init() {
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_GPIO), onInterrupt, FALLING);
}

void WaterflowSensorHandler::setCalibrationFactor(float _calibrationFactor) {
    calibrationFactor = _calibrationFactor;
}

void WaterflowSensorHandler::updateVolume() {
    noInterrupts();
    totalPulseCount_ += pulseCount_;
    byte pulse_per_sec = pulseCount_;
    pulseCount_ = 0; // pulse reset after successfully acquired
    
    // Get Flowrate
    float volume_per_sec = float(pulse_per_sec) * calibrationFactor;   // uncomment for flowrate measurement in L/s

    waterflowData_.totalVolume += volume_per_sec; // langsung ditambahkan ke volume dengan asumsi ini sudah per detik, jadi langsung dalam L saja

    interrupts();
}

void WaterflowSensorHandler::updateData() {
    noInterrupts();
    byte pulse_per_min = totalPulseCount_;
    totalPulseCount_ = 0; // pulse reset after successfully acquired
    
    // Get Flowrate
    float _avgFlowRate = float(pulse_per_min) * calibrationFactor;
    waterflowData_.flowRate = _avgFlowRate;
    waterflowData_.timestamp = 0; // implement timestamp here
    interrupts();

    publishQueue.push(waterflowData_);

    // empty waterflowData_
    waterflowData_.timestamp = 0;
    waterflowData_.flowRate = 0.0;
    waterflowData_.totalVolume = 0.0;

    if (publishQueue.size() > PUBLISH_QUEUE_MAX_SIZE) {
        Serial.println("publishQueue.size MAX, move to flushQueue.txt");
        // move publish queue into DataLogger flushQueue.txt
        // ...

        // delete all contents from publishQueue
        while(!publishQueue.empty()) {
            publishQueue.pop();
        }
    }
}

bool WaterflowSensorHandler::isEmpty() {
    return publishQueue.empty();
}

waterflowData_t WaterflowSensorHandler::getData() {
    return publishQueue.front();
}

void WaterflowSensorHandler::popData() {
    publishQueue.pop();
}

