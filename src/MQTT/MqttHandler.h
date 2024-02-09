#pragma once

#include "Global.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "../led/LedHandler.h"
#include "InternetHandler.h"
#include "DeviceIdGenerator.h"
#include <ArduinoJson.h>
#include "led/LedHandler.h"

class MqttHandler{
    public:
     void init();
     bool connect();
     bool publish(String, String);
     bool publish(String, long);
     bool publish(String, float);
     bool publish(String, JsonDocument);
     void subscribe(String);
     void loop();
     bool isConnected();

     static void reconnectTask(void *);
    
    private:
     WiFiClient wifiClient;
     PubSubClient mqttClient;
};

void callback(char *, byte *, unsigned int);



