#include "SensorHandler.h"

SensorHandler sensorHandler;

SensorHandler::SensorHandler() {
    sensorHandler.calibrationFactor = 0.117;
}

void IRAM_ATTR pulseCounter() {
    pulseCount++;
}

void SensorHandler::setup() {
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
}

void SensorHandler::read() {
    unsigned long now = millis();
    if (now - lastRead > 1000) {
        byte pulse1Sec = 0;
        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Get Flowrate
        sensorHandler.flowRate = ((1000.0 / (millis() - lastRead)) * pulse1Sec) * sensorHandler.calibrationFactor;   // uncomment for flowrate measurement in L/min
        // sensorHandler.flowRate = sensorHandler.flowRate / 60;                                                     // uncomment for flowrate measurement in L/s
        
        // Get Volume
        sensorHandler.volume += (sensorHandler.flowRate / 60) ;                                                  // uncomment for volume measurement in Litres 
        
        lastRead = millis();
    }
}

void SensorHandler::publish() {
    unsigned long now = millis();
    if (now - lastPublish > 1000) {
        mqttHandler.measurement = "waterbox";
        char * topic = mqttHandler.generateTopic();
        mqttHandler.publishData(topic, flowRate);
        Serial.print("Flowrate : ");
        Serial.print(flowRate);
        Serial.println(" L/min");

        delete[] topic;

        mqttHandler.measurement = "volume";
        topic = mqttHandler.generateTopic();
        mqttHandler.publishData(topic, volume);
        Serial.print("Total Volume : ");
        Serial.print(volume);
        Serial.println(" L");
        Serial.println();

        delete[] topic;

        lastPublish = now;
    } 
}

