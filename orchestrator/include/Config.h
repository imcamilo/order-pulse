#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config
{
    // Networking
    inline const char *WIFI_SSID = "PruebaRobotica";
    inline const char *WIFI_PASS = "pruebarobotica";
    inline const char *MQTT_BROKER = "10.157.147.151";
    inline const int   MQTT_PORT    = 1883;

    // Identidad MQTT del panel de control.   
    inline const char *MQTT_CLIENT_ID = "OrderPulse-Control";

    // Mesas
    static constexpr int NUM_TABLES = 4;
    inline const char *TABLE_NAMES[NUM_TABLES] = {
        "MESA 1", "MESA 2", "MESA 3", "MESA 4"
    };

    // Topic publicado al cambiar estado:
    //   orderpulse/table/{TABLE_ID}/status
    // Se construye dinámicamente en main.cpp.

    // Order y ETA hardcodeados (por ahora)
    inline const int   DEFAULT_ORDER_ID  = 101;
    inline const char *ETA_PENDING       = "15 min";
    inline const char *ETA_PREPARING     = "5 min";
    inline const char *ETA_READY         = "0 min";

    // ===== Pines CYD (ESP32-2432S028) =====
    // TFT (configurado vía build_flags en platformio.ini)
    // Touch XPT2046 - bus SPI separado
    inline const int PIN_TOUCH_CS   = 33;
    inline const int PIN_TOUCH_IRQ  = 36;
    inline const int PIN_TOUCH_CLK  = 25;
    inline const int PIN_TOUCH_MISO = 39;
    inline const int PIN_TOUCH_MOSI = 32;
}

#endif
