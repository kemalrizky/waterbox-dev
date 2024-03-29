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
  // sensorHandler.readFlowrate();
  // sensorHandler.calculateVolume();

  // // Mantaining mqtt connection
  // if(!mqttHandler.isConnected()) {              
  //   ledHandler.turnOff(CONNECTION_LED_PINOUT);

  //   if(!internetHandler.isConnected()) {
  //     if(internetHandler.connect()) {
  //       if(mqttHandler.connect()) {
  //         ledHandler.turnOn(CONNECTION_LED_PINOUT);
  //       }
  //     }                
  //   }
  //   else {
  //     if(mqttHandler.connect()) {
  //       ledHandler.turnOn(CONNECTION_LED_PINOUT);
  //     }
  //   }                   
  // }

  // if(mqttHandler.isConnected() && sensorHandler.isFlowrateRead == true) {
  //   if(mqttHandler.publish("waterbox/W0002/flow_sensor/flowrate", sensorHandler.getFlowrate()) || 
  //       mqttHandler.publish("waterbox/W0002/flow_sensor/volume", sensorHandler.getVolume())) {
  //     ledHandler.blink(DATA_LED_PINOUT);
  //   }
  // }

  mqttHandler.loop();

  otaHandler.handleRequest();
}
