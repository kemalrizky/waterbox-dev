#include "WebServerHandler.h"

WebServerHandler webServerHandler;
WebServer server(80);

void WebServerHandler::init() {
    delay(1000);
    /* SETUP YOR WEB OWN ENTRY POINTS */
    server.on("/myurl", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", "Hello there!");
    });

    ESP2SOTA.begin(&server);
    server.begin();
}

void WebServerHandler::handleReq() {
    server.handleClient();                // Handle update requests
    delay(5);
}