#include "OtaHandler.h"

void OtaHandler::init() {
    if (server == nullptr) {
        server = new WebServer(80);
        ESP2SOTA.begin(server);
        server->begin();
    }
}

void OtaHandler::handleRequest() {
    server->handleClient();                // Handle update requests
}