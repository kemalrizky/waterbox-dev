#pragma once

#include <Preferences.h>

class ConfigStorage {
    public:
        void init();
        void setCalibrationFactor(float);
        float getCalibrationFactor();

    private:
        void saveCalibrationFactor();
        void loadCalibrationFactor();
        float calibrationFactor = 0;
        Preferences preferences;
};

extern ConfigStorage configStorage;