#include "InternetHandler.h"

void InternetHandler::init() {
    WiFi.mode(WIFI_STA);
}

InternetStatusCode InternetHandler::checkConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        return CONNECTED;
    }
    else {
        return DISCONNECTED;
    }
}

InternetStatusCode InternetHandler::connect() {
    Serial.println("Connecting to WiFi..");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // polling until successfully connected or timeout
    long _timeout = millis() + 10000;
    while (WiFi.status() != WL_CONNECTED && millis() < _timeout);

    switch (WiFi.status()) {
        case WL_DISCONNECTED:
            Serial.println("WiFi disconnected.");
            return DISCONNECTED;
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("Cannot connect to SSID: \"" + String(WIFI_SSID) + "\"");
            return DISCONNECTED;
            break;
        case WL_IDLE_STATUS:
            Serial.println("Establishing connection..");
            return CONNECTING;
            break;
        case WL_CONNECTED:
            Serial.println("WiFi connected.");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            return CONNECTED;
            break;
        default:
            Serial.println("WiFi cannot connect, error code = " + String(WiFi.status()));
            return DISCONNECTED;
            break;
    }
}