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
  Serial.print("\nAttempting MQTT connection...");

  if (mqttClient.connect("W0201918")) {
    Serial.println("connected");
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

bool MqttHandler::publish(String _topic, float _data) {
  char _payload[7];  // max 999.99
  dtostrf(_data, 3, 2, _payload);

  if (mqttClient.publish(_topic.c_str(), _payload)) {
    Serial.println("\nData published.");
    return true;
  } else {
    Serial.print("failed to publish, rc=");
    Serial.print(mqttClient.state());
    return false;
  }
}

bool MqttHandler::isConnected() {
  if (!mqttClient.connected()) {
    Serial.println("Not connected to MQTT broker");
    return false;
  } else {
    Serial.println("Connected to MQTT broker");
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
          if (mqttHandler->connect()) {
            ledHandler.turnOn(CONNECTION_LED_PINOUT);
          }
        }
      } else {
        if (mqttHandler->connect()) {
          ledHandler.turnOn(CONNECTION_LED_PINOUT);
        }
      }
    }

    vTaskDelay(1000);
  }
}