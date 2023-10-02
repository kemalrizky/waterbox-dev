#pragma once

#include "Global.h"
#include <WebServer.h>
#include <ESP2SOTA.h>

class OtaHandler {
    public:
     void initServer();
     void handleReq();
    
    private:
     WebServer *server = nullptr;
     bool isServerInitialized();
};
