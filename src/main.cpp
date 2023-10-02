#include "Global.h"
#include "Internet\InternetHandler.h"
#include "Internet\MqttHandler.h"
#include "OTA\OtaHandler.h"
#include "Sensor\SensorHandler.h"

InternetHandler internetHandler;
MqttHandler mqttHandler;
OtaHandler otaHandler;
SensorHandler sensorHandler;

void setup() {
  Serial.begin(115200);
  internetHandler.wifiSetup();
  internetHandler.connect();

  mqttHandler.setup();

  otaHandler.initServer();
  sensorHandler.setup();
  sensorHandler.setCalibrationFactor(0.117);
}

void loop() {
  sensorHandler.readFlowrate();
  sensorHandler.calculateVolume();

  if(!mqttHandler.isConnected()) {              // Check if Waterbox is connected to MQTTClient
    if(!internetHandler.isConnected()) {
      internetHandler.connect();                // Connect to WiFi
    }
    mqttHandler.connect();                      // Connect to MQTT Broker
  }

  if(mqttHandler.isConnected() && sensorHandler.isFlowrateRead == true) {
    mqttHandler.publish("waterbox/W0002/flow_sensor/flowrate", sensorHandler.getFlowrate());
    mqttHandler.publish("waterbox/W0002/flow_sensor/volume", sensorHandler.getVolume());
  }

  mqttHandler.loop();

  otaHandler.handleReq();
}
