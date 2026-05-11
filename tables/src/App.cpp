#include <Arduino.h>
#include "App.h"
#include "PayloadParser.h"

// MQTT callback uses a C-style function signature, so we trampoline through
// a singleton instance to deliver the message to the App method.
static App *gAppInstance = nullptr;

static void mqttTrampoline(char *topic, byte *payload, unsigned int length)
{
    if (gAppInstance != nullptr)
        gAppInstance->onMqttMessage(payload, length);
}

App::App()
    : _indicator(Config::PIN_RGB_LED, Config::NUM_PIXELS),
      _buzzer(Config::PIN_BUZZER, Config::BUZZER_VOLUME),
      _wifi(Config::WIFI_SSID, Config::WIFI_PASS),
      _mqtt(_wifi.client(), Config::MQTT_BROKER, Config::MQTT_PORT,
            Config::MQTT_CLIENT_ID.c_str()),
      _handler(&_indicator, &_buzzer) {}

void App::setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Order Pulse Table ===");

    _indicator.begin();
    _buzzer.begin();

    if (!_wifi.begin())
        Serial.println("[App] WiFi not ready at boot, will retry");

    gAppInstance = this;
    _mqtt.setCallback(mqttTrampoline);
    _mqtt.subscribeOnConnect(Config::TOPIC_STATUS.c_str());

    if (!_mqtt.begin())
        Serial.println("[App] MQTT not ready at boot, will retry");
}

void App::loop()
{
    _wifi.loop();
    _mqtt.loop();
    _buzzer.tick();
}

void App::onMqttMessage(const uint8_t *payload, size_t length)
{
    OrderEvent evt;
    if (!PayloadParser::parse(payload, length, evt))
    {
        Serial.println("[App] bad payload, ignoring");
        return;
    }
    _handler.handle(evt);
}
