#ifndef INTERNET_HANDLER_H
#define INTERNET_HANDLER_H

#include "wifi_credentials.h"
#include <WiFi.h>

enum InternetStatusCode {
    DISCONNECTED = 0,
    CONNECTED,
};

class InternetHandler{
    public:
     void init();
     InternetStatusCode connect();
     InternetStatusCode checkConnection();

    private:
     InternetStatusCode internetStatus;
};

#endif
