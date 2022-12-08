
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqttHandler.h"
#include "SPIFFS.h"
#include "spiffsHandler.h"
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>

#define FLOW_SENSOR 15

RTC_DS3231 rtc;

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP);
// void flushSetup();
// void flushLoop(void*);

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  // ----MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // ----SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occured while mounting SPIFFS");
    return;
  }

  // ----RTC
  // if (! rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   while (1);
  //   }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // ---Flow Sensor
  pulseCount = 0;
  flowRate = 0.0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  writeFile();
  // flushSetup();
}

void loop() {
  // ----MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

  
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor; // L/min
    previousMillis = millis();
    
  }

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // convert the value to a char array
    char flowString[6];
    dtostrf(flowRate, 3, 1, flowString);
    Serial.print("\nFlowrate: ");
    Serial.println(flowString);
    client.publish("waterbox/W0002/flow_sensor", flowString);


    // SPIFFS
    appendFile(flowString);
    Serial.println("___________________");
  }

  // ----RTC
  // DateTime RTCnow = rtc.now();
  // String second = String(RTCnow.second());
  // String minute = String(RTCnow.minute());
  // String hour = String(RTCnow.hour());
  // String day = String(RTCnow.day());
  // String month = String(RTCnow.month());
  // String year = String(RTCnow.year());
  
  // Serial.print(day);
  // Serial.print("-");
  // Serial.print(month);
  // Serial.print("-");
  // Serial.print(year);
  // Serial.print(" ");
  // Serial.print(hour);
  // Serial.print(":");
  // Serial.print(minute);
  // Serial.print(":");
  // Serial.println(second);
  
  delay(1000);
}

/*
// ---------------- IDEAS

void publishToMqtt(String);
bool isWifiConnected();
bool storedDataNotEmpty();
void flushStoredData();

void flushSetup() {
  xTaskCreate(flushLoop, "flushLoop", 1024, NULL, 1, NULL);
}

void publishToMqtt(String message) {
  if (isWifiConnected()) {
    // client.publish(message)
  } else {
    appendFile(message);
  }
}

void flushLoop(void* _params) {
  while(1) {
    if(isWifiConnected()) {
      if (storedDataNotEmpty()) {
        flush();
      }
    }
  }
}
*/