#include "MqttHandler.h"
MqttHandler mqttHandler;

PubSubClient mqttClient;

const char *mqttServer = MQTT_SERVER;

MqttHandler::MqttHandler() {
    mqttHandler.device = "waterbox";
    mqttHandler.deviceId = waterbox.id;
    mqttHandler.sensor = "flow_sensor";
    mqttHandler.measurement = "undefined";
}

void MqttHandler::setup() {
    mqttClient.setClient(wifiClient);
    mqttClient.setServer(mqttServer, 1883);
    mqttClient.setCallback(callback);
}

void callback(char* _topic, byte* _payload, unsigned int _length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(_topic);
    Serial.print(". Message: ");
    String _message;
    
    for (int i = 0; i < _length; i++) {
        Serial.print((char)_payload[i]);
        _message += (char)_payload[i];
    }
    
    Serial.println();
}

char * MqttHandler::assignClientId() {
    String _clientIdStr = mqttHandler.deviceId;
    char* _clientIdChar = new char[_clientIdStr.length() + 1]; // +1 for the null terminator
    _clientIdStr.toCharArray(_clientIdChar, _clientIdStr.length() + 1);
    return _clientIdChar;
}

bool MqttHandler::connect() {
    Serial.print("\nAttempting MQTT connection...");

    char * clientId = assignClientId();
    
    if (mqttClient.connect(clientId)) {         
      Serial.println("connected");
      return true;
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 1 seconds");
      return false;
    }
}

void MqttHandler::mantainConnection() {
    if(!mqttClient.connected()) {
        if (!internetHandler.isConnected()) {
            internetHandler.connect();
        }
        
        mqttHandler.connect();
        
        // Subscribe
        // mqttHandler.subscribe("waterbox/W0002/calibration_factor");
    } 

}

void MqttHandler::loop() {
    mqttClient.loop();
}

void MqttHandler::subscribe(char *_topic) {
    mqttClient.subscribe(_topic);
}

char * MqttHandler::generateTopic(){
    String _topicString = device + "/" + deviceId + "/" + sensor + "/" + measurement;

    char* _topicChar = new char[_topicString.length() + 1]; // +1 for the null terminator
    _topicString.toCharArray(_topicChar, _topicString.length() + 1);

    return _topicChar;
}

bool MqttHandler::publishData(char *_topic, float _data) {
    char _charBuffer[7];                     // max 999.99
    dtostrf(_data, 3, 2, _charBuffer);  

    if (mqttClient.publish(_topic, _charBuffer)) {   
      Serial.println("\nData published.");
      return true;
    } 
    else {
      Serial.print("failed to publish, rc=");
      Serial.print(mqttClient.state());
      return false;
    }
}