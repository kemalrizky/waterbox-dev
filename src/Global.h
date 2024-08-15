#pragma once

#include <Arduino.h>
#include "MQTT/credentials.h"

#ifndef WIFI_CREDENTIALS_H
// Internet
#ifndef WIFI_SSID
#define WIFI_SSID "FILL_IN_WIFI_SSID_HERE"
#define WIFI_PASS "FILL_IN_WIFI_PASS_HERE"
#endif

// MQTT Server
#ifndef MQTT_SERVER
#define MQTT_SERVER "FILL_IN_MQTT_SERVER_HERE"
#endif
#endif // WIFI_CREDENTIALS_H

// Waterflow Business Logic
#define READ_VOLUME_INTERVAL_MS    1000
#define READ_FLOW_INTERVAL_MS      60000 // 60000
#define PUBLISH_INTERVAL_MS        60000 // 60000