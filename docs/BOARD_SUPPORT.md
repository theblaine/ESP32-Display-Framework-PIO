# Board Support

## Support Philosophy

The framework separates board-specific display configuration from reusable drawing/UI code.

A board is not considered fully supported merely because one display demo builds. Document support by capability.

## Waveshare ESP32-S3-LCD-1.47

Status: **Primary / full development board**

Current PlatformIO board:

```ini
board = esp32-s3-devkitc1-n16r8
board_build.flash_mode = qio
board_build.flash_size = 16MB
```

Shared build flags:

```ini
-DBOARD_HAS_PSRAM
-DDISPLAY_BOARD_WAVESHARE_147
```

Current known hardware:

| Item | Value |
|---|---|
| MCU | ESP32-S3 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| LCD | ST7789 |
| Resolution | 172×320 |
| microSD | SD_MMC, working |
| RGB LED | working |
| BOOT button | GPIO 0 |
| Wi-Fi | working |
| MQTT | working |
| NTP/local time | working via `TimeService` and `demo_ntp` |
| PNG from SD | working |
| Windows SD USB maintenance | working via `demo_sd_usb` |

The complete Home Dashboard is currently designed/tested on this board.

See `Board.md` for pin-level notes.

## TTGO T-Display V1.1

Status: **Partial / display-focused, physically verified**

PlatformIO board:

```ini
board = esp32dev
board_build.flash_mode = dio
board_build.flash_size = 4MB
```

Build flag:

```ini
-DDISPLAY_BOARD_TTGO_T_DISPLAY
```

Current environments:

```text
ttgo_display_graphics
ttgo_display_text
ttgo_display_console
ttgo_display_calibration
ttgo_display_widgets
ttgo_wifi
ttgo_mqtt
ttgo_ntp
```

The widget environment uses the same `src/demos/display_widgets/` source and
shared `Display_Widgets` implementation as Waveshare. Demo layouts select
compact positions, spacing, and sizes from `Display::width()` and
`Display::height()`; no TTGO-specific widget implementation exists. Physical
TTGO testing verified that the widgets render correctly with these layouts.

Physical testing also verified Wi-Fi connectivity, MQTT plain-text/JSON
handling, and NTP synchronization through `TimeService`. The MQTT demo uses
`WiFiMulti` and `PubSubClient` directly, so it does not prove the separate
`MQTTService` abstraction.

TTGO remains display/service-demo focused. Its current configuration has a
135×240 ST7789 display, 4 MB flash, and no configured PSRAM. The Waveshare SD,
RGB LED, button, PNG, native-USB maintenance, and full Home Dashboard paths
have not been claimed as TTGO-supported.

Do not assume the Waveshare Home Dashboard hardware features exist on TTGO.

In particular, the current framework application makes assumptions about:

- SD card availability
- Waveshare RGB LED
- BOOT-button behavior
- board-specific PNG line width
- display dimensions/layout
- native USB maintenance workflow

Those need board-specific review before claiming full Home Dashboard support.

## Board Selection

`lib/Display_Boards/DisplayConfig.h` chooses a board configuration based on compile-time flags.

Current selectors:

```cpp
DISPLAY_BOARD_WAVESHARE_147
DISPLAY_BOARD_TTGO_T_DISPLAY
```

If neither is defined, the build fails intentionally.

## Adding a New Display Board

A new board should generally be introduced in stages.

### Stage 1: Prove Display Hardware

Create a board-specific display configuration and compile a simple graphics/text demo.

Verify:

- resolution
- orientation
- SPI pins
- reset/DC/CS
- backlight behavior
- color correctness

### Stage 2: Reuse Existing Display Demos

Try:

```text
demo_display_graphics
demo_display_text
demo_display_console
```

Do not start with the full Home Dashboard.

### Stage 3: Verify Widgets

Build the board's widget-demo environment (for TTGO, `ttgo_display_widgets`).

Look for layout assumptions tied to the Waveshare 172×320 screen.

### Stage 4: Audit Supporting Hardware

Check individually:

- button mapping
- RGB LED
- SD interface
- Wi-Fi
- MQTT
- NTP/local time
- USB capabilities

### Stage 5: Application Support

Only after the hardware/services are proven should the Home Dashboard be enabled/adapted.

The current application is not yet fully board-abstracted.

## Portability Boundaries

The display stack is the most portable part of the framework.

More board-specific today:

- `SD_Card` pin definitions
- `RGB_Lamp` pin
- `Buttons` physical button list/mapping
- PNG line-buffer width
- Home Dashboard pixel layouts
- USB SD maintenance behavior

These are good candidates for future board abstraction if more boards are added.

## What "Supported" Should Mean

For documentation, distinguish:

- **Builds** — compiler succeeds
- **Display verified** — rendered on physical hardware
- **Feature verified** — a specific peripheral/service works
- **Application verified** — complete target application works

Avoid labeling a board "fully supported" until the application behavior has been physically tested.
