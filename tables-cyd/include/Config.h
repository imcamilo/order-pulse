#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config
{
    // Identity
    inline const int TABLE_ID = 1;

    // Networking
    inline const char *WIFI_SSID    = "wifi-network";
    inline const char *WIFI_PASS    = "wifi-password";
    inline const char *MQTT_BROKER  = "127.0.0.1";
    inline const int   MQTT_PORT    = 1883;

    // Topics
    inline const String TOPIC_STATUS = "orderpulse/table/" + String(TABLE_ID) + "/status";

    // Unique clientId per table (prevents two boards from kicking each other on the broker)
    inline const String MQTT_CLIENT_ID = "OrderPulse-Client-" + String(TABLE_ID);

    // ===== CYD pins (ESP32-2432S028) =====
    // Onboard RGB LED (common-anode, LOW = ON)
    inline const int PIN_RGB_R = 4;
    inline const int PIN_RGB_G = 16;
    inline const int PIN_RGB_B = 17;

    // Buzzer / SPEAK pin
    inline const int PIN_BUZZER     = 26;
    inline const int BUZZER_VOLUME  = 45; // duty 0-255
}

#endif
