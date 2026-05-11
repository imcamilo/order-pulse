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

public:
    NetworkManager(const char *clientId);
    void begin(MQTT_CALLBACK_SIGNATURE); // Receives the callback function from outside
    void loop();
    bool isConnected();
};

#endif