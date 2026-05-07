#include "NetworkManager.h"

NetworkManager::NetworkManager(const char *clientId)
    : _mqttClient(_espClient), _clientId(clientId) {}

void NetworkManager::begin()
{
    _mqttClient.setServer(Config::MQTT_BROKER, Config::MQTT_PORT);
    connectWiFi();
    connectMqtt();
}

void NetworkManager::connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    Serial.printf("\n[WiFi] Connecting to %s...", Config::WIFI_SSID);
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}

void NetworkManager::connectMqtt()
{
    while (!_mqttClient.connected())
    {
        Serial.print("[MQTT] Connecting... ");
        if (_mqttClient.connect(_clientId))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.printf("failed rc=%d, retry in 2s\n", _mqttClient.state());
            delay(2000);
        }
    }
}

void NetworkManager::loop()
{
    if (WiFi.status() != WL_CONNECTED)
        connectWiFi();

    if (!_mqttClient.connected())
        connectMqtt();

    _mqttClient.loop();
}

bool NetworkManager::publish(const char *topic, const char *payload)
{
    return _mqttClient.publish(topic, payload);
}

bool NetworkManager::isConnected()
{
    return _mqttClient.connected();
}
