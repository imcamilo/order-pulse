#include <Arduino.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "StatusIndicator.h"
#include "NetworkManager.h"

// Instancias de nuestras Clases
StatusIndicator indicator(Config::PIN_RGB_LED, Config::NUM_PIXELS);
NetworkManager network("OrderPulse-Client-3");

// El callback se queda aquí porque es Lógica de Negocio:
// Define QUÉ hacer con los datos recibidos.
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  JsonDocument doc;
  deserializeJson(doc, payload, length);

  const char *status = doc["status"] | "unknown";
  int orderId = doc["order"] | 0;

  Serial.printf(">>> Order #%d is now: %s\n", orderId, status);
  indicator.update(status);
}

void setup()
{
  Serial.begin(115200);

  indicator.begin();
  // Le pasamos nuestra función de callback al manager
  network.begin(mqttCallback);
}

void loop()
{
  // El manager se encarga de WiFi, MQTT y reconexiones automáticamente
  network.loop();
}