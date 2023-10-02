#include "InternetHandler.h"

const char *wifiSsid = WIFI_SSID;
const char *wifiPass = WIFI_PASS;

void InternetHandler::wifiSetup() {
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
    Serial.println("\nConnecting to WiFi...");
    
    if (isConnected()) {
        WiFi.disconnect();
    }
    
    WiFi.begin(wifiSsid, wifiPass);

    if (!isConnected()) {
        Serial.println("\nWiFi connection failed");
        return false;
    }
    else {
        Serial.println("\nWiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    
}