#include "global.h"
#include "mqtt/InternetHandler.h"
#include "mqtt/MqttHandler.h"
#include "ota/OtaHandler.h"
#include "waterflowsensor/SensorHandler.h"
#include "led/LedHandler.h"

InternetHandler internetHandler;
MqttHandler mqttHandler;
OtaHandler otaHandler;
SensorHandler sensorHandler;
LedHandler ledHandler;

WaterFlowData wfd;

void setup() {
  Serial.begin(115200);
  ledHandler.setup();
  ledHandler.turnOn(POWER_LED_PINOUT);
  internetHandler.init();
  mqttHandler.setup();
  otaHandler.init();
  sensorHandler.init();
  sensorHandler.setCalibrationFactor(0.117);
}

void loop() {
  // update sensor data
  sensorHandler.updateData(&wfd);

  // Mantaining mqtt connection
  if(!mqttHandler.isConnected()) {              
    ledHandler.turnOff(CONNECTION_LED_PINOUT);

    if(internetHandler.checkConnection() == InternetStatusCode::DISCONNECTED) {
      if(internetHandler.connect()) {
        if(mqttHandler.connect()) {
          ledHandler.turnOn(CONNECTION_LED_PINOUT);
        }
      }                
    }
    else {
      if(mqttHandler.connect()) {
        ledHandler.turnOn(CONNECTION_LED_PINOUT);
      }
    }                   
  } else {
  // send data
    if(!sensorHandler.dataSent) {
      if(mqttHandler.publish("waterbox/W0002/flow_sensor/flowrate", wfd.flowRate) && 
          mqttHandler.publish("waterbox/W0002/flow_sensor/volume", wfd.totalVolume)) {
        ledHandler.blink(DATA_LED_PINOUT);
        sensorHandler.dataSent = true;
      }
    }
  }

  mqttHandler.loop();

  otaHandler.handleRequest();
}
