# orchestrator

Touch control panel firmware for the **CYD (ESP32-2432S028)**. Lets the user
pick a table from a grid and pick a status (PENDIENTE / PREPARANDO / LISTO);
each tap publishes the change to that table's MQTT topic.

## Component diagram

```
                          ┌──────────────┐
                          │   main.cpp   │  (instantiates & wires)
                          └──────┬───────┘
                                 │
            ┌────────────────────┼─────────────────┬─────────────────┐
            │                    │                 │                 │
            ▼                    ▼                 ▼                 ▼
     ┌──────────────┐    ┌──────────────┐   ┌─────────────┐   ┌────────────┐
     │ WifiManager  │    │  MqttClient  │   │   Touch     │   │ UIManager  │
     │ (HW: WiFi)   │◄──►│ (HW: PubSub) │   │(HW: XPT2046)│   │(HW:TFT_eSPI)│
     └──────────────┘    └──────▲───────┘   └──────┬──────┘   └─────┬──────┘
                                │                  │ (x,y)          │
                                │ publish          ▼                ▼
                                │           ┌──────────────────────────────┐
                                │           │ TouchLogic  (PURE)           │
                                │           │  hitTest, resolveTouch,      │
                                │           │  button geometry, Action enum│
                                │           └──────────────────────────────┘
                                │
                                │ ◄── onStatusSelected(tableId, status)
                                │
                          ┌─────┴──────┐
                          │  main.cpp  │  builds JSON payload + topic
                          │  callback  │
                          └────────────┘

                          (Labels: PURE — Spanish strings used by UIManager)
```

**Legend:** `(HW: ...)` = touches hardware. **PURE** = no Arduino/hardware deps, unit-testable in native env.

## Flow per touch

```
XPT2046 touch panel
        │ (raw x,y)
        ▼
   Touch::readPoint  ──►  screen-mapped (x,y)
                              │
                              ▼
        ui.handleTouch(x,y)
                              │
                              ▼
        TouchLogic::resolveTouch(_screen, x, y) → Action
                              │
        ┌─────────────────────┼─────────────────────────┐
        ▼                     ▼                         ▼
  SELECT_TABLE           GO_BACK                  SELECT_STATUS
   draw detail          draw list             onStatusSelected(table, status)
                                                    │
                                                    ▼
                                     build topic + JSON payload
                                                    │
                                                    ▼
                                         mqtt.publish(topic, json)
                                                    │
                                                    ▼
                                           draw "ENVIADO" overlay
                                                    │
                                                    ▼  (1s later, in ui.tick())
                                          back to table list
```

## Files

| File | Role |
|---|---|
| `Config.h` | WiFi/MQTT/touch pins/table names (edit before flashing) |
| `WifiManager.{h,cpp}` | WiFi connection + reconnect |
| `MqttClient.{h,cpp}` | MQTT publish + reconnect |
| `Touch.{h,cpp}` | XPT2046 wrapper + calibration |
| `UIManager.{h,cpp}` | TFT drawing + state transitions |
| `TouchLogic.{h,cpp}` | **PURE** hit-test, resolve touch → Action, button geometry |
| `Labels.h` | **PURE** Spanish UI strings (one place to edit) |
| `main.cpp` | Boot + wiring + MQTT publish on UI event |
