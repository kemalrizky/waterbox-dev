#ifndef TIME_HANDLER_H
#define TIME_HANDLER_H

// #include <NTPClient.h>
// #include <WiFiUdp.h>
#include "../MQTT/InternetHandler.h"
#include "esp_sntp.h"
#include "time.h"

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_MULTIPLIER 7
#define DAYLIGHT_OFFSET_SEC_MULTIPLIER 1

class TimeHandler {
 public:
  void init();
  static void timeSyncCallback(timeval*);
  unsigned long getEpochTime();
  void NTPSyncSuccess();
  void printLocalTime();

 private:
  const char* ntpServer = NTP_SERVER;
  const long gmtOffset_sec = 3600 * GMT_OFFSET_MULTIPLIER;
  const int daylightOffset_sec = 3600 * DAYLIGHT_OFFSET_SEC_MULTIPLIER;
  bool initialNTPSyncSuccess = false;
};

extern TimeHandler timeHandler;

#endif