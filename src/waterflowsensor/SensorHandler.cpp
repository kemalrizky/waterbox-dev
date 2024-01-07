#include "SensorHandler.h"
#include "global.h"

SensorHandler* SensorHandler::pSensorHandler = nullptr; // Initialize the static pointer

SensorHandler::SensorHandler() : calibrationFactor(1.0) {
    pSensorHandler = this;
}

void IRAM_ATTR SensorHandler::onInterrupt() {
    if (pSensorHandler != nullptr) {
        pSensorHandler->pulseCount++;
    }
}

void SensorHandler::init() {
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), onInterrupt, FALLING);
}

void SensorHandler::setCalibrationFactor(float _calibrationFactor) {
    calibrationFactor = _calibrationFactor;
}

void SensorHandler::updateData(WaterFlowData *s_waterflowdata) {
    noInterrupts();
    byte pulse_per_sec = pulseCount;
    pulseCount = 0; // pulse reset
    
    // Get Flowrate
    float _flowRate = float(pulse_per_sec) * calibrationFactor;   // uncomment for flowrate measurement in L/s
    s_waterflowdata->flowRate = _flowRate;
    s_waterflowdata->totalVolume += _flowRate; // langsung ditambahkan ke volume dengan asumsi ini sudah per detik, jadi langsung dalam L saja
    s_waterflowdata->isPublished = false;

    interrupts();
}


