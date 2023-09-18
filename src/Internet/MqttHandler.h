#pragma once

#include "credentials.h"
const char *mqttServer = MQTT_SERVER;
#include "Misc\WaterboxId.h"
#include "InternetHandler.h"

#include <PubSubClient.h>
extern PubSubClient mqttClient;

class MqttHandler{
    public:
     String device;
     String deviceId;
     String sensor;
     String measurement;

     MqttHandler();

     void setup();
     bool connect();
     void mantainConnection();
     bool publishData(char *, float);
     void subscribe(char *);
     void loop();
     char * generateTopic();
    
    private:
     char * assignClientId();
};

void callback(char *, byte *, unsigned int);

extern MqttHandler mqttHandler;


