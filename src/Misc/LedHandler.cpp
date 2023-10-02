#include "LedHandler.h"

void LedHandler::setup() {
    pinMode(POWER_LED_PINOUT, OUTPUT);
    pinMode(CONNECTION_LED_PINOUT, OUTPUT);
    pinMode(DATA_LED_PINOUT, OUTPUT);
}

void LedHandler::turnOn(int _pinout) {
    digitalWrite(_pinout, HIGH);
}

void LedHandler::turnOff(int _pinout) {
    digitalWrite(_pinout, LOW);
}

void LedHandler::blink(int _pinout) {
    digitalWrite(_pinout, HIGH);
    
    lastOn = millis();
    while(lastOn - millis() < 500){}

    digitalWrite(_pinout, LOW);
}