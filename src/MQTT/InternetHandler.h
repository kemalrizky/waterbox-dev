#ifndef INTERNET_HANDLER_H
#define INTERNET_HANDLER_H

#include "global.h"
#include "wifi_credentials.h"
#include <WiFi.h>

class InternetHandler{
    public:
     void init();
     bool connect();
     bool isConnected();
};

#endif
