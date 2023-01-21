
#include <WiFi.h>
#include <PubSubClient.h>
#include "SPIFFS.h"
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>

#define FLOW_SENSOR 15

RTC_DS3231 rtc;

long currntMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long totalVolume;
long lastMsg = 0;
char msg[50];
int value = 0;

float flow = 0;

const int ledPinRED = 19;
const int ledPinGREEN = 13;

const char* ssid = "Buffriend";
const char* password = "123456789";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "161.97.179.79";

WiFiClient espClient;
PubSubClient client(espClient);

void SPIFFSreadFile() {
    Serial.println("\nReading file");

    File file = SPIFFS.open("/test.txt");
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    Serial.println("File Content:");
    while(file.available()){
        Serial.write(file.read());
    }
    Serial.println();
    file.close();
}

void SPIFFSwriteFile(){
  Serial.println("\nWriting file");
  
  File file = SPIFFS.open("/test.txt", FILE_WRITE);
  if(!file){
      Serial.println("Failed to open file for writing");
      return;
  }
  if(file.println("Temperature data")){
      Serial.println("File written");
  } 
  else {
      Serial.println("Write failed");
  }
  file.close();
}

void SPIFFSappendFile(const char * message){
  Serial.println("\nAppending to file");
  File file = SPIFFS.open("/test.txt", FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.println(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
  Serial.println("___________________");
}

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

void setupWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageFlow;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageFlow += (char)message[i];
  }
  Serial.println();
}

void reconnectMQTT() {
  // Loop until we're reconnected
  unsigned long currentMillis = millis();
  if(!client.connected() && (currentMillis - previousMillis >=interval)) {
    if (WiFi.status() != WL_CONNECTED){
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    Serial.print("\nAttempting MQTT connection...");
    // Attempt to connect
    if (client.connect("WaterboxClient")) {
      digitalWrite(ledPinRED, LOW);
      digitalWrite(ledPinGREEN, HIGH);
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
    previousMillis = currentMillis;
  }
}

void setup() {
  SPI.begin(23,22);
  Serial.begin(115200);
  // ----MQTT
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPinRED, OUTPUT);
  pinMode(ledPinGREEN, OUTPUT);

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
  flowMilliLitres = 0;
  totalMilliLitres = 0;

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  SPIFFSwriteFile();

}

void loop() {
  
  
  // ----MQTT

  if (!client.connected()) {
    digitalWrite(ledPinRED, HIGH);
    digitalWrite(ledPinGREEN, LOW);
    reconnectMQTT();
  }
  client.loop();

  currntMillis = millis();
  if (currntMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor; // L/min
    previousMillis = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    totalVolume = totalMilliLitres/1000;
  }

  long now = millis();
  if (now - lastMsg > 5000) {           // send MQTT message every 5 second
    lastMsg = now;
    
    // convert the value to a char array
    char flowString[6];
    dtostrf(flowRate, 3, 1, flowString);
    Serial.print("\nFlowrate: "); Serial.print(flowString); Serial.print("L/min"); Serial.print("\t");  
    client.publish("waterbox/W0002/flow_sensor/flowrate", flowString);

    char volumeString[6];
    dtostrf(totalVolume, 3, 1, volumeString);
    Serial.print("Total Volume: "); Serial.print(totalVolume); Serial.println("L");
    client.publish("waterbox/W0002/flow_sensor/total_volume", volumeString);



    // SPIFFS
    SPIFFSappendFile(flowString);
    
  }

  // ----RTC
  // DateTime RTCnow = rtc.now();
  // String second = String(RTCnow.second());
  // String minute = String(RTCnow.minute());
  // String hour = String(RTCnow.hour());
  // String day = String(RTCnow.day());
  // String month = String(RTCnow.month());
  // String year = String(RTCnow.year());
  
  // Serial.print(day); Serial.print("-"); Serial.print(month); Serial.print("-"); Serial.print(year);
  // Serial.print(" ");
  // Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.println(second);
}