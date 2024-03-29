#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "global.h"
#include <PubSubClient.h>
#include <WiFi.h>

// MQTT Server
#define MQTT_SERVER "161.97.179.79"

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

#endif


