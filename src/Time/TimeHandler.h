#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>


class TimeHandler {
    public:
     void init();
     bool updateTime();
     unsigned long lastUpdate;
     unsigned long getCurrentTime();
     unsigned long currentTime;
    
    private:
     WiFiUDP ntpUDP;
     NTPClient *ntpClient = nullptr;

     bool isNtpInitialized();

     int timeZone;

};