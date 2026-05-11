# Order Pulse

MQTT-based order tracking: a touch panel changes order states and every
table receives the event on its own board (buzzer, LED, screen).

## Projects

| Folder | Role | Hardware |
|---|---|---|
| [`tables/`](./tables) | Receiver per table (buzzer + LED) | ESP32-S3 DevKitC-1 + passive buzzer |
| [`tables-cyd/`](./tables-cyd) | Receiver per table with screen | CYD ESP32-2432S028 |
| [`orchestrator/`](./orchestrator) | Touch panel that publishes states | CYD ESP32-2432S028 |

Each subfolder is an independent PlatformIO project.

## Before flashing

Edit each subproject's `include/Config.h`:

- **`WIFI_SSID`** and **`WIFI_PASS`** — your network.
- **`MQTT_BROKER`** — Mosquitto broker IP.
- **`TABLE_ID`** (only in `tables/` and `tables-cyd/`) — unique per board. Each board only receives events for its own `TABLE_ID`.

## Flash

```bash
cd tables/         # or tables-cyd/ or orchestrator/
pio run -t upload -t monitor
```

If you have multiple boards connected, pin the port in `platformio.ini`:

```ini
upload_port  = /dev/cu.usbserial-130
monitor_port = /dev/cu.usbserial-130
```

## MQTT contract

- **Topic:** `orderpulse/table/{TABLE_ID}/status`
- **Payload:**
  ```json
  {"order": 101, "status": "ready", "eta": "0 min"}
  ```
- **States:** `pending`, `preparing`, `ready`.
