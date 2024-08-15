#include "ConfigStorage.h"

void ConfigStorage::init() {
    loadCalibrationFactor();
}

void ConfigStorage::saveCalibrationFactor() {
    preferences.begin("config", false);
    preferences.putFloat("calibFactor", calibrationFactor);
    preferences.end();

    Serial.printf("Current Calibration Factor: %f\n", calibrationFactor);
}

void ConfigStorage::loadCalibrationFactor() {
    preferences.begin("config", false);
    calibrationFactor = preferences.getFloat("calibFactor", 0.0);
    preferences.end();

    Serial.printf("Current Calibration Factor: %f\n", calibrationFactor);
}

float ConfigStorage::getCalibrationFactor()
{
    return calibrationFactor;
}

void ConfigStorage::setCalibrationFactor(float _calibrationFactor)
{
    calibrationFactor = _calibrationFactor;

    saveCalibrationFactor();
}