# Framework Overview

## Goal

The framework is designed so a project can reuse display, UI, network, MQTT, SD-card, button, image, and RGB functionality without copying hardware-specific implementation into every application.

The primary design rule is:

> Applications should depend on stable public framework layers. Board-specific and low-level display details should stay below them.

## Major Layers

```text
Home Dashboard / Demos
        │
        ├── Page modules
        ├── NetworkService
        ├── MQTTService
        ├── SD_Card
        ├── PNG_Image
        ├── Buttons
        ├── RGB_Lamp
        └── Logger
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

## Display_Boards

Header:

```cpp
#include "DisplayConfig.h"
```

This layer selects board-specific display configuration at compile time.

Current build flags include:

```text
DISPLAY_BOARD_WAVESHARE_147
DISPLAY_BOARD_TTGO_T_DISPLAY
```

Applications generally should not include board configuration headers directly.

## Display_ST7789

Low-level ST7789 driver and backlight implementation.

Application code should normally avoid using it directly.

`PNG_Image` currently uses the low-level `LCD_AddWindow()` primitive internally because the decoder outputs scanlines.

## Display

Public display facade.

Use it for:

- initialization
- width/height
- brightness

Example:

```cpp
#include "Display.h"

Display::begin();
Display::setBrightness(90);

const uint16_t width = Display::width();
const uint16_t height = Display::height();
```

## Display_GFX

Board-independent primitive graphics.

Use it for:

- screen fills
- pixels
- lines
- rectangles
- circles
- triangles
- rounded rectangles
- common RGB565 colors

## Display_Text

Bitmap text rendering and text helpers.

Use it for:

- positioned text
- centered/right-aligned text
- wrapping
- text measurement
- cursor/print behavior

## Display_Widgets

Higher-level reusable UI components built from graphics and text.

Examples include:

- panels
- labels
- values
- label/value cards
- status indicators
- header/footer bars
- battery indicators
- progress bars
- signal meters
- tables
- built-in monochrome icons

The widget library intentionally avoids page-specific business logic.

For example, `Display_DrawSignalMeter()` accepts a generic level. `NetworkPage` owns the Wi-Fi RSSI thresholds used to convert dBm to a 0–4 signal level.

## Services

### NetworkService

Owns Wi-Fi network registration, connection management, reconnect behavior, and simple connection information.

Call `NetworkService::loop()` regularly.

### MQTTService

Owns broker connection, topic subscription registration, automatic re-subscription, publish, and one application message callback.

Call `MQTTService::loop()` regularly.

The Home Dashboard has one central MQTT router in `main.cpp`. It dispatches topic payloads to page-specific handlers.

## Supporting Hardware Libraries

### Buttons

Debounces the BOOT button and exposes short, long, and very-long press events.

### RGB_Lamp

Controls the onboard addressable RGB LED through a standard RGB interface.

### SD_Card

Owns SD_MMC initialization and common filesystem/raw-sector helpers.

### PNG_Image

Reusable PNG renderer. Reads PNG files from the mounted SD card and draws decoded scanlines at an X/Y position.

### Logger

Shared INFO/WARN/ERROR logging macros.

## Applications vs. Demos

### Demos

Demos should answer a focused question:

> Does this capability work on the hardware?

Examples:

- `demo_display_widgets`
- `demo_png`
- `demo_sd_usb`

Demos are intentionally independent PlatformIO environments.

### Applications

Applications combine reusable pieces into a real workflow.

The main current application is:

```text
src/apps/home_dashboard/
```

Its page modules live under:

```text
src/apps/home_dashboard/pages/
```

## Home Dashboard Ownership

`main.cpp` owns:

- project startup
- page enumeration
- rotation list
- automatic rotation timing
- short-press page advance
- shared footer
- network registration
- MQTT connection configuration
- MQTT topic subscription
- MQTT topic routing

A page module owns:

- page-specific state
- payload parsing for that page
- colors/layout
- drawing

This boundary is important. Avoid putting page-specific layout code back into `main.cpp`.

## Data Flow Example

Device Overview demonstrates multiple sources on one page:

```text
SD card              Local ESP32 state          MQTT
   │                        │                     │
   ▼                        ▼                     ▼
80×80 PNG             IP / RSSI / status     JSON payload
   │                        │                     │
   └──────────────┬─────────┴──────────────┬──────┘
                  ▼                        ▼
             DeviceOverviewPage::draw()
                  │
                  ▼
               Display
```

This is a useful pattern for future pages.

## Third-Party and Legacy Components

The repository also contains:

- `PNGdec` — third-party PNG decoder used by `PNG_Image`
- `lvgl` — third-party LVGL tree
- `LVGL_Driver` — retained for factory/demo functionality
- `Wireless` — legacy Wi-Fi/BLE test helpers

Do not confuse these with the preferred application-facing service layers such as `NetworkService`.
