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
    // Nota: Como String es un objeto, inline también es obligatorio aquí
    inline const String TOPIC_STATUS = "orderpulse/table/" + String(TABLE_ID) + "/status";

    // ClientId único por mesa (evita que dos placas se pateen en el broker)
    inline const String MQTT_CLIENT_ID = "OrderPulse-Client-" + String(TABLE_ID);

    // Hardware Pins
    inline const int PIN_RGB_LED = 48;
    inline const int NUM_PIXELS = 1;
    inline const int PIN_BUZZER = 5;
    // Volumen del buzzer: duty cycle 0-255 (8-bit). ~25 = suave, 128 = máximo.
    inline const int BUZZER_VOLUME = 125;
}

#endif