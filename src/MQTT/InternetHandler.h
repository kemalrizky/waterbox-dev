#pragma once

#include "Global.h"
#include "credentials.h"
#include <WiFi.h>

class InternetHandler{
    public:
     void wifiSetup();
     bool connect();
     bool isConnected();
};

