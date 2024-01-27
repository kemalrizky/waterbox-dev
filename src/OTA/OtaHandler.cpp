#include "OtaHandler.h"

void OtaHandler::init() {
    if (server == nullptr) {
        server = new WebServer(80);
        ESP2SOTA.begin(server);
        server->begin();
    }
    Serial.println("\nESP server initialized");

    Serial.println("Setting up MDNS responder");
    // Initialize mDNS
    if (!MDNS.begin(deviceIdGenerator.getId().c_str())) {   
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("MDNS responder setup success!");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 23);
}

void OtaHandler::handleRequest() {
    server->handleClient();                // Handle update requests
}