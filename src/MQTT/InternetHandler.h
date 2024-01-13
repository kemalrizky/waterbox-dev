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
     InternetStatusCode init();
     InternetStatusCode connect();
     InternetStatusCode checkConnection();

    private:
     InternetStatusCode internetStatus;
     const char *wifi_ssid = WIFI_SSID;
     const char *wifi_pass = WIFI_PASS;
};

#endif
