#include "InternetHandler.h"

void InternetHandler::init() {
    WiFi.mode(WIFI_STA);
}

bool InternetHandler::isConnected() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    else {
        return false;
    }
}

bool InternetHandler::connect() {
    SERIAL_PRINTLN("\nConnecting to WiFi...");
    
    if (isConnected()) {
        WiFi.disconnect();
    }
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    if (!isConnected()) {
        SERIAL_PRINTLN("\nWiFi connection failed");
        return false;
    }
    else {
        SERIAL_PRINTLN("\nWiFi connected");
        SERIAL_PRINT("IP address: ");
        SERIAL_PRINTLN(WiFi.localIP());
        return true;
    }
    
}