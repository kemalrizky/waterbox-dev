#ifndef LED_HANDLER_H
#define LED_HANDLER_H

#include "global.h"

#define POWER_LED_PINOUT 17
#define CONNECTION_LED_PINOUT 18
#define DATA_LED_PINOUT 19

class LedHandler {
    public:
     void setup();
     void turnOn(int _pinout);
     void turnOff(int _pinout);
     void blink(int _pinout);

    private:
     unsigned long lastOn;
};

#endif