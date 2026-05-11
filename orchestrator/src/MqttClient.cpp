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

void MqttClient::begin()
{
    _client.setServer(_broker, _port);
    _connectBlocking();
}

void MqttClient::loop()
{
    if (!_client.connected())
        _connectBlocking();
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

void MqttClient::_connectBlocking()
{
    while (!_client.connected())
    {
        Serial.print("[MQTT] connecting... ");
        if (_client.connect(_clientId))
        {
            Serial.println("ok");
            if (_topicToSubscribe != nullptr)
            {
                _client.subscribe(_topicToSubscribe);
                Serial.printf("[MQTT] subscribed to %s\n", _topicToSubscribe);
            }
        }
        else
        {
            Serial.printf("failed rc=%d, retry in 2s\n", _client.state());
            delay(2000);
        }
    }
}
