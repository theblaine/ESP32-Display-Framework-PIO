# Home Dashboard MQTT Protocol

## Purpose

This document defines the MQTT topics and JSON payloads used by the ESP32 Home Dashboard application.

The MQTT broker transports dashboard data. The ESP32 application owns the screen layout and rendering. Publishers such as Home Assistant, MQTT.fx, scripts, or other systems only send data updates.

## Broker Configuration

Current development broker:

```text
Host: 10.0.0.50
Port: 1883
Authentication: None
```

Current ESP32 MQTT client ID:

```text
waveshare-home-dashboard
```

These values are environment-specific configuration and may change later.

## General Message Rules

- Payloads must be valid JSON objects.
- Field names are case-sensitive.
- Values are currently sent as JSON strings.
- Publishers may send a complete page snapshot or only the fields that changed.
- Fields omitted from a message retain their previously stored values on the ESP32.
- Malformed JSON is rejected and logged.
- Use retained MQTT messages for persistent state data so the ESP32 receives the latest values after reconnecting.

Example partial update:

```json
{
  "aircraft": "34"
}
```

Example complete update:

```json
{
  "feed": "Online",
  "aircraft": "34",
  "mlat": "Yes",
  "updated": "Now"
}
```

---

## Home Assistant Page

### Topic

```text
home/dashboard/homeassistant
```

### Current Fields

| Field | Description | Example |
|---|---|---|
| `sparePlug13` | State of Home Assistant entity `switch.spare_plug_13_socket_1` | `"ON"` or `"OFF"` |
| `alarm` | Reserved for a future alarm state | `"Disarmed"` |
| `indoorTemperature` | Reserved for a future indoor-temperature value | `"72 F"` |
| `humidity` | Reserved for a future humidity value | `"41%"` |

### Current Live Payload

```json
{
  "sparePlug13": "ON"
}
```

or:

```json
{
  "sparePlug13": "OFF"
}
```

### Current Home Assistant Automation

```yaml
alias: ESP32 Dashboard - Spare Plug 13
description: Publishes Spare Plug 13 state to the ESP32 dashboard

triggers:
  - trigger: state
    entity_id:
      - switch.spare_plug_13_socket_1

conditions: []

actions:
  - action: mqtt.publish
    data:
      topic: home/dashboard/homeassistant
      payload: >-
        {
          "sparePlug13": "{{ states('switch.spare_plug_13_socket_1') | upper }}"
        }
      qos: 0
      retain: true

mode: restart
```

### ESP32 Display Mapping

| MQTT Field | Display Label |
|---|---|
| `sparePlug13` | `Plug 13` |
| `alarm` | `Alarm` |
| `indoorTemperature` | `Indoor` |
| `humidity` | `Humidity` |

The `Plug 13` value is shown in green when `ON` and red when `OFF`.

---

## FlightRadar24 Page

### Topic

```text
home/dashboard/flightradar
```

### Current Fields

| Field | Description | Example |
|---|---|---|
| `feed` | Feeder status | `"Online"` |
| `aircraft` | Aircraft count | `"34"` |
| `mlat` | MLAT status | `"Yes"` |
| `updated` | Human-readable last-update value | `"Now"` |

### Complete Payload

```json
{
  "feed": "Online",
  "aircraft": "34",
  "mlat": "Yes",
  "updated": "Now"
}
```

### Partial Payload Examples

```json
{
  "aircraft": "41"
}
```

```json
{
  "updated": "5 sec"
}
```

### ESP32 Display Mapping

| MQTT Field | Display Label |
|---|---|
| `feed` | `Feed` |
| `aircraft` | `Aircraft` |
| `mlat` | `MLAT` |
| `updated` | `Updated` |

At present, FlightRadar24 data is being tested manually with MQTT.fx. A real publisher has not yet been implemented.

---

## Pi-hole Page

### Planned Topic

```text
home/dashboard/pihole
```

### Planned Fields

| Field | Description | Example |
|---|---|---|
| `status` | Pi-hole service status | `"Online"` |
| `queries` | DNS query count | `"12,438"` |
| `blocked` | Blocked-query percentage | `"8.4%"` |
| `clients` | Client count | `"27"` |

### Planned Payload

```json
{
  "status": "Online",
  "queries": "12,438",
  "blocked": "8.4%",
  "clients": "27"
}
```

This topic is documented for planning only. The ESP32 is not yet subscribed to it.

---

## Topic Summary

| Page | Topic | Status |
|---|---|---|
| Home Assistant | `home/dashboard/homeassistant` | Implemented and live |
| FlightRadar24 | `home/dashboard/flightradar` | Implemented; currently tested with MQTT.fx |
| Pi-hole | `home/dashboard/pihole` | Planned |

## Testing with MQTT.fx

Subscribe to:

```text
home/dashboard/#
```

Publish valid JSON to the desired page topic.

Example:

**Topic**

```text
home/dashboard/homeassistant
```

**Payload**

```json
{
  "sparePlug13": "ON"
}
```

Publishing plain text such as:

```text
blaine
```

will fail JSON parsing and produce an `InvalidInput` warning in the ESP32 serial log.

## Design Notes

- MQTT is a transport layer, not the UI definition.
- The ESP32 application controls page layouts, labels, colors, and page rotation.
- Topic-specific handlers update stored page data.
- A visible page redraws immediately when its data changes.
- A non-visible page displays the latest stored data when normal rotation reaches it.
- The MQTT service remembers subscriptions and re-subscribes after reconnecting.

## Future Additions

Potential future fields and pages should be added here before or alongside implementation.

Likely next additions:

- More Home Assistant entities
- Live Pi-hole statistics
- Real FlightRadar24 publishing
- Complete retained snapshots for each dashboard page
- A non-secret configuration file for broker address, port, topics, and page timing
