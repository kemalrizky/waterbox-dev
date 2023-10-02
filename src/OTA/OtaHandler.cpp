#include "OtaHandler.h"

bool OtaHandler::isServerInitialized() {
    if (server == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

void OtaHandler::initServer() {
    if (isServerInitialized()) {
        server = new WebServer(80);
        ESP2SOTA.begin(server);
        server->begin();
    }
}

void OtaHandler::handleReq() {
    server->handleClient();                // Handle update requests
}