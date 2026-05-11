#include <Arduino.h>
#include "MqttClient.h"

MqttClient::MqttClient(WiFiClient &tcp, const char *broker, int port, const char *clientId)
    : _client(tcp), _broker(broker), _port(port), _clientId(clientId),
      _topicToSubscribe(nullptr) {}

void MqttClient::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    _client.setCallback(callback);
}

void MqttClient::subscribeOnConnect(const char *topic)
{
    _topicToSubscribe = topic;
}

bool MqttClient::begin()
{
    _client.setServer(_broker, _port);
    return _attemptConnect();
}

void MqttClient::loop()
{
    if (!_client.connected())
    {
        const uint32_t now = millis();
        if (now - _lastReconnectAttempt >= RECONNECT_INTERVAL_MS)
        {
            _lastReconnectAttempt = now;
            _attemptConnect();
        }
    }
    _client.loop();
}

bool MqttClient::isConnected()
{
    return _client.connected();
}

bool MqttClient::publish(const char *topic, const char *payload)
{
    return _client.publish(topic, payload);
}

bool MqttClient::_attemptConnect()
{
    Serial.print("[MQTT] Connecting... ");
    if (_client.connect(_clientId))
    {
        Serial.println("ok");
        if (_topicToSubscribe != nullptr)
        {
            _client.subscribe(_topicToSubscribe);
            Serial.printf("[MQTT] subscribed to %s\n", _topicToSubscribe);
        }
        return true;
    }
    Serial.printf("failed rc=%d\n", _client.state());
    return false;
}
