#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// Owns WiFi connection lifecycle. Exposes the underlying WiFiClient so
// MQTT (or any other TCP-based component) can use it.
class WifiManager
{
private:
    const char *_ssid;
    const char *_pass;
    WiFiClient _tcpClient;

    void _connectBlocking();

public:
    WifiManager(const char *ssid, const char *pass);
    void begin();
    void loop();
    bool isConnected();
    WiFiClient &client();
};

#endif
