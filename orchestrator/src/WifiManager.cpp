#include <Arduino.h>
#include "WifiManager.h"

WifiManager::WifiManager(const char *ssid, const char *pass)
    : _ssid(ssid), _pass(pass) {}

void WifiManager::begin()
{
    _connectBlocking();
}

void WifiManager::loop()
{
    if (WiFi.status() != WL_CONNECTED)
        _connectBlocking();
}

bool WifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

WiFiClient &WifiManager::client()
{
    return _tcpClient;
}

void WifiManager::_connectBlocking()
{
    Serial.printf("\n[WiFi] Connecting to %s...", _ssid);
    WiFi.begin(_ssid, _pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}
