#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

// Thin wrapper around PubSubClient. Reconnects automatically and (when
// configured) resubscribes to a single topic on each reconnect.
class MqttClient
{
private:
    PubSubClient _client;
    const char *_broker;
    int _port;
    const char *_clientId;
    const char *_topicToSubscribe;

    void _connectBlocking();

public:
    MqttClient(WiFiClient &tcp, const char *broker, int port, const char *clientId);

    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void subscribeOnConnect(const char *topic);
    void begin();
    void loop();
    bool isConnected();
    bool publish(const char *topic, const char *payload);
};

#endif
