#include <Arduino.h>
#include "WifiManager.h"

WifiManager::WifiManager(const char *ssid, const char *pass)
    : _ssid(ssid), _pass(pass) {}

bool WifiManager::begin()
{
    return _attemptConnect();
}

void WifiManager::loop()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    const uint32_t now = millis();
    if (now - _lastReconnectAttempt < RECONNECT_INTERVAL_MS)
        return;

    _lastReconnectAttempt = now;
    _attemptConnect();
}

bool WifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

WiFiClient &WifiManager::client()
{
    return _tcpClient;
}

bool WifiManager::_attemptConnect()
{
    Serial.printf("[WiFi] Connecting to %s\n", _ssid);
    WiFi.begin(_ssid, _pass);

    const uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start >= CONNECT_TIMEOUT_MS)
        {
            Serial.println("[WiFi] timeout");
            return false;
        }
        delay(100);
    }
    Serial.printf("[WiFi] OK ip=%s\n", WiFi.localIP().toString().c_str());
    return true;
}
