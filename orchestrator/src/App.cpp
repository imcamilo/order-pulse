#include <Arduino.h>
#include <ArduinoJson.h>
#include <cstring>
#include "App.h"

// Trampolines for C-style callbacks expected by PubSubClient and UIManager.
static App *gAppInstance = nullptr;

static void uiStatusSelectedTrampoline(int tableId, const char *status)
{
    if (gAppInstance != nullptr)
        gAppInstance->publishStatus(tableId, status);
}

App::App()
    : _tft(),
      _touch(),
      _ui(&_tft),
      _wifi(Config::WIFI_SSID, Config::WIFI_PASS),
      _mqtt(_wifi.client(), Config::MQTT_BROKER, Config::MQTT_PORT,
            Config::MQTT_CLIENT_ID) {}

void App::setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Order Pulse Control Panel ===");

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    _tft.init();
    _tft.setRotation(1);
    _tft.fillScreen(TFT_BLACK);
    _tft.setTextColor(TFT_WHITE);
    _tft.drawString("Booting...", 10, 10, 2);

    _touch.begin();

    if (!_wifi.begin())
        Serial.println("[App] WiFi not ready at boot, will retry");

    if (!_mqtt.begin())
        Serial.println("[App] MQTT not ready at boot, will retry");

    gAppInstance = this;
    _ui.begin(uiStatusSelectedTrampoline);
}

void App::loop()
{
    _wifi.loop();
    _mqtt.loop();
    _ui.tick();

    int x, y;
    if (_touch.readPoint(x, y))
        _ui.handleTouch(x, y);
}

void App::publishStatus(int tableId, const char *status)
{
    char topic[48];
    snprintf(topic, sizeof(topic), "orderpulse/table/%d/status", tableId);

    const char *eta;
    if (strcmp(status, "pending") == 0)
        eta = Config::ETA_PENDING;
    else if (strcmp(status, "preparing") == 0)
        eta = Config::ETA_PREPARING;
    else
        eta = Config::ETA_READY;

    JsonDocument doc;
    doc["order"]  = Config::DEFAULT_ORDER_ID;
    doc["status"] = status;
    doc["eta"]    = eta;

    char payload[128];
    serializeJson(doc, payload, sizeof(payload));

    if (!_mqtt.publish(topic, payload))
    {
        Serial.printf("[App] publish FAILED: %s\n", topic);
        return;
    }
    Serial.printf("[App] %s : %s\n", topic, payload);
}
