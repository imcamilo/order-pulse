# tables

Table-side firmware for **ESP32-S3 DevKitC-1** with onboard NeoPixel and a
passive piezo buzzer. Subscribes to its `TABLE_ID` topic and reacts when an
order status arrives.

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
                                                       └─────┬───────────┬───┘
                                                             │           │
                                                             ▼           ▼
                                                  ┌─────────────────┐ ┌──────────────────┐
                                                  │ StatusIndicator │ │ NotificationSound│
                                                  │ (HW: NeoPixel)  │ │  (HW: LEDC)      │
                                                  └────────┬────────┘ └────────┬─────────┘
                                                           │ uses              │ uses
                                                           ▼                   ▼
                                                  ┌─────────────────────────────────┐
                                                  │ StatusLogic (PURE)              │
                                                  │  rgbForStatus, readyMotif       │
                                                  └─────────────────────────────────┘
```

**Legend:** `(HW: ...)` = touches hardware. **PURE** = no Arduino/hardware deps, unit-testable in native env.

## Flow per MQTT message

```
broker → MqttClient → mqttCallback(payload)
                          │
                          ▼
                  PayloadParser::parse(payload) → OrderEvent
                          │
                          ▼
                  OrderEventHandler::handle(evt)
                          │
        ┌─────────────────┼─────────────────┐
        ▼                 ▼                 ▼
  Serial.printf    indicator.update(s) buzzer.playOrderReady() [only if status == "ready"]
                  └─► StatusLogic::      └─► StatusLogic::
                       rgbForStatus           readyMotif
```

## Files

| File | Role |
|---|---|
| `Config.h` | WiFi/MQTT/pins (edit before flashing) |
| `WifiManager.{h,cpp}` | WiFi connection + reconnect |
| `MqttClient.{h,cpp}` | MQTT subscribe + reconnect |
| `PayloadParser.{h,cpp}` | **PURE** JSON parsing |
| `OrderEventHandler.{h,cpp}` | Routes parsed event to peripherals |
| `StatusIndicator.{h,cpp}` | NeoPixel driver |
| `NotificationSound.h` | Non-blocking LEDC buzzer |
| `StatusLogic.{h,cpp}` | **PURE** status→color/motif mapping |
| `main.cpp` | Boot + wiring |
