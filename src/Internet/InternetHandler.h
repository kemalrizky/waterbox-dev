#pragma once

#include "Global.h"
#include "credentials.h"
const char *wifiSsid = WIFI_SSID;
const char *wifiPass = WIFI_PASS;

class InternetHandler{
    public:
     void wifiSetup();
     void initConnect();
     void connect();
     bool isConnected();
};

extern InternetHandler internetHandler;

#include <WiFi.h>
extern WiFiClient wifiClient;
