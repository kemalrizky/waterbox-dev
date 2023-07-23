#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESP2SOTA.h>
#include <Wire.h>
#include <Config.h>

WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

long lastAttempt = 0;
long lastCount = 0;
long lastMsg = 0;
long lastPul = 0;

volatile byte pulseCount;

float flowRate = 0.0;
unsigned long totalLitres = 0.0;
float totalVolume = 0.0;
byte pulse1Sec = 0;

String pulse;

void setupWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("\nConnecting to ");
  Serial.println(MODEM_SSID);
  
  digitalWrite(LED_RED, HIGH);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(MODEM_SSID, MODEM_PASS);
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
      WiFi.begin(MODEM_SSID, MODEM_PASS);
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
  WiFi.softAP(ESP_SSID, ESP_PASS);
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

  ESP2SOTA.begin(&server);
  server.begin();
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

    float calibrationFactor = 6.9;                                                    // variable to calibrate
    // Get Flowrate
    flowRate = ((1000.0 / (millis() - lastCount)) * pulse1Sec) / calibrationFactor;   // uncomment for flowrate measurement in L/min
    // flowRate = flowRate / 60;                                                      // uncomment for flowrate measurement in L/s
    
    // Get Volume
    totalVolume += (flowRate / 60) ;                                                  // uncomment for volume measurement in Litres

    // Saving pulse per one second into string for calibration purpose  
    pulse = String(pulse1Sec);      
    
    lastCount = millis();
  }
}

void sendFlowSensorPulse(){
  // Calibration
  unsigned long now = millis();
  if (now - lastPul > 5000) {
    client.publish("waterbox/W0002/flow_sensor/pulse", pulse.c_str());
    lastPul = now;
  }
}

void setup() {
  Serial.begin(115200);
  // Defining LED pinout
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  // MQTT
  setupWiFi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  // OTA feature, MUST HAVE LINE!
  setupOTA();
  
  // Flowsensor
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  
}

void loop() {
  reconnectMQTT();
  client.loop();

  getFlowrate();
  
  unsigned long now = millis();
  if (now - lastMsg > 5000) {           // send MQTT message every 5 second
    
    // Send flowrate data
    char flowrateString[6];
    dtostrf(flowRate, 3, 2, flowrateString);                                  // convert the value to a char array
    client.publish("waterbox/W0002/flow_sensor/flowrate", flowrateString);
    Serial.print("Flowrate : ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    // Send total volume data
    char volumeString[6];
    dtostrf(totalVolume, 3, 2, volumeString);                                 // convert the value to a char array
    client.publish("waterbox/W0002/flow_sensor/total_volume", volumeString);
    Serial.print("Total Volume : ");
    Serial.print(totalVolume);
    Serial.println(" L");
    Serial.println();

    lastMsg = now;
  }

  // Calibration purpose
  sendFlowSensorPulse();                // uncomment to send measured pulse per second by the flow sensor to the cloud
  
  // OTA feature, MUST HAVE LINE!
  server.handleClient();                // Handle update requests
  delay(5);
}
