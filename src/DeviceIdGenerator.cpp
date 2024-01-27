#include "DeviceIdGenerator.h"

void DeviceIdGenerator::generateId() {
    String _macAddr = WiFi.macAddress();
    deviceId_ = "WBX-" + _macAddr.substring(0,2) + _macAddr.substring(6,8) + _macAddr.substring(12,14);
}

void DeviceIdGenerator::init() {
    generateId();
    Serial.println("Waterbox device Id: " + deviceId_);
}

String DeviceIdGenerator::getId() {
    return deviceId_;
}