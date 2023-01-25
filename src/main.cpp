
#include <WiFi.h>
#include <PubSubClient.h>
#include "SPIFFS.h"
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>

#define FLOW_SENSOR 15
#define LED_RED 19
#define LED_GREEN 13
#define SSID "Buffriend"
#define PASSWORD "123456789"
#define MQTT_SERVER "161.97.179.79"

WiFiClient espClient;
PubSubClient client(espClient);
RTC_DS3231 rtc;

long lastAttempt = 0;
long lastCount = 0;
long lastMsg = 0;

volatile byte pulseCount;

float flowRate = 0.0;
unsigned long totalLitres = 0.0;
unsigned long totalVolume = 0.0;

void spiffsReadFile() {
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
    file.close();
}

void spiffsWriteFile(const char * _content){
  Serial.println("Writing file");
  
  File file = SPIFFS.open("/test.txt", FILE_WRITE);
  if(!file){
      Serial.println("Failed to open file for writing");
      return;
  }
  if(file.println(_content)){
      Serial.println("File written");
  } 
  else {
      Serial.println("Write failed");
  }
  file.close();
}

void spiffsAppendFile(const char * _content){
  Serial.println("Appending to file");
  File file = SPIFFS.open("/test.txt", FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.println(_content)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void setupWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("\nConnecting to ");
  Serial.println(SSID);
  
  digitalWrite(LED_RED, HIGH);
  
  WiFi.begin(SSID, PASSWORD);
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
  unsigned long now = millis();
  if(!client.connected() && (now - lastAttempt >= 5000)) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);

    if (WiFi.status() != WL_CONNECTED){
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.begin(SSID, PASSWORD);
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    Serial.print("\nAttempting MQTT connection...");
    // Attempt to connect
    if (client.connect("WaterboxClient")) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
    lastAttempt = now;
  }
}

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

void getFlowrate(){
  unsigned long now = millis();
  if (now - lastCount > 1000) {
    byte pulse1Sec = 0;
    pulse1Sec = pulseCount;
    pulseCount = 0;

    float calibrationFactor = 4.5;
    // Get Flowrate
    flowRate = ((1000.0 / (millis() - lastCount)) * pulse1Sec) / calibrationFactor; // L/min
    
    // Get Volume
    // unsigned int flowMilliLitres;        // Divide the flow rate in litres/minute by 60 to determine how many litres have passed through 
    // flowLitres = (flowRate / 60);        // the sensor in this 1 second interval
    // totalVolume += flowLitres;           
    
    lastCount = millis();
  }
}

void sendMQTTmsg(const char * _topic, float _content, bool _saveSPIFFS = true){
  unsigned long now = millis();
  if (now - lastMsg > 5000) {           // send MQTT message every 5 second
    // convert the value to a char array
    char contentString[6];
    dtostrf(_content, 3, 1, contentString);
    // Serial.print("\nFlowrate: "); Serial.print(flowString); Serial.print("L/min"); Serial.print("\t");  
    client.publish(_topic, contentString);

    if (_saveSPIFFS == true){
      spiffsAppendFile(contentString);
    }
    lastMsg = now;
  }
}

void setup() {
  SPI.begin(23,22);
  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  // ----MQTT
  setupWiFi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);


  // ----SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occured while mounting SPIFFS");
    return;
  }
  spiffsWriteFile("Waterbox v2");

  // ----RTC
  // if (! rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   while (1);
  //   }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  
}

void loop() {
  reconnectMQTT();
  client.loop();

  getFlowrate();
  sendMQTTmsg("waterbox/W0002/flow_sensor/flowrate", flowRate);
  

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