#pragma once

#include "Global.h"
#include "credentials.h"

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
