#pragma once

#include "Global.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP2SOTA.h>

class WebServerHandler {
    public:
     void init();
     void handleReq();
};

extern WebServerHandler webServerHandler;