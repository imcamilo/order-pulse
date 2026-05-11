#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config
{
    // Identity
    inline const int TABLE_ID = 2;

    // Networking
    inline const char *WIFI_SSID = "PruebaRobotica";
    inline const char *WIFI_PASS = "pruebarobotica";
    inline const char *MQTT_BROKER = "10.157.147.151";
    inline const int MQTT_PORT = 1883;

    // Topics
    // Note: String is an object, so inline is also required here
    inline const String TOPIC_STATUS = "orderpulse/table/" + String(TABLE_ID) + "/status";

    // Unique clientId per table (prevents two boards from kicking each other on the broker)
    inline const String MQTT_CLIENT_ID = "OrderPulse-Client-" + String(TABLE_ID);

    // Hardware Pins
    inline const int PIN_RGB_LED = 48;
    inline const int NUM_PIXELS = 1;
    inline const int PIN_BUZZER = 5;
    // Buzzer volume: duty cycle 0-255 (8-bit). ~25 = soft, 128 = max.
    inline const int BUZZER_VOLUME = 125;
}

#endif