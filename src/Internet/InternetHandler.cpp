#include "InternetHandler.h"

InternetHandler internethandler;
WiFiClient wifiClient;

void InternetHandler::wifiSetup() {
    WiFi.mode(WIFI_STA);
}

void InternetHandler::initConnect() {
    delay(10);
    unsigned long lastAttempt = millis();

    Serial.println("\nInitializing WiFi connection...");

    WiFi.begin(wifiSsid, wifiPass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        
        // give up connection attempt if failed to connect after trying for 10 second
        unsigned long now = millis();
        if (lastAttempt - now > 5000){
            lastAttempt = millis();
            break;
        }
    }
    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void InternetHandler::connect() {
    Serial.println("\nConnecting to WiFi...");
    
    WiFi.disconnect();
    WiFi.begin(wifiSsid, wifiPass);
    
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

bool InternetHandler::isConnected() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    else {
        return false;
    }
}

