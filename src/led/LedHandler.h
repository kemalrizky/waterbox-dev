#pragma once

#include "Global.h"
#include "MQTT/InternetHandler.h"

#define LED_INTERNET_R_PINOUT 27
#define LED_INTERNET_G_PINOUT 26
#define LED_INTERNET_B_PINOUT 25

#define LED_TELEMETRY_PINOUT 13

enum LedEvent{
    WIFI_DISCONNECTED = 0,
    WIFI_CONNECTED,
    TELEMETRY_SUCCEEDED,

};

class LedHandler {
    public:
     void init();
     void setInternetLedOnEvent(InternetStatusCode);
     void blinkTelemetryLed();
     
};

extern LedHandler ledHandler;
