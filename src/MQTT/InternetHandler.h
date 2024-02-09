#pragma once

#include "credentials.h"
#include <WiFi.h>

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

