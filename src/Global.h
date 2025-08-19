#pragma once

#include <Arduino.h>

// Internet
#ifndef WIFI_SSID
#define WIFI_SSID "WBX_AP"
#define WIFI_PASS "groundEx2022"
#endif

// MQTT Server
#ifndef MQTT_SERVER
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#endif

// Waterflow Business Logic
#define READ_VOLUME_INTERVAL_MS    1000
#define READ_FLOW_INTERVAL_MS      60000 // 60000
#define PUBLISH_INTERVAL_MS        60000 // 60000