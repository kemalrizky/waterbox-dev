#include "TimeHandler.h"

void TimeHandler::init()
{
    long _timeout = millis() + 10000;
    while (internetHandler.checkConnection() != InternetStatusCode::CONNECTED && (millis() < _timeout));
  
    if (internetHandler.checkConnection() == InternetStatusCode::CONNECTED) {
        sntp_set_time_sync_notification_cb(timeSyncCallback);
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // JUST THIS  
    } else {
      Serial.println("TimeHandler::init - internet is NOT connected, failed to do initial time sync. reboot");
        // log error, refuse to continue
        ESP.restart();
    }
}

void TimeHandler::timeSyncCallback(timeval * tmv) {
  Serial.println("Success to obtain time from NTP, RTC updated!");
  
  // set a flag here if needed time synced
  timeHandler.NTPSyncSuccess();
}

void TimeHandler::NTPSyncSuccess() {
  initialNTPSyncSuccess = true;
}

// getEpochTime() Function that gets current epoch time. already in unix timestamp format
unsigned long TimeHandler::getEpochTime() {
  if (!initialNTPSyncSuccess) {
    Serial.println("Initial NTP Sync failed. Reboot");
    ESP.restart();
  }

  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return(0);
  }

  time(&now);
  return now;
}

void TimeHandler::printLocalTime()
{
  // print local time in a readable format, for debugging purposes
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%d/%b/%Y %H:%M:%S");
}