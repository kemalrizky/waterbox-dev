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

void MqttHandler::setup() {
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(MQTT_SERVER, 1883);
}

bool MqttHandler::connect() {
  Serial.print("Attempting MQTT connection...");

  if (mqttClient.connect(deviceIdGenerator.getId().c_str())) {
    Serial.println(" connected");
    // resubscribe to all topics
    // ...

    return true;
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 1 seconds");
    return false;
  }
}

void MqttHandler::subscribe(String _topic) {
  mqttClient.subscribe(_topic.c_str());
}

void MqttHandler::loop() { mqttClient.loop(); }

bool MqttHandler::publish(JsonDocument _data) {
  String _topic = "waterbox/" + deviceIdGenerator.getId();
  
  // Serialize JSON object to a temporary buffer
  char _payload[256];
  serializeJson(_data, _payload);

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
  MqttHandler* mqttHandler = (MqttHandler*) pv;

  while (1) {
    // Mantaining mqtt connection
    if (!mqttHandler->isConnected()) {
      ledHandler.turnOff(CONNECTION_LED_PINOUT);

      if (internetHandler.checkConnection() ==
          InternetStatusCode::DISCONNECTED) {
        if (internetHandler.connect()) {
          vTaskDelay(1000);
          if (mqttHandler->connect()) {
            vTaskDelay(1000);
            ledHandler.turnOn(CONNECTION_LED_PINOUT);
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
          vTaskDelay(1000);
          ledHandler.turnOn(CONNECTION_LED_PINOUT);
        } else {
          // log fail to connect to mqtt
          // ...
        }
      }
    }

    vTaskDelay(10000);
  }
}