#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config
{
    // Identity
    inline const int TABLE_ID = 3;

    // Networking
    inline const char *WIFI_SSID = "wifi-network";
    inline const char *WIFI_PASS = "wifi-password";
    inline const char *MQTT_BROKER = "127.0.0.1";
    inline const int MQTT_PORT = 1883;

    // Topics
    // Nota: Como String es un objeto, inline también es obligatorio aquí
    inline const String TOPIC_STATUS = "orderpulse/table/" + String(TABLE_ID) + "/status";

    // Hardware Pins
    inline const int PIN_RGB_LED = 48;
    inline const int NUM_PIXELS = 1;
}

#endif