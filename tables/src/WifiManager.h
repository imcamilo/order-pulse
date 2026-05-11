#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// Owns WiFi connection lifecycle. begin() attempts a single bounded
// connection. loop() retries periodically (non-blocking) if the link drops.
// Exposes the underlying WiFiClient so MQTT can use it.
class WifiManager
{
public:
    static constexpr uint32_t CONNECT_TIMEOUT_MS    = 15000;
    static constexpr uint32_t RECONNECT_INTERVAL_MS = 5000;

private:
    const char *_ssid;
    const char *_pass;
    WiFiClient _tcpClient;
    uint32_t _lastReconnectAttempt = 0;

    bool _attemptConnect();

public:
    WifiManager(const char *ssid, const char *pass);
    // Returns true if connected before the timeout.
    bool begin();
    // Non-blocking. Retries connection at RECONNECT_INTERVAL_MS cadence.
    void loop();
    bool isConnected();
    WiFiClient &client();
};

#endif
