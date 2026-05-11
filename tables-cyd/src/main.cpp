#include <Arduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "Config.h"
#include "NetworkManager.h"
#include "StatusIndicator.h"
#include "StatusDisplay.h"
#include "NotificationSound.h"

// Instances
TFT_eSPI tft;
NetworkManager   network(Config::MQTT_CLIENT_ID.c_str());
StatusIndicator  indicator(Config::PIN_RGB_R, Config::PIN_RGB_G, Config::PIN_RGB_B);
StatusDisplay    display(&tft, Config::TABLE_ID);
NotificationSound buzzer(Config::PIN_BUZZER, Config::BUZZER_VOLUME);

// MQTT callback: parses the JSON and triggers display + LED + buzzer.
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    JsonDocument doc;
    deserializeJson(doc, payload, length);

    const char *status = doc["status"] | "unknown";
    int orderId       = doc["order"]  | 0;
    const char *eta   = doc["eta"]    | "-";

    Serial.printf(">>> Order #%d -> %s (eta %s)\n", orderId, status, eta);

    display.update(status, orderId, eta);
    indicator.update(status);
    buzzer.playForStatus(status);
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

    network.begin(mqttCallback);
}

void loop()
{
    network.loop();
    buzzer.tick();
}
