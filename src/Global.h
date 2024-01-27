#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

// #define SERIAL_PRINTLN(arg) (Serial.println(arg))
// #define SERIAL_PRINT(arg) (Serial.print(arg))

#ifndef WIFI_SSID
#define WIFI_SSID "FILL_IN_WIFI_SSID_HERE"
#define WIFI_PASS "FILL_IN_WIFI_PASS_HERE"
#endif

// MQTT Server
#ifndef MQTT_SERVER
#define MQTT_SERVER "FILL_IN_MQTT_SERVER_HERE"
#endif

#endif