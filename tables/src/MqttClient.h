#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

// Thin wrapper around PubSubClient. begin() attempts a single connection
// and returns the result. loop() retries non-blocking on disconnect.
// When configured via subscribeOnConnect(), resubscribes on each reconnect.
class MqttClient
{
public:
    static constexpr uint32_t RECONNECT_INTERVAL_MS = 2000;

private:
    PubSubClient _client;
    const char *_broker;
    int _port;
    const char *_clientId;
    const char *_topicToSubscribe;
    uint32_t _lastReconnectAttempt = 0;

    bool _attemptConnect();

public:
    MqttClient(WiFiClient &tcp, const char *broker, int port, const char *clientId);

    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void subscribeOnConnect(const char *topic);
    // Returns true if connected to the broker on the first attempt.
    bool begin();
    // Non-blocking. Retries connection at RECONNECT_INTERVAL_MS cadence.
    void loop();
    bool isConnected();
    bool publish(const char *topic, const char *payload);
};

#endif
