#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Config.h"
#include "StatusIndicator.h"
#include "StatusDisplay.h"
#include "NotificationSound.h"
#include "WifiManager.h"
#include "MqttClient.h"
#include "PayloadParser.h"
#include "OrderEventHandler.h"

// Hardware instances
TFT_eSPI          tft;
StatusIndicator   indicator(Config::PIN_RGB_R, Config::PIN_RGB_G, Config::PIN_RGB_B);
StatusDisplay     display(&tft, Config::TABLE_ID);
NotificationSound buzzer(Config::PIN_BUZZER, Config::BUZZER_VOLUME);

// Networking instances
WifiManager wifi(Config::WIFI_SSID, Config::WIFI_PASS);
MqttClient  mqtt(wifi.client(), Config::MQTT_BROKER, Config::MQTT_PORT,
                 Config::MQTT_CLIENT_ID.c_str());

// Event pipeline (pure parser -> hardware-aware handler)
OrderEventHandler eventHandler(&indicator, &display, &buzzer);

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    OrderEvent evt;
    if (PayloadParser::parse(payload, length, evt))
        eventHandler.handle(evt);
}

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Order Pulse Table (CYD) ===");
    Serial.printf("Table #%d\n", Config::TABLE_ID);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    tft.init();
    tft.setRotation(1);

    indicator.begin();
    buzzer.begin();
    display.begin();

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
