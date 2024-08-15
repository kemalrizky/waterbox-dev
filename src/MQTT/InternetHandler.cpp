#include "InternetHandler.h"

void InternetHandler::init() {
#if defined(WIFI_MODE_STA)
    // STA Mode
    Serial.println("WiFi Mode: Station");
    WiFi.mode(WIFI_STA);
#elif defined(WIFI_MODE_AP)
    // AP Mode
    Serial.println("WiFi Mode: Access Point (AP)");
    WiFi.mode(WIFI_AP);
#elif defined(WIFI_MODE_AP_STA)
    // AP & STA Mode
    Serial.println("WiFi Mode: Access Point (AP) & Station");
    WiFi.mode(WIFI_AP_STA);
#endif // WIFI_MODE_STA
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
#if defined(WIFI_MODE_STA) || defined(WIFI_MODE_AP_STA)
    Serial.println("Connecting to WiFi..");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // polling until successfully connected or timeout
    long _timeout = millis() + 10000;
    while (WiFi.status() != WL_CONNECTED && millis() < _timeout);
#else
    Serial.println("WiFi Mode STA inactive.. do nothing");
#endif // WIFI_MODE_STA || defined(WIFI_MODE_AP_STA)

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
            Serial.print("STA IP address: ");
            Serial.println(WiFi.localIP());
            return CONNECTED;
            break;
        default:
            Serial.println("WiFi cannot connect, error code = " + String(WiFi.status()));
            return DISCONNECTED;
            break;
    }

#ifdef WIFI_MODE_AP || defined(WIFI_MODE_AP_STA)
    Serial.println("Activating WiFi AP \"" + String(AP_SSID) + "\"");
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());
#endif
}