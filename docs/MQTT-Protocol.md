# Home Dashboard MQTT Protocol

## Purpose

This document defines MQTT topics and payloads currently used by the ESP32 Home Dashboard application.

The ESP32 owns screen layout and rendering. MQTT publishers supply data only.

The authoritative topic registration/routing code is in:

```text
src/apps/home_dashboard/main.cpp
```

## Current Development Broker

Current application constants:

```text
Host: 10.0.0.50
Port: 1883
Client ID: waveshare-home-dashboard
```

These are development values in `main.cpp`, not a generic framework requirement.

## MQTTService Behavior

The Home Dashboard:

1. starts `MQTTService`
2. registers one message callback
3. registers page topics
4. calls `MQTTService::loop()` continuously
5. lets `MQTTService` reconnect and re-subscribe automatically

Messages are routed by exact topic string in `handleMqttMessage()`.

## General JSON Pattern

Most dashboard topics use JSON.

Typical page behavior:

- malformed JSON is rejected/logged
- page-specific handlers own parsing
- a page stores the latest received data
- if the page is visible when data arrives, it redraws immediately
- if the page is not visible, the stored data appears when rotation later reaches it

Some page handlers preserve omitted values; others use defaults. Check the page implementation before relying on partial JSON updates.

## Home Assistant

Topic:

```text
home/dashboard/homeassistant
```

Current page source:

```text
HomeAssistantPage.cpp
```

The current MQTT handler parses only the `sparePlug13` field.

The page also displays Alarm, Indoor Temperature, and Humidity rows, but those
values are currently initialized placeholders rather than MQTT-driven fields.
Do not assume those rows are populated from this topic until their handlers are
implemented.

Example:

```json
{
  "sparePlug13": "ON"
}
```

## FlightRadar24

Topic:

```text
home/dashboard/flightradar
```

Current fields:

```text
feed
aircraft
mlat
updated
```

Example:

```json
{
  "feed": "Online",
  "aircraft": "7",
  "mlat": "No",
  "updated": "Now"
}
```

## Pi-hole

Topic:

```text
home/dashboard/pihole
```

Current fields:

```text
status
queries
blocked
clients
```

Example:

```json
{
  "status": "Online",
  "queries": "13,653",
  "blocked": "12.6%",
  "clients": "6"
}
```

The current implementation is active, not merely planned.

## DeathStar Ping Response

Response topic:

```text
home/deathstar/ping/response
```

When `DeathStarPage::draw()` runs, it publishes:

```text
Topic: home/deathstar/ping
Payload: status
```

Expected response JSON fields:

```text
hostname
internal_ip
external_ip
vpn_status
timestamp
```

Example:

```json
{
  "hostname": "DEATHSTAR",
  "internal_ip": "10.0.0.14",
  "external_ip": "73.192.212.24",
  "vpn_status": "Disconnected",
  "timestamp": "2026-08-10T21:25:14"
}
```

## Device Overview

Topic:

```text
home/dashboard/deviceoverview
```

Current fields:

```text
location
status
```

Example:

```json
{
  "location": "Home Office",
  "status": "Online"
}
```

`DeviceOverviewPage` combines this MQTT data with local state:

- IP address from `NetworkService`
- RSSI from `NetworkService`
- Wi-Fi state
- MQTT connection state
- PNG asset loaded from SD card
- signal meter derived from RSSI

This is the best current example of a composite page using both local and external data.

## Topic Summary

| Function/Page | Topic | Direction |
|---|---|---|
| Home Assistant | `home/dashboard/homeassistant` | Broker → ESP32 |
| FlightRadar24 | `home/dashboard/flightradar` | Broker → ESP32 |
| Pi-hole | `home/dashboard/pihole` | Broker → ESP32 |
| Device Overview | `home/dashboard/deviceoverview` | Broker → ESP32 |
| DeathStar request | `home/deathstar/ping` | ESP32 → Broker |
| DeathStar response | `home/deathstar/ping/response` | Broker → ESP32 |

## Testing with MQTT.fx

Subscribe to:

```text
home/dashboard/#
```

For Device Overview, publish:

**Topic**

```text
home/dashboard/deviceoverview
```

**Payload**

```json
{
  "location": "Garage",
  "status": "Offline"
}
```

Expected result when Device Overview is visible:

- page redraws immediately
- Location becomes `Garage`
- Status becomes red `Offline`

Publish again with:

```json
{
  "location": "Home Office",
  "status": "Online"
}
```

Status becomes green `Online`.

## Adding a New MQTT Page

See:

```text
docs/CREATING_PAGES.md
```

The standard pattern is:

1. add page `handleMessage(payload)`
2. add topic constant in `main.cpp`
3. register with `MQTTService::subscribe()`
4. route topic in the central callback
5. redraw only when that page is currently visible

## Retained Messages

For state-like data, retained MQTT messages are usually useful because a reconnecting ESP32 can immediately receive the latest state.

Whether a publisher should retain a message is an application decision rather than enforced by the framework.
