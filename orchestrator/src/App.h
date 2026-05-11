#ifndef APP_H
#define APP_H

#include <TFT_eSPI.h>
#include "Config.h"
#include "WifiManager.h"
#include "MqttClient.h"
#include "Touch.h"
#include "UIManager.h"

// Application object that owns the orchestrator's hardware and networking
// components. main.cpp is a thin shim that instantiates one App and
// forwards setup()/loop() calls.
class App
{
private:
    TFT_eSPI    _tft;
    Touch       _touch;
    UIManager   _ui;
    WifiManager _wifi;
    MqttClient  _mqtt;

public:
    App();
    void setup();
    void loop();
    // Called when the UI signals the user picked a status. Public so
    // tests can drive it directly.
    void publishStatus(int tableId, const char *status);
};

#endif
