#include "TimeHandler.h"

bool TimeHandler::isNtpInitialized() {
    if(ntpClient == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

void TimeHandler::init() {
    if(!isNtpInitialized()) {
        ntpClient = new NTPClient(ntpUDP);

        ntpClient->begin();
        timeZone = 8;               // Indonesia GMT +8
        ntpClient->setTimeOffset(timeZone * 3600);
    }
}

bool TimeHandler::updateTime() {
    if(!ntpClient->update()) {
        if(!ntpClient->forceUpdate()){
            return false;
        }
        else {
            currentTime = ntpClient->getEpochTime();
            lastUpdate = millis();
            return true;
        }
    }
    else {  
        currentTime = ntpClient->getEpochTime();
        lastUpdate = millis();
        return true;
    }
}

unsigned long TimeHandler::getCurrentTime() {
    unsigned long _currentTime = currentTime + (millis() - lastUpdate);
    return _currentTime;
}