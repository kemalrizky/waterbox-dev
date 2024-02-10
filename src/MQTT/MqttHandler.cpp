#include "MqttHandler.h"

const char* mqttServer = MQTT_SERVER;

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

void MqttHandler::init() {
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(MQTT_SERVER, 1883);
}

bool MqttHandler::connect() {
  if (mqttClient.connect(deviceIdGenerator.getId().c_str())) {
    Serial.println("Connected to MQTT server");
    // resubscribe to all topics
    // ...

    return true;
  } else {
    Serial.print("Connection to MQTT server failed, rc=");
    Serial.print(mqttClient.state());
    return false;
  }
}

void MqttHandler::subscribe(String _topic) {
  mqttClient.subscribe(_topic.c_str());
}

void MqttHandler::loop() { mqttClient.loop(); }

bool MqttHandler::publish(String _topic, String _data) {
  // Convert long to string
  String _payload = _data;

  if (mqttClient.publish(_topic.c_str(), _payload.c_str())) {
    Serial.println("Data published.");
    return true;
  } else {
    Serial.print("failed to publish, rc=");
    Serial.print(mqttClient.state());
    return false;
  }
}

bool MqttHandler::publish(String _topic, long _data) {
  // Convert long to string
  String _payload = String(_data);

  if (mqttClient.publish(_topic.c_str(), _payload.c_str())) {
    Serial.println("Data published.");
    return true;
  } else {
    Serial.print("failed to publish, rc=");
    Serial.print(mqttClient.state());
    return false;
  }
}

bool MqttHandler::publish(String _topic, float _data) {
  // Convert float to string
  char _payload[7];  // max 999.99
  dtostrf(_data, 3, 2, _payload);

  if (mqttClient.publish(_topic.c_str(), _payload)) {
    Serial.println("Data published.");
    return true;
  } else {
    Serial.print("failed to publish, rc=");
    Serial.print(mqttClient.state());
    return false;
  }
}

#define MAX_MQTT_PUBLISH_PAYLOAD_SIZE 256
bool MqttHandler::publish(String _topic, JsonDocument _data) {
  // do check, if JsonDocument converted size is larger than char[256], if so log error
  // ...

  char _payload[MAX_MQTT_PUBLISH_PAYLOAD_SIZE];
  serializeJson(_data,
                _payload);  // Serialize JSON object to a temporary buffer

  if (mqttClient.publish(_topic.c_str(), _payload)) {
    Serial.println("Data published.");
    return true;
  } else {
    Serial.print("Failed to publish, error code =");
    Serial.print(mqttClient.state());
    return false;
  }
}

bool MqttHandler::isConnected() {
  if (!mqttClient.connected()) {
    // Serial.println("Not connected to MQTT broker");
    return false;
  } else {
    // Serial.println("Connected to MQTT broker");
    return true;
  }
}

void MqttHandler::reconnectTask(void* pv) {
  MqttHandler* mqttHandler = (MqttHandler*)pv;

  while (1) {
    // Mantaining connection to MQTT Server
    if (!mqttHandler->isConnected()) {
      ledHandler.setInternetLedOnEvent(InternetStatusCode::DISCONNECTED);
      if (internetHandler.checkConnection() ==
          InternetStatusCode::DISCONNECTED) {
        if (internetHandler.connect()) {
          
          vTaskDelay(1000);
          if (mqttHandler->connect()) {
            ledHandler.setInternetLedOnEvent(InternetStatusCode::CONNECTED);
            vTaskDelay(1000);
          } else {
            // log fail to connect to mqtt
            // ...
          }
        } else {
          // log fail to connect to internet
          // ...
        }
      } else {
        if (mqttHandler->connect()) {
          ledHandler.setInternetLedOnEvent(InternetStatusCode::CONNECTED);
          vTaskDelay(1000);
        } else {
          // log fail to connect to mqtt
          // ...
        }
      }
    }

    vTaskDelay(10000);
  }
}