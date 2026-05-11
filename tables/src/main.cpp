#include <Arduino.h>
#include "Config.h"
#include "StatusIndicator.h"
#include "NotificationSound.h"
#include "WifiManager.h"
#include "MqttClient.h"
#include "PayloadParser.h"
#include "OrderEventHandler.h"

// Hardware instances
StatusIndicator    indicator(Config::PIN_RGB_LED, Config::NUM_PIXELS);
NotificationSound  buzzer(Config::PIN_BUZZER, Config::BUZZER_VOLUME);

// Networking instances
WifiManager wifi(Config::WIFI_SSID, Config::WIFI_PASS);
MqttClient  mqtt(wifi.client(), Config::MQTT_BROKER, Config::MQTT_PORT,
                 Config::MQTT_CLIENT_ID.c_str());

// Event pipeline (pure parser -> hardware-aware handler)
OrderEventHandler eventHandler(&indicator, &buzzer);

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    OrderEvent evt;
    if (PayloadParser::parse(payload, length, evt))
        eventHandler.handle(evt);
}

void setup()
{
    Serial.begin(115200);

    indicator.begin();
    buzzer.begin();

    wifi.begin();
    mqtt.setCallback(mqttCallback);
    mqtt.subscribeOnConnect(Config::TOPIC_STATUS.c_str());
    mqtt.begin();
}

void loop()
{
    wifi.loop();
    mqtt.loop();
    buzzer.tick();
}
