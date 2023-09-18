#include "WaterboxId.h"

Waterbox waterbox;

Waterbox::Waterbox() { 
    generateDeviceId();
}

void Waterbox::generateDeviceId() {
    randomSeed(millis());

    waterbox.id = "W" + String(random(1, 1001));
}