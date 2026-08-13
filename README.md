# ESP32 Display Framework for PlatformIO

**Current version: v0.2.11**

A reusable PlatformIO framework for ESP32 display projects. The project separates board-specific display configuration, reusable drawing/UI libraries, hardware services, focused demos, and modular applications so that future projects can reuse only the pieces they need.

The primary development target is the **Waveshare ESP32-S3-LCD-1.47**. Selected display demos also support the **TTGO T-Display V1.1**.

> This project is still pre-1.0. APIs and organization may continue to evolve.

## Start Here

If you are new to this repository, read these in order:

1. [`docs/GETTING_STARTED.md`](docs/GETTING_STARTED.md) — clone, configure Wi-Fi, build, upload, and run a demo/application.
2. [`docs/FRAMEWORK_OVERVIEW.md`](docs/FRAMEWORK_OVERVIEW.md) — how applications, pages, services, libraries, and board configuration fit together.
3. [`docs/LIBRARY_REFERENCE.md`](docs/LIBRARY_REFERENCE.md) — what each library does, which header to include, and its main public API.
4. [`docs/CREATING_PAGES.md`](docs/CREATING_PAGES.md) — build a new Home Dashboard page and optionally feed it MQTT data.
5. [`docs/MQTT-Protocol.md`](docs/MQTT-Protocol.md) — current Home Dashboard MQTT topics and payloads.
6. [`docs/SD_CARD_GUIDE.md`](docs/SD_CARD_GUIDE.md) — SD files, JSON, PNG assets, raw-sector support, and Windows USB maintenance mode.
7. [`docs/BOARD_SUPPORT.md`](docs/BOARD_SUPPORT.md) — supported boards and current portability boundaries.

Hardware details for the primary board are also documented in [`docs/Board.md`](docs/Board.md).

## Supported Boards

### Waveshare ESP32-S3-LCD-1.47

Primary board and the only board currently exercising the complete Home Dashboard feature set.

- ESP32-S3
- 16 MB flash
- 8 MB PSRAM
- 172 × 320 ST7789 LCD
- microSD card slot
- addressable RGB LED
- BOOT button on GPIO 0

### TTGO T-Display V1.1

Early support for selected display-only demos.

Current TTGO environments:

- `ttgo_display_graphics`
- `ttgo_display_text`
- `ttgo_display_console`
- `ttgo_display_calibration`

See [`docs/BOARD_SUPPORT.md`](docs/BOARD_SUPPORT.md).

## Repository Layout

```text
ESP32-Display-Framework-PIO/
├── platformio.ini
├── include/
│   ├── ProjectVersion.h
│   └── secrets.example.h
├── lib/
│   ├── Buttons/
│   ├── Display/
│   ├── Display_Boards/
│   ├── Display_GFX/
│   ├── Display_ST7789/
│   ├── Display_Text/
│   ├── Display_Widgets/
│   ├── Logger/
│   ├── MQTTService/
│   ├── NetworkService/
│   ├── PNG_Image/
│   ├── RGB_Lamp/
│   └── SD_Card/
├── src/
│   ├── apps/
│   │   └── home_dashboard/
│   └── demos/
└── docs/
```

Third-party/legacy trees such as `PNGdec`, `lvgl`, `LVGL_Driver`, and `Wireless` are also retained where required by existing demos.

## Architecture at a Glance

```text
Applications / Demos
        │
        ├── Pages
        ├── NetworkService
        ├── MQTTService
        ├── SD_Card
        ├── Buttons
        ├── RGB_Lamp
        └── PNG_Image
                │
                ▼
Display_Widgets
Display_Text
Display_GFX
Display
                │
                ▼
Display_ST7789
                │
                ▼
Display_Boards
```

Application code should normally use the public framework layers rather than call `Display_ST7789` directly.

## Home Dashboard

The main application is:

```text
src/apps/home_dashboard/
```

Current page modules include:

- Test
- Home Assistant
- FlightRadar24
- Pi-hole
- System Status
- System Monitor
- Network
- DeathStar
- SD Card
- Image
- Device Overview

`main.cpp` owns application-level behavior:

- startup
- page rotation
- BOOT-button navigation
- shared status footer
- Wi-Fi configuration
- MQTT client setup and topic routing

Each page owns its own drawing and page-specific data.

The rotation list is intentionally explicit. Comment, uncomment, reorder, or remove entries in `ROTATION_PAGES` to control which pages appear.

## Demos

The repository includes independent demos so capabilities can be tested without the Home Dashboard:

| Environment | Purpose |
|---|---|
| `demo_display_widgets` | Reusable UI widgets |
| `demo_display_console` | Cursor/console text |
| `demo_display_text` | Positioned/aligned text |
| `demo_display_graphics` | Graphics primitives |
| `demo_factory` | Original combined Waveshare hardware demo |
| `demo_png` | PNG files rendered from microSD |
| `demo_wifi` | Wi-Fi connectivity |
| `demo_mqtt` | MQTT connectivity |
| `demo_sd` | SD-card access |
| `demo_sd_usb` | Expose the SD card to Windows as USB Mass Storage |
| `demo_rgb` | Addressable RGB LED |

See `platformio.ini` for the authoritative environment list.

## Configuration and Secrets

The repository intentionally includes:

```text
include/secrets.example.h
```

but not a real `secrets.h`.

For the Home Dashboard, copy:

```text
include/secrets.example.h
```

to:

```text
include/secrets.h
```

and edit the Wi-Fi values. Keep `secrets.h` out of source control.

## Versioning

Project version constants live in:

```text
include/ProjectVersion.h
```

Version numbers are intended for meaningful feature/release milestones, not every Git commit.

See [`CHANGELOG.md`](CHANGELOG.md).
