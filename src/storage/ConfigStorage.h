#pragma once

#include <Preferences.h>

class ConfigStorage {
    public:
        void init();
        void saveCalibrationFactor();
        void loadCalibrationFactor();
        void setCalibrationFactor(float);
        float getCalibrationFactor();

    private:
        float calibrationFactor = 0;
        Preferences preferences;
};

extern ConfigStorage configStorage;