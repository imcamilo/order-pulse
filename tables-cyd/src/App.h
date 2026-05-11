#ifndef APP_H
#define APP_H

#include <cstdint>
#include <cstddef>
#include <TFT_eSPI.h>
#include "Config.h"
#include "StatusIndicator.h"
#include "StatusDisplay.h"
#include "NotificationSound.h"
#include "WifiManager.h"
#include "MqttClient.h"
#include "OrderEventHandler.h"

// Application object that owns all hardware and networking components
// and wires them together. main.cpp is a thin shim that instantiates
// one App and forwards setup()/loop() calls.
class App
{
private:
    TFT_eSPI          _tft;
    StatusIndicator   _indicator;
    StatusDisplay     _display;
    NotificationSound _buzzer;
    WifiManager       _wifi;
    MqttClient        _mqtt;
    OrderEventHandler _handler;

public:
    App();
    void setup();
    void loop();
    void onMqttMessage(const uint8_t *payload, size_t length);
};

#endif
