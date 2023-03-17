#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
/* INCLUDE ESP2SOTA LIBRARY */
#include <ESP2SOTA.h>
#include "SPIFFS.h"
#include <Wire.h>

#define FLOW_SENSOR 15
#define LED_RED 19
#define LED_GREEN 13
#define ROUTER_SSID "SSID"
#define ROUTER_PASS "Password"
#define MQTT_SERVER "MQTT Server"
#define AP_SSID "AP"
#define AP_PASS "Password"

WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

String pulse;

long lastAttempt = 0;
long lastCount = 0;
long lastMsg = 0;
long lastPul = 0;

volatile byte pulseCount;

float flowRate = 0.0;
unsigned long totalLitres = 0.0;
unsigned long totalVolume = 0.0;
byte pulse1Sec = 0;

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
  Serial.println(ROUTER_SSID);
  
  digitalWrite(LED_RED, HIGH);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ROUTER_SSID, ROUTER_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String message;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
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
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(ROUTER_SSID, ROUTER_PASS);
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

void setupOTA(){
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(1000);
  IPAddress IP = IPAddress (192, 168, 0, 199);
  IPAddress NMask = IPAddress (255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, NMask);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* SETUP YOR WEB OWN ENTRY POINTS */
  server.on("/myurl", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", "Hello there!");
  });
}

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

void getFlowrate(){
  unsigned long now = millis();
  if (now - lastCount > 1000) {
    pulse1Sec = 0;
    pulse1Sec = pulseCount;
    pulseCount = 0;

    float calibrationFactor = 6.1;
    // Get Flowrate
    flowRate = ((1000.0 / (millis() - lastCount)) * pulse1Sec) / calibrationFactor; // L/min
    // flowRate = flowRate / 60; // L/s

    pulse = String(pulse1Sec);
    
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
    dtostrf(_content, 3, 2, contentString);
    client.publish(_topic, contentString);

    if (_saveSPIFFS == true){
      spiffsAppendFile(contentString);
    }
    lastMsg = now;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  // ----MQTT
  setupWiFi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  // OTA feature, MUST HAVE LINE!
  setupOTA();
  /* INITIALIZE ESP2SOTA LIBRARY */
  ESP2SOTA.begin(&server);
  server.begin();

  // ----SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occured while mounting SPIFFS");
    return;
  }
  spiffsWriteFile("Waterbox v2");
  
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  
}

void loop() {
  reconnectMQTT();
  client.loop();

  getFlowrate();
  sendMQTTmsg("waterbox/W0002/flow_sensor/flowrate", flowRate, false);

  // Testing
  // unsigned long now = millis();
  // if (now - lastPul > 5000) {           // send MQTT message every 5 second
  //   client.publish("pulseOneSec", pulse.c_str());
  //   lastPul = now;
  // }
  
  // OTA feature, MUST HAVE LINE!
  server.handleClient(); // Handle update requests
  delay(5);
}
