#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "global.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "../led/LedHandler.h"
#include "InternetHandler.h"

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

     static void reconnectTask(void *);
    
    private:
     WiFiClient wifiClient;
     PubSubClient mqttClient;
};

void callback(char *, byte *, unsigned int);

#endif


