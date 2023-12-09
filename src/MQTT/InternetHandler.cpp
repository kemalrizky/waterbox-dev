#include "InternetHandler.h"
#include "global.h"

void InternetHandler::init() {
    WiFi.mode(WIFI_STA);
}

InternetStatusCode InternetHandler::checkConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        return internetStatus = CONNECTED;
    }
    else {
        return internetStatus = DISCONNECTED;
    }
}

InternetStatusCode InternetHandler::connect() {
    Serial.println("\nConnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed");
        return internetStatus = CONNECTED;
    }
    else {
        Serial.println("\nWiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return internetStatus = DISCONNECTED;
    } 
}