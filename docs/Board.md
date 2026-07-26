# Waveshare ESP32-S3-LCD-1.47

## Overview

This document contains hardware-specific information for the Waveshare
ESP32-S3-LCD-1.47 development board.

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

Working configuration:

```ini
board = esp32-s3-devkitc1-n16r8

board_build.flash_mode = qio
board_build.flash_size = 16MB

build_flags =
    -DBOARD_HAS_PSRAM
```

---

# Display

| Item | Value |
|------|-------|
| Controller | ST7789 |
| Resolution | 172×320 |
| Library | Display_ST7789 |

---

# microSD

Status: Working

Library:

```
lib/SD_Card
```

Notes:

- PNG demo expects images in the root directory.

---

# RGB LED

Status: Working

Library:

```
lib/RGB_Lamp
```

Notes:

- Onboard addressable RGB LED.
- Physical LED channel order is **GRB**.
- The `RGB_Lamp` wrapper presents a standard RGB interface.

---

# Wi-Fi

Status: Working

Library:

```
WiFiMulti (ESP32 Arduino Core)
```

Notes:

- Supports multiple configured Wi-Fi networks.
- Automatic reconnect enabled.
- Uses the shared `Logger` library for diagnostics.

---

# Logger

Status: Working

Library:

```
lib/Logger
```

Notes:

- Centralized logging interface.
- Currently outputs using `printf()`.
- Supports:
  - INFO
  - WARN
  - ERROR
- Designed to support additional outputs in the future.

---

# Buttons

| Item | Value |
|------|-------|
| BOOT GPIO | 0 |
| Active state | LOW |
| Library | `lib/Buttons` |

The widget demo uses short, long, and three-second very-long press events.

---

# Pin Assignments

| Function | GPIO |
|----------|------|
| LCD | See Display_ST7789 |
| SD | See SD_Card |
| RGB | See RGB_Lamp |

---

# Tested Features

| Feature | Status |
|---------|--------|
| LVGL | ✅ |
| PNG / SD Card | ✅ |
| WiFiMulti | ✅ |
| RGB LED | ✅ |
| BLE | ☐ |
| OTA | ☐ |
| MQTT | ☐ |

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
| `demo_wifi` | Wi-Fi connectivity and scanning | ✅ |
| `demo_rgb` | RGB LED control | ✅ |

---

# Project Layout

```
lib/
include/
src/
docs/
```

---

# Useful Links

- Waveshare Wiki
- Waveshare GitHub
- LVGL
- PNGdec

---

# Serial Output

- Serial monitor: **115200 baud**
- `printf()` is verified working through the board's USB serial connection.
- Arduino `Serial` is not currently used by the template.
- The shared `Logger` library currently outputs through `printf()`.