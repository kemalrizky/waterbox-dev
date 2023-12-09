#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include "global.h"
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

#endif
