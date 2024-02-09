#include "LedHandler.h"

void LedHandler::init() {
    pinMode(LED_INTERNET_B_PINOUT, OUTPUT);
    pinMode(LED_INTERNET_G_PINOUT, OUTPUT);
    pinMode(LED_INTERNET_R_PINOUT, OUTPUT);

    pinMode(LED_TELEMETRY_PINOUT, OUTPUT);
}


void LedHandler::blinkTelemetryLed() {
    digitalWrite(LED_TELEMETRY_PINOUT, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(LED_TELEMETRY_PINOUT, LOW);
}

void LedHandler::setInternetLedOnEvent(InternetStatusCode _internetStatus) {
    switch (_internetStatus)
    {
    case CONNECTED:
        digitalWrite(LED_INTERNET_R_PINOUT, LOW);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED_INTERNET_G_PINOUT, HIGH);
        break;
    
    case DISCONNECTED:
        digitalWrite(LED_INTERNET_G_PINOUT, LOW);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED_INTERNET_R_PINOUT, HIGH);
        break;
    }
}