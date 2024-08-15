#pragma once

#include "credentials.h"
#include <WiFi.h>
#include "Global.h"


// #define WIFI_MODE_STA
// #define WIFI_MODE_AP
#define WIFI_MODE_AP_STA

enum InternetStatusCode {
    DISCONNECTED = 0,
    CONNECTED,
    CONNECTING,
};

class InternetHandler{
    public:
     void init();
     InternetStatusCode connect();
     InternetStatusCode checkConnection();
};

extern InternetHandler internetHandler;

