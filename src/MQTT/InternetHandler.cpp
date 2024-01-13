#include "InternetHandler.h"
#include "global.h"

InternetStatusCode InternetHandler::init() {
    WiFi.mode(WIFI_STA);
    Serial.println("\nConnecting to WiFi...");
    WiFi.begin(wifi_ssid, wifi_pass);
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed");
        return internetStatus = DISCONNECTED;
    }
    else {
        Serial.print("\n Connected to");
        Serial.println(wifi_ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return internetStatus = CONNECTED;
    } 
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
    WiFi.begin(wifi_ssid, wifi_pass);

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed");
        return internetStatus = DISCONNECTED;
    }
    else {
        Serial.print("\n Connected to");
        Serial.println(wifi_ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return internetStatus = CONNECTED;
    } 
}