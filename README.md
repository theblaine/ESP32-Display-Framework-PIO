# ESP32 Display Framework for PlatformIO

Version 1.0 development baseline for a reusable, multi-board ESP32 display framework. The current baseline remains fully targeted to the **Waveshare ESP32-S3-LCD-1.47** while the architecture is refactored in small, testable steps.

The repository contains working board drivers, layered display libraries, reusable UI widgets, button handling, runtime backlight control, and focused demo environments. It is intended to be a known-good starting point for new projects using this specific board.

## Hardware

- Waveshare ESP32-S3-LCD-1.47
- ESP32-S3 with 16 MB flash and 8 MB PSRAM
- 172 × 320 ST7789 LCD
- microSD card slot
- Addressable RGB LED
- BOOT button on GPIO 0

Detailed hardware notes and pin assignments are in [`docs/Board.md`](docs/Board.md).

## Software

- PlatformIO
- Arduino framework
- `pioarduino/platform-espressif32`

The shared PlatformIO configuration is defined in [`platformio.ini`](platformio.ini):

```ini
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
board = esp32-s3-devkitc1-n16r8
framework = arduino
```

## Display Architecture

The display code is organized in layers:

```text
Display_ST7789
      ↓
Display_GFX
      ↓
Display_Text
      ↓
Display_Widgets
```

- **Display_ST7789** initializes and communicates with the LCD controller and manages backlight brightness.
- **Display_GFX** provides drawing primitives.
- **Display_Text** provides positioned text drawing and console-style printing.
- **Display_Widgets** provides reusable dashboard and UI components.

## Reusable Libraries

| Library | Purpose |
|---|---|
| `Buttons` | Debounced BOOT-button events, including short, long, and very-long presses |
| `Display_ST7789` | LCD initialization, transfer functions, and backlight control |
| `Display_GFX` | Graphics primitives used by the higher display layers |
| `Display_text` | Text positioning, alignment, measurement, cursor, and print functions |
| `Display_Widgets` | Panels, labels, values, progress bars, status indicators, bars, icons, batteries, and tables |
| `Logger` | Lightweight INFO, WARN, and ERROR serial logging |
| `LVGL_Driver` | LVGL display integration |
| `PNGdec` | PNG decoding support |
| `RGB_Lamp` | Onboard addressable RGB LED control |
| `SD_Card` | microSD initialization and file helpers |
| `Wireless` | Wi-Fi and Bluetooth-related helpers used by the original demos |

## Display Widgets

The widget library currently includes:

- Panels and labels
- Progress bars
- Values and label/value cards
- Status indicators
- Header and footer bars
- Built-in monochrome icons
- Icon-and-label rows
- Battery indicators
- Label/value tables

The complete interactive widget demonstration is documented in [`src/demos/display_widgets/README.md`](src/demos/display_widgets/README.md).

## Demo Environments

Each demo is built as a separate PlatformIO environment.

| Environment | Description |
|---|---|
| `demo_display_widgets` | Interactive demonstration of the reusable widget library |
| `demo_display_console` | Console-style text output using cursor and print functions |
| `demo_display_text` | Positioned, centered, and right-aligned text drawing |
| `demo_display_graphics` | Low-level graphics primitives |
| `demo_factory` | Original combined Waveshare hardware demonstration |
| `demo_png` | PNG loading and rendering from microSD |
| `demo_wifi` | Wi-Fi initialization and scanning |
| `demo_rgb` | Onboard RGB LED demonstration |

The default environment is currently:

```ini
default_envs = demo_display_text
```

Change that line in `platformio.ini`, select an environment from the PlatformIO toolbar, or build explicitly from a terminal:

```bash
pio run -e demo_display_widgets
pio run -e demo_display_widgets -t upload
pio device monitor -b 115200
```

## Wi-Fi Credentials

The real credential file is intentionally excluded from Git:

```text
include/secrets.h
```

To use a Wi-Fi demo, copy the example and enter local credentials:

```text
include/secrets.example.h
        ↓
include/secrets.h
```

Never commit `include/secrets.h`.

## Widget Demo Controls

While running `demo_display_widgets`:

- **Short BOOT press:** advance to the next page
- **Long BOOT press:** return to the configured startup page
- **Hold BOOT for three seconds:** cycle brightness through 100%, 75%, 50%, and 25%
- **RESET:** restart and return to the startup page

## Project Structure

```text
docs/                       Board-specific documentation
include/                    Project-wide headers and local secrets
lib/                        Reusable and third-party libraries
src/demos/                  One source folder per PlatformIO demo environment
test/                       Reserved for PlatformIO tests
platformio.ini              Environments and build configuration
```

## Project Status

**Version 1.0.0 — stable Waveshare-specific template**

This repository is the completed reference project for the Waveshare ESP32-S3-LCD-1.47. Future multi-board development continues separately in the **ESP32-Display-Framework-PIO** project.

## License Notes

Third-party libraries retain their own licenses and documentation inside their respective folders. Review those licenses before redistributing a combined binary or source package.

Project-specific source may be used and adapted according to the license selected for this repository.

## Author

Created and maintained by Blaine Sherman.
