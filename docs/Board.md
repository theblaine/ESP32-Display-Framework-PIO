# Waveshare ESP32-S3-LCD-1.47

## Overview

This document contains hardware-specific information for the Waveshare
ESP32-S3-LCD-1.47 development board.

For framework-wide portability and support status, also see:

```text
docs/BOARD_SUPPORT.md
```

---

# MCU

| Item | Value |
|------|-------|
| MCU | ESP32-S3 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| Framework | Arduino |

---

# PlatformIO

Working shared configuration:

```ini
board = esp32-s3-devkitc1-n16r8

board_build.flash_mode = qio
board_build.flash_size = 16MB

build_flags =
    -DBOARD_HAS_PSRAM
    -DDISPLAY_BOARD_WAVESHARE_147
```

Individual environments may add or override settings. The authoritative
configuration is always `platformio.ini`.

---

# Display

| Item | Value |
|------|-------|
| Controller | ST7789 |
| Resolution | 172×320 |
| Public facade | `lib/Display` |
| Graphics | `lib/Display_GFX` |
| Text | `lib/Display_Text` |
| Widgets | `lib/Display_Widgets` |
| Low-level driver | `lib/Display_ST7789` |

Application/page code should normally use the public display libraries
rather than call `Display_ST7789` directly.

---

# microSD

Status: Working

Library:

```text
lib/SD_Card
```

Interface:

```text
SD_MMC
```

Pin assignments are defined in `SD_Card.h`.

Current uses include:

- text/configuration files
- JSON application data
- PNG assets
- raw-sector helpers
- Windows USB Mass Storage maintenance through `demo_sd_usb`

The PNG demo searches for `.png` files on the SD-card root.

---

# RGB LED

Status: Working

Library:

```text
lib/RGB_Lamp
```

Notes:

- Onboard addressable RGB LED.
- Physical LED channel order is **GRB**.
- The `RGB_Lamp` wrapper presents a standard RGB interface.

---

# Wi-Fi

Status: Working

Preferred application service:

```text
lib/NetworkService
```

Notes:

- Up to three candidate networks are currently configured by the Home Dashboard.
- `NetworkService` uses `WiFiMulti` internally.
- Automatic reconnect/connection management is handled by the service.
- Current SSID, IP address, and RSSI are exposed through the public API.
- Shared diagnostics use `Logger`.

New applications should prefer `NetworkService` rather than call
`WiFiMulti` directly.

---

# MQTT

Status: Working

Preferred application service:

```text
lib/MQTTService
```

Notes:

- Uses PubSubClient.
- Handles broker connection/reconnection.
- Registered subscriptions are restored after reconnect.
- Exposes publish, subscribe, connection state, and one application callback.
- The Home Dashboard routes topics centrally in `main.cpp`.

See:

```text
docs/MQTT-Protocol.md
```

---

# NTP / Local Time

Status: Working

Preferred application service:

```text
lib/TimeService
```

`TimeService::begin(...)` uses ESP32 `configTzTime(...)` to configure a POSIX
timezone and NTP servers. Synchronization proceeds in the background, so the
service has no required `loop()`. The Home Dashboard initializes it globally
with Pacific timezone `PST8PDT,M3.2.0,M11.1.0`, and `SystemStatusPage` displays
the resulting local date and time.

---

# Logger

Status: Working

Library:

```text
lib/Logger
```

Notes:

- Centralized logging interface.
- Currently outputs using `printf()`.
- Supports INFO, WARN, and ERROR macros.
- Designed to support additional outputs in the future.

---

# Buttons

| Item | Value |
|------|-------|
| BOOT GPIO | 0 |
| Active state | LOW |
| Library | `lib/Buttons` |

The current button library supports:

- short press
- long press
- very-long press

In the Home Dashboard, short press advances to the next configured page.

With the USB connector at the top and display facing you:

- top-right: BOOT
- top-left: RESET

---

# Pin Assignments

| Function | GPIO |
|----------|------|
| LCD | See `Display_ST7789` / board configuration |
| SD | See `SD_Card.h` |
| RGB | See `RGB_lamp.h` |
| BOOT | 0 |

---

# Tested Features

| Feature | Status |
|---------|--------|
| Display / ST7789 | ✅ |
| Display graphics/text/widgets | ✅ |
| LVGL factory/demo support | ✅ |
| PNG from SD card | ✅ |
| SD JSON/text access | ✅ |
| Windows SD USB maintenance | ✅ |
| NetworkService / Wi-Fi | ✅ |
| MQTTService / MQTT | ✅ |
| TimeService / NTP | ✅ |
| RGB LED | ✅ |
| BOOT button events | ✅ |
| BLE | Not used by current framework application |
| OTA | Not implemented |

---

# Demo Environments

| Environment | Purpose | Status |
|-------------|---------|--------|
| `demo_display_widgets` | Interactive widget demonstration | ✅ |
| `demo_display_console` | Console-style text output | ✅ |
| `demo_display_text` | Positioned and aligned text | ✅ |
| `demo_display_graphics` | Graphics primitives | ✅ |
| `demo_factory` | Original combined hardware demo | ✅ |
| `demo_png` | PNG rendering from microSD | ✅ |
| `demo_wifi` | Wi-Fi connectivity | ✅ |
| `demo_mqtt` | MQTT connectivity | ✅ |
| `demo_ntp` | Wi-Fi + NTP synchronization + display output | ✅ |
| `demo_sd` | SD-card access | ✅ |
| `demo_sd_usb` | Windows USB Mass Storage access to microSD | ✅ |
| `demo_rgb` | RGB LED control | ✅ |

The authoritative environment list is `platformio.ini`.

---

# Project Layout

```text
lib/
include/
src/
docs/
```

---

# Serial Output

- Shared monitor speed: **115200 baud**
- Normal framework logging currently uses `printf()`.
- Normal Home Dashboard serial monitoring is verified.
- `demo_sd_usb` is a special native-USB maintenance environment; do not
  rely on its serial behavior as representative of the normal application.

---

# Related Documentation

```text
docs/GETTING_STARTED.md
docs/FRAMEWORK_OVERVIEW.md
docs/LIBRARY_REFERENCE.md
docs/BOARD_SUPPORT.md
docs/SD_CARD_GUIDE.md
docs/MQTT-Protocol.md
```
