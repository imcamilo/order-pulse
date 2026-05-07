#include "NetworkManager.h"

NetworkManager::NetworkManager(const char *clientId)
    : _mqttClient(_espClient), _clientId(clientId) {}

void NetworkManager::begin(MQTT_CALLBACK_SIGNATURE)
{
    _mqttClient.setServer(Config::MQTT_BROKER, Config::MQTT_PORT);
    _mqttClient.setCallback(callback);
    connectWiFi();
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

void NetworkManager::loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!_mqttClient.connected())
    {
        Serial.print("[MQTT] Attempting connection... ");
        if (_mqttClient.connect(_clientId))
        {
            // Serial.println("Connected!");
            _mqttClient.subscribe(Config::TOPIC_STATUS.c_str());
        }
        else
        {
            Serial.printf("Failed, rc=%d. Try again in 5s\n", _mqttClient.state());
            delay(5000);
        }
    }
    _mqttClient.loop();
}

bool NetworkManager::isConnected()
{
    return _mqttClient.connected();
}