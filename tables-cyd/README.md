# tables-cyd

Table-side firmware for the **CYD (ESP32-2432S028)**: TFT display + discrete
RGB LED + speaker. Subscribes to its `TABLE_ID` topic and reacts with
screen, LED and sound.

## Component diagram

```
                          ┌──────────────┐
                          │   main.cpp   │  (instantiates & wires)
                          └──────┬───────┘
                                 │
        ┌────────────────────────┼────────────────────────────────┐
        │                        │                                │
        ▼                        ▼                                ▼
 ┌──────────────┐        ┌──────────────┐               ┌─────────────────┐
 │ WifiManager  │        │  MqttClient  │  callback ──► │ PayloadParser   │  PURE
 │   (HW: WiFi) │◄──────►│ (HW: PubSub) │               │  (JSON → struct)│
 └──────────────┘        └──────────────┘               └────────┬────────┘
                                                                 │ OrderEvent
                                                                 ▼
                                                       ┌─────────────────────┐
                                                       │ OrderEventHandler   │
                                                       │ (dispatch glue)     │
                                                       └─────┬─────┬─────┬───┘
                                                             │     │     │
                                                             ▼     ▼     ▼
                                          ┌─────────────────┐ ┌─────────────────┐ ┌──────────────────┐
                                          │ StatusIndicator │ │ StatusDisplay   │ │ NotificationSound│
                                          │  (HW: GPIO RGB) │ │ (HW: TFT_eSPI)  │ │  (HW: LEDC)      │
                                          └────────┬────────┘ └────────┬────────┘ └─────────┬────────┘
                                                   │ uses              │ uses              │ uses
                                                   └──────────┬────────┴─────────┬─────────┘
                                                              ▼                  ▼
                                                  ┌────────────────────┐  ┌─────────────────┐
                                                  │ StatusLogic (PURE) │  │   Labels  (PURE)│
                                                  │ rgbForStatus,      │  │ Spanish UI text │
                                                  │ visualForStatus,   │  │ (single source) │
                                                  │ sequenceForStatus  │  └─────────────────┘
                                                  └────────────────────┘
```

**Legend:** `(HW: ...)` = touches hardware. **PURE** = no Arduino/hardware deps, unit-testable in native env.

## Flow per MQTT message

```
broker → MqttClient → mqttCallback(payload)
                          │
                          ▼
                  PayloadParser::parse → OrderEvent
                          │
                          ▼
                  OrderEventHandler::handle(evt)
                          │
        ┌─────────────────┼─────────────────┐
        ▼                 ▼                 ▼
  display.update(...)  indicator.update(s)  buzzer.playForStatus(s)
       │                    │                     │
       │ StatusLogic::      │ StatusLogic::       │ StatusLogic::
       │ visualForStatus    │ rgbForStatus        │ sequenceForStatus
       └─► Labels::*        └─► RGB pins           └─► LEDC tones
```

## Files

| File | Role |
|---|---|
| `Config.h` | WiFi/MQTT/pins/TABLE_ID (edit before flashing) |
| `WifiManager.{h,cpp}` | WiFi connection + reconnect |
| `MqttClient.{h,cpp}` | MQTT subscribe + reconnect |
| `PayloadParser.{h,cpp}` | **PURE** JSON parsing |
| `OrderEventHandler.{h,cpp}` | Routes parsed event to all peripherals |
| `StatusIndicator.{h,cpp}` | Discrete RGB LED driver |
| `StatusDisplay.{h,cpp}` | TFT renderer |
| `NotificationSound.h` | Non-blocking LEDC buzzer |
| `StatusLogic.{h,cpp}` | **PURE** status→color/visual/notes |
| `Labels.h` | **PURE** Spanish UI strings (one place to edit) |
| `main.cpp` | Boot + wiring |
