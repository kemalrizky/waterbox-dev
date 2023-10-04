#pragma once

#include "Global.h"
#include "credentials.h"
#include <PubSubClient.h>
#include <WiFi.h>

class MqttHandler{
    public:
     void setup();
     bool connect();
     bool publish(String, float);
     void subscribe(String);
     void loop();
     bool isConnected();
    
    private:
     WiFiClient wifiClient;
     PubSubClient mqttClient;
};

void callback(char *, byte *, unsigned int);


