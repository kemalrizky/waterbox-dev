#pragma once

#include <WebServer.h>
#include <ESP2SOTA.h>
#include <ESPmDNS.h>
#include <DeviceIdGenerator.h>

class OtaHandler {
    public:
     void init();
     void handleRequest();
    
    private:
     WebServer *server = nullptr;
};

