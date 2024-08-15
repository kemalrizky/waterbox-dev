#pragma once

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <functional>

#include "storage/ConfigStorage.h"
#include "waterflowsensor/WaterflowSensorHandler.h"
#include "MQTT/InternetHandler.h"

#define AP_SSID "ESP32-Access-Point"
#define AP_PASS "123456789"
#define PARAM_MESSAGE "message"

class ConfigWebServer {
    public:
        void init();
        void endPointRegister(AsyncWebServer *);
        static void notFound(AsyncWebServerRequest *);
    private:
        AsyncWebServer * server;
};

extern ConfigWebServer configWebServer;