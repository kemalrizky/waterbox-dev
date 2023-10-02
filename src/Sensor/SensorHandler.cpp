#include "SensorHandler.h"

SensorHandler* SensorHandler::instance = nullptr; // Initialize the static pointer


SensorHandler::SensorHandler(float _calibrationFactor): calibrationFactor(_calibrationFactor) {
    instance = this;
}

void IRAM_ATTR SensorHandler::pulseCounter() {
    if (instance != nullptr) {
        instance->pulseCount++;
    }
}

void SensorHandler::setup() {
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
}

void SensorHandler::readFlowrate() {
    unsigned long now = millis();
    if (now - lastRead > 1000) {
        noInterrupts();
        byte pulse1Sec = pulseCount;
        pulseCount = 0;
        interrupts();

        // Get Flowrate
        flowRate = float(pulse1Sec) * calibrationFactor * (1000.0 / (millis() - now));   // uncomment for flowrate measurement in L/s
        
        lastRead = now;
        isFlowrateRead = true;
    }
    else {
        isFlowrateRead = false;
    }
}

void SensorHandler::calculateVolume() {
    if(isFlowrateRead) {
        volume += flowRate; 
    }
}

float SensorHandler::getFlowrate() {
    float _flowRate = flowRate;
    return _flowRate;
}

float SensorHandler::getVolume() {
    float _volume = volume;
    return _volume;
}



