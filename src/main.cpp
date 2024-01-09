#include "global.h"
#include "mqtt/InternetHandler.h"
#include "mqtt/MqttHandler.h"
#include "ota/OtaHandler.h"
#include "waterflowsensor/WaterflowSensorHandler.h"
#include "led/LedHandler.h"
#include "DataLogger/FileHandler.h"
#include "DataLogger/DataLogger.h"

LedHandler ledHandler;

InternetHandler internetHandler;
MqttHandler mqttHandler;
OtaHandler otaHandler;

WaterflowSensorHandler waterflowSensorHandler;

DataLogger dataLogger;

void mqttReconnectTask(void *);
void publishTask(void *);
void flushTask(void *);

void setup() {
  Serial.begin(115200);

  ledHandler.setup();
  ledHandler.turnOn(POWER_LED_PINOUT);

  internetHandler.init();
  mqttHandler.setup();
  otaHandler.init();

  xTaskCreate(mqttReconnectTask, "mqttReconnectTask", 1024 * 1, NULL, 3, NULL);
  xTaskCreate(publishTask, "publishTask", 1024 * 2, NULL, 1, NULL);
  // xTaskCreate(flushTask, "flushTask", 1024 * 1, NULL, 1, NULL);

  waterflowSensorHandler.init();
  waterflowSensorHandler.setCalibrationFactor(0.117);
}

void loop() {
  // update sensor data
  if (millis() - waterflowSensorHandler.lastReadVolume > waterflowSensorHandler.readVolumeInterval) {
    waterflowSensorHandler.updateVolume();
    waterflowSensorHandler.lastReadVolume = millis();
  }
  
  if (millis() - waterflowSensorHandler.lastReadFlow > waterflowSensorHandler.readFlowInterval) {
    waterflowSensorHandler.updateData();
    waterflowSensorHandler.lastReadFlow = millis();
  }

  mqttHandler.loop();
  otaHandler.handleRequest();
}

void mqttReconnectTask(void * pv) {
  while(1) {
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
    }

    vTaskDelay(1000); 
  }
}

void publishTask(void * pv) {  
  long lastPublish = 0;
  long publishInterval = 60000;

  while(1) {
    // send data
    if (millis() - lastPublish > publishInterval) {
      if(mqttHandler.isConnected()) {
        while(!waterflowSensorHandler.isEmpty()) {
          if(mqttHandler.publish("waterbox/W0002/flow_sensor/flowrate", waterflowSensorHandler.getData().flowRate) && 
              mqttHandler.publish("waterbox/W0002/flow_sensor/volume", waterflowSensorHandler.getData().totalVolume)) {
            waterflowSensorHandler.popData();

            ledHandler.blink(DATA_LED_PINOUT);
          }

          vTaskDelay(1000);
        }
      }
    }
    vTaskDelay(1000);
  }
}

void flushTask(void * pv) {
  while(1) {
    // check if flushQueue.txt is not empty
    //
    vTaskDelay(1000); 
  }
}