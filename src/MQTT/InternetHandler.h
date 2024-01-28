#ifndef INTERNET_HANDLER_H
#define INTERNET_HANDLER_H

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

#endif
