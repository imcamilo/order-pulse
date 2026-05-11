#include <Arduino.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "StatusIndicator.h"
#include "NetworkManager.h"
#include "NotificationSound.h"

// Instances of our classes
StatusIndicator indicator(Config::PIN_RGB_LED, Config::NUM_PIXELS);
NetworkManager network(Config::MQTT_CLIENT_ID.c_str());
NotificationSound buzzer(Config::PIN_BUZZER, Config::BUZZER_VOLUME);

// Callback stays here because it's business logic:
// defines WHAT to do with the received data.
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  JsonDocument doc;
  deserializeJson(doc, payload, length);

  const char *status = doc["status"] | "unknown";
  int orderId = doc["order"] | 0;

  Serial.printf(">>> Order #%d is now: %s\n", orderId, status);
  indicator.update(status);

  if (strcmp(status, "ready") == 0)
    buzzer.playOrderReady();
}

void setup()
{
  Serial.begin(115200);

  indicator.begin();
  buzzer.begin();
  // Pass our callback function to the manager
  network.begin(mqttCallback);
}

void loop()
{
  // Manager handles WiFi, MQTT and reconnections automatically
  network.loop();
  buzzer.tick();
}