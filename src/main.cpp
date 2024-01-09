#include "global.h"
#include "mqtt/InternetHandler.h"
#include "mqtt/MqttHandler.h"
#include "ota/OtaHandler.h"
#include "waterflowsensor/SensorHandler.h"
#include "led/LedHandler.h"
#include "FileHandler.h"
#include "DataLogger.h"

LedHandler ledHandler;

InternetHandler internetHandler;
MqttHandler mqttHandler;
OtaHandler otaHandler;

SensorHandler sensorHandler;
WaterflowData wfd;

DataLogger dataLogger;

void mqttReconnectTask(void *);

void setup() {
  Serial.begin(115200);

  dataLogger.init();
}

void loop() {

}

// void setup() {
//   Serial.begin(115200);

//   ledHandler.setup();
//   ledHandler.turnOn(POWER_LED_PINOUT);

//   internetHandler.init();
//   mqttHandler.setup();
//   otaHandler.init();
//   xTaskCreate(mqttReconnectTask, "mqttReconnectTask", 1024 * 1, NULL, 1, NULL);

//   sensorHandler.init();
//   sensorHandler.setCalibrationFactor(0.117);
// }

// void loop() {
//   // update sensor data
//   if (millis() - sensorHandler.lastRead > sensorHandler.readInterval) {
//     sensorHandler.updateData(&wfd);
//     sensorHandler.lastRead = millis();
//   }
  
//   // send data
//   if (millis() - sensorHandler.lastPublish > sensorHandler.publishInterval) {
//     if(mqttHandler.isConnected()) {
//       if(!wfd.isPublished) {
//         if(mqttHandler.publish("waterbox/W0002/flow_sensor/flowrate", wfd.flowRate) && 
//             mqttHandler.publish("waterbox/W0002/flow_sensor/volume", wfd.totalVolume)) {
//           wfd.totalVolume = 0; // volume reset after successful publish
//           ledHandler.blink(DATA_LED_PINOUT);
//           wfd.isPublished = true;
//           sensorHandler.lastPublish = millis();
//         }
//       }
//     }
//   }

//   mqttHandler.loop();
//   otaHandler.handleRequest();
// }

// void mqttReconnectTask(void * pv) {
//   while(1) {
//     // Mantaining mqtt connection
//     if(!mqttHandler.isConnected()) {              
//       ledHandler.turnOff(CONNECTION_LED_PINOUT);

//       if(internetHandler.checkConnection() == InternetStatusCode::DISCONNECTED) {
//         if(internetHandler.connect()) {
//           if(mqttHandler.connect()) {
//             ledHandler.turnOn(CONNECTION_LED_PINOUT);
//           }
//         }                
//       }
//       else {
//         if(mqttHandler.connect()) {
//           ledHandler.turnOn(CONNECTION_LED_PINOUT);
//         }
//       }                   
//     }

//     vTaskDelay(1000); 
//   }
// }