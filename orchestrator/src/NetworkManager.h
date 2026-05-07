#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"

class NetworkManager
{
private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;
    const char *_clientId;

    void connectWiFi();
    void connectMqtt();

public:
    NetworkManager(const char *clientId);
    void begin();
    void loop();
    bool publish(const char *topic, const char *payload);
    bool isConnected();
};

#endif
