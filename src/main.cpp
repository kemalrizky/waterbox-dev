#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <ESP2SOTA.h>
#include <Wire.h>
#include <Config.h>

WiFiClient espClient;
PubSubClient MQTTclient(espClient);
WebServer server(80);

long lastAttempt = 0;
long lastCount = 0;
long lastMsg = 0;
long lastPul = 0;
long lastIP = 0;

volatile byte pulseCount;
String pulse;

double flowRate = 0.0;
double totalVolume = 0.0;
double calibrationFactor = 0.117;                                                    // variable to calibrate
byte pulse1Sec = 0;



void setupWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("\nConnecting to ");
  Serial.println(MODEM_SSID);
  
  digitalWrite(LED_RED, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(MODEM_SSID, MODEM_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Need works!
  // while(!MDNS.begin("waterbox-0002")) {
  //   Serial.println("Starting mDNS...");
  //   delay(1000);
  // }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String message;

  String mqttInCalibratFactor = "waterbox/W0002/calibration_factor";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }
  
  calibrationFactor = message.toDouble();
  
  Serial.println();
  Serial.print("Calibration factor: ");
  Serial.println(calibrationFactor, 5);
}

void turnOnLED(bool MQTTconnected){
  if (MQTTconnected == true) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
  else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }
  
}

void reconnectMQTT() {
  // Loop until we're reconnected
  unsigned long now = millis();
  if(!MQTTclient.connected() && (now - lastAttempt >= 1000)) {
    turnOnLED(MQTTclient.connected());

    if (WiFi.status() != WL_CONNECTED){
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(MODEM_SSID, MODEM_PASS);
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    Serial.print("\nAttempting MQTT connection...");
    // Attempt to connect
    if (MQTTclient.connect("WaterboxClient")) {
      turnOnLED(MQTTclient.connected());
      Serial.println("connected");
      // Subscribe
      MQTTclient.subscribe("waterbox/W0002/calibration_factor");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 1 seconds");
    }
    lastAttempt = now;
  }
}

void setupOTA(){
  delay(1000);
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

    // Get Flowrate
    flowRate = ((1000.0 / (millis() - lastCount)) * pulse1Sec) * calibrationFactor;   // uncomment for flowrate measurement in L/min
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
    MQTTclient.publish("waterbox/W0002/flow_sensor/pulse", pulse.c_str());
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
  MQTTclient.setServer(MQTT_SERVER, 1883);
  MQTTclient.setCallback(callback);

  // OTA feature, MUST HAVE LINE!
  setupOTA();
  
  // Flowsensor
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
  
}

void loop() {
  reconnectMQTT();
  MQTTclient.loop();

  getFlowrate();
  
  unsigned long now = millis();
  if (now - lastMsg > 2000) {           // send MQTT message every 2 second
    
    // Send flowrate data
    char flowrateString[6];
    dtostrf(flowRate, 3, 2, flowrateString);                                  // convert the value to a char array
    MQTTclient.publish("waterbox/W0002/flow_sensor/flowrate", flowrateString);
    Serial.print("Flowrate : ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    // Send total volume data
    char volumeString[6];
    dtostrf(totalVolume, 3, 2, volumeString);                                 // convert the value to a char array
    MQTTclient.publish("waterbox/W0002/flow_sensor/total_volume", volumeString);
    Serial.print("Total Volume : ");
    Serial.print(totalVolume);
    Serial.println(" L");
    Serial.println();

    lastMsg = now;
  }

  // Calibration purpose
  // sendFlowSensorPulse();                // uncomment to send measured pulse per second by the flow sensor to the cloud
  
  // OTA feature, MUST HAVE LINE!
  server.handleClient();                // Handle update requests
  delay(5);
}
