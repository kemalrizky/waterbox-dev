#ifndef DEVICEIDGENERATOR_H
#define DEVICEIDGENERATOR_H

#include "Global.h"
#include <WiFi.h>

class DeviceIdGenerator {
    public:
     void init();
     String getId();

    private:
     void generateId();
     String deviceId_;
};

extern DeviceIdGenerator deviceIdGenerator;
#endif