#include <Arduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "Config.h"
#include "NetworkManager.h"
#include "Touch.h"
#include "UIManager.h"

// Instances of our classes
TFT_eSPI tft;
NetworkManager network(Config::MQTT_CLIENT_ID);
Touch touch;
UIManager ui(&tft);

// Callback fired by the UI when a status is selected.
// Defines WHAT to do (publish the matching topic with JSON payload).
void onStatusSelected(int tableId, const char *status)
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

    if (network.publish(topic, payload))
        Serial.printf(">>> %s : %s\n", topic, payload);
    else
        Serial.printf(">>> publish FAILED: %s\n", topic);
}

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Order Pulse Control Panel ===");

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Booting...", 10, 10, 2);

    touch.begin();
    network.begin();
    ui.begin(onStatusSelected);
}

void loop()
{
    network.loop();
    ui.tick();

    int x, y;
    if (touch.readPoint(x, y))
        ui.handleTouch(x, y);
}
