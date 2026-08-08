# ESP32 Display Framework for PlatformIO

**Current version: v0.2.0**

A reusable PlatformIO framework for ESP32 display projects, built around layered display libraries, board-specific configuration, focused hardware demos, and modular applications.

The framework currently supports the **Waveshare ESP32-S3-LCD-1.47** as the primary board and includes early **TTGO T-Display V1.1** support for selected display demos.

## Current Status

Version **v0.2.0** establishes the first modular framework baseline:

- Layered ST7789 display stack
- Board-specific display configuration
- Reusable graphics, text, and widget libraries
- Runtime LCD brightness control
- Wi-Fi and MQTT service wrappers
- Reusable button and RGB LED helpers
- Independent PlatformIO demo environments
- Modular Home Dashboard application
- Configurable dashboard page rotation and locked development mode
- Initial multi-board support

The project is still evolving. Public APIs and internal organization may change before a future 1.0 release.

## Supported Boards

### Waveshare ESP32-S3-LCD-1.47

Primary development board.

- ESP32-S3
- 16 MB flash
- 8 MB PSRAM
- 172 × 320 ST7789 LCD
- microSD card slot
- Addressable RGB LED
- BOOT button on GPIO 0

See [`docs/Board.md`](docs/Board.md) for current Waveshare hardware notes.

### TTGO T-Display V1.1

Early framework support is available for selected display demos using the classic ESP32-based TTGO T-Display V1.1.

Current TTGO PlatformIO environments:

- `ttgo_display_graphics`
- `ttgo_display_text`
- `ttgo_display_console`
- `ttgo_display_calibration`

## Software

- PlatformIO
- Arduino framework
- `pioarduino/platform-espressif32`
- PubSubClient
- ArduinoJson where required

Shared build settings and all environments are defined in [`platformio.ini`](platformio.ini).

The current default environment is:

```ini
[platformio]
default_envs = app_home_dashboard
```

## Framework Architecture

The display stack is intentionally layered so board-specific hardware details remain below reusable drawing and UI code.

```text
Display_Boards
      ↓
Display_ST7789
      ↓
Display
      ↓
Display_GFX
      ↓
Display_Text
      ↓
Display_Widgets
```

### Core Display Libraries

| Library | Responsibility |
|---|---|
| `Display_Boards` | Compile-time board selection and board-specific display configuration |
| `Display_ST7789` | Low-level ST7789 initialization, transfers, orientation, and backlight PWM |
| `Display` | Public display facade used by application and framework code |
| `Display_GFX` | Graphics primitives |
| `Display_Text` | Positioned text, alignment, measurement, cursor, and print functions |
| `Display_Widgets` | Higher-level reusable dashboard and UI components |

### Supporting Libraries

| Library | Responsibility |
|---|---|
| `Buttons` | Debounced button events including short, long, and very-long presses |
| `Logger` | Lightweight serial logging |
| `NetworkService` | Wi-Fi connection management used by current applications and demos |
| `MQTTService` | MQTT connection, subscription, callback, and reconnect handling |
| `RGB_Lamp` | Addressable RGB LED control |
| `SD_Card` | microSD initialization and file helpers |
| `LVGL_Driver` | LVGL integration retained for the factory/demo stack |
| `Wireless` | Wireless helper code retained for original/demo functionality |
| `PNGdec` | Third-party PNG decoder |
| `lvgl` | Third-party LVGL source tree |

## Home Dashboard Application

The primary application is located at:

```text
src/apps/home_dashboard/
```

Its pages are modular and live under:

```text
src/apps/home_dashboard/pages/
```

Current pages:

- Home Assistant
- FlightRadar24
- Pi-hole
- Test

`main.cpp` owns application-level behavior such as startup, MQTT topic routing, page rotation, and the shared status footer. Each page module owns its own data, MQTT payload parsing, page-specific colors, and drawing code.

The dashboard supports two operating modes:

- **Rotate** — cycles through the explicitly configured rotation-page list.
- **Locked** — remains on one selected page for development and testing.

This allows experimental or test pages to remain in the project without appearing in normal dashboard rotation.

MQTT topics and payload formats are documented in [`docs/MQTT-Protocol.md`](docs/MQTT-Protocol.md).

## Display Widgets

The widget library currently includes:

- Panels and labels
- Values and label/value cards
- Progress bars
- Status indicators
- Header and footer bars
- Built-in monochrome icons
- Icon-and-label rows
- Battery indicators
- Label/value tables

The interactive widget demonstration is documented in [`src/demos/display_widgets/README.md`](src/demos/display_widgets/README.md).

## PlatformIO Environments

### Waveshare

| Environment | Purpose |
|---|---|
| `app_home_dashboard` | Modular Home Dashboard application |
| `demo_display_widgets` | Interactive widget demonstration |
| `demo_display_console` | Console-style text output |
| `demo_display_text` | Positioned and aligned text |
| `demo_display_graphics` | Graphics primitives |
| `demo_factory` | Original combined Waveshare hardware demonstration |
| `demo_png` | PNG rendering from microSD |
| `demo_wifi` | Wi-Fi initialization and scanning |
| `demo_mqtt` | MQTT connectivity and JSON messaging |
| `demo_sd` | microSD functionality |
| `demo_rgb` | RGB LED control |

### TTGO T-Display V1.1

| Environment | Purpose |
|---|---|
| `ttgo_display_graphics` | Graphics demo |
| `ttgo_display_text` | Text demo |
| `ttgo_display_console` | Console text demo |
| `ttgo_display_calibration` | Display calibration/testing |

## Building and Uploading

Build the default environment:

```bash
pio run
```

Build a specific environment:

```bash
pio run -e demo_display_widgets
```

Build and upload:

```bash
pio run -e demo_display_widgets -t upload
```

Open the serial monitor:

```bash
pio device monitor -b 115200
```

You can also select environments and run Build/Upload from the PlatformIO controls in VS Code.

## Wi-Fi Credentials

Local credentials belong in:

```text
include/secrets.h
```

That file is intentionally excluded from Git.

Start from:

```text
include/secrets.example.h
```

and create your own local `include/secrets.h`.

Never commit real credentials.

## Project Structure

```text
ESP32-Display-Framework-PIO/
├── docs/                       Project and hardware documentation
├── include/                    Project-wide headers and local configuration
├── lib/                        Reusable framework and third-party libraries
├── src/
│   ├── apps/                   Complete applications
│   │   └── home_dashboard/
│   └── demos/                  Focused PlatformIO demos
├── test/                       Reserved for PlatformIO tests
├── BASELINE.md                 Historical refactor-baseline notes
├── CHANGELOG.md                Project change history
├── README.md                   Project overview
└── platformio.ini              PlatformIO environments and build configuration
```

## Versioning

The project uses semantic-style version numbers:

```text
MAJOR.MINOR.PATCH
```

During pre-1.0 development:

- Patch releases are used for fixes, documentation, and behavior-preserving cleanup.
- Minor releases are used for meaningful framework capabilities, new boards, widgets, or other larger additions.
- A future `v1.0.0` will mark a stable public framework baseline.

The current baseline is **v0.2.0**.

## Documentation

- [`docs/Board.md`](docs/Board.md) — Waveshare hardware notes
- [`docs/MQTT-Protocol.md`](docs/MQTT-Protocol.md) — Home Dashboard MQTT topics and payloads
- [`src/demos/display_widgets/README.md`](src/demos/display_widgets/README.md) — widget-demo controls and behavior
- [`BASELINE.md`](BASELINE.md) — historical notes from the initial framework refactor
- [`CHANGELOG.md`](CHANGELOG.md) — project history

## License Notes

Third-party libraries retain their own licenses and documentation inside their respective folders. Review those licenses before redistributing a combined binary or source package.

A project-level license should be selected before publishing the repository for broad reuse.

## Author

Created and maintained by Blaine Sherman.
