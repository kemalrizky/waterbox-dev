// must mantain this order of dependency
#include "Global.h"
#include "Misc\WaterboxId.h" 
#include "Internet\InternetHandler.h"
#include "Internet\MqttHandler.h"
#include "WebServer\WebServerHandler.h"
#include "Sensor\SensorHandler.h"

void setup() {
  Serial.begin(115200);
  internetHandler.wifiSetup();
  internetHandler.initConnect();

  mqttHandler.setup();

  webServerHandler.init();
  sensorHandler.setup();
}

void loop() {
  mqttHandler.mantainConnection();
  mqttHandler.loop();

  sensorHandler.read();
  sensorHandler.publish();

  webServerHandler.handleReq();
}
