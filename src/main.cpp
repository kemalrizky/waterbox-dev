#include "Global.h"
#include "DeviceIdGenerator.h"
#include "MQTT/InternetHandler.h"
#include "MQTT/MqttHandler.h"
#include "OTA/OtaHandler.h"
#include "waterflowsensor/WaterflowSensorHandler.h"
#include "led/LedHandler.h"
#include "DataLogger/FileHandler.h"
#include "DataLogger/DataLogger.h"

TimeHandler timeHandler;
LedHandler ledHandler;
DeviceIdGenerator deviceIdGenerator;

InternetHandler internetHandler;
MqttHandler mqttHandler;
OtaHandler otaHandler;

WaterflowSensorHandler waterflowSensorHandler;

DataLogger dataLogger;

void publishTask(void *);
void flushTask(void *);

void setup() {
  Serial.begin(115200);

  deviceIdGenerator.init();

  ledHandler.init();

  internetHandler.init();
  mqttHandler.init();
  otaHandler.init();

  xTaskCreate(mqttHandler.reconnectTask, "mqttReconnectTask", 1024 * 5, &mqttHandler, 3, NULL);
  xTaskCreate(publishTask, "publishTask", 1024 * 2, NULL, 1, NULL);
  // xTaskCreate(flushTask, "flushTask", 1024 * 1, NULL, 1, NULL);

  timeHandler.init();

  // DUMMY WATERFLOW INTERRUPT FOR DEVELOPMENT PURPOSES
  xTaskCreate(waterflowSensorHandler.dummyPulseTask, "dummyPulseTask", 1024 * 1, &waterflowSensorHandler, 1, NULL);


  waterflowSensorHandler.init();
  waterflowSensorHandler.setCalibrationFactor(0.117);
}

void loop() {
  // update sensor data
  if (millis() - waterflowSensorHandler.lastReadVolume > waterflowSensorHandler.readVolumeInterval) {
    waterflowSensorHandler.updateVolumePerSec();
    waterflowSensorHandler.lastReadVolume = millis();
  }
  
  if (millis() - waterflowSensorHandler.lastReadFlow > waterflowSensorHandler.readFlowInterval) {
    timeHandler.printLocalTime();
    waterflowSensorHandler.updateQueuePerMin();
    waterflowSensorHandler.lastReadFlow = millis();
  }

  mqttHandler.loop();
  otaHandler.handleRequest();

}

void publishTask(void * pv) {  
  long lastPublish = 0;
  long publishInterval = PUBLISH_INTERVAL_MS;

  while(1) {
    // send data
    if (millis() - lastPublish > publishInterval) {
      if(mqttHandler.isConnected()) {
        while(!waterflowSensorHandler.isEmpty()) {
          if(mqttHandler.publish(String("waterbox/" + deviceIdGenerator.getId()), waterflowSensorHandler.getData())) {
            waterflowSensorHandler.dequeueData(); // removed successfully published data from waterflowSensorHandler.publishQueue

            ledHandler.blinkTelemetryLed();
          }

          vTaskDelay(1000);
        }
        lastPublish = millis();
      } else {
        // log that mqtt not connected
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