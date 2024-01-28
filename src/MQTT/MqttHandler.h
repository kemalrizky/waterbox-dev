#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "Global.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "../led/LedHandler.h"
#include "InternetHandler.h"
#include "DeviceIdGenerator.h"
#include <ArduinoJson.h>

class MqttHandler{
    public:
     void setup();
     bool connect();
     bool publish(JsonDocument);
     void subscribe(String);
     void loop();
     bool isConnected();

     static void reconnectTask(void *);
    
    private:
     WiFiClient wifiClient;
     PubSubClient mqttClient;
};

void callback(char *, byte *, unsigned int);

#endif


