# Library Reference

This document answers two practical questions:

1. **What header do I include?**
2. **What does that library own?**

For exact parameter lists, the public header in `lib/<Library>/` remains the source of truth.

## Quick Include Map

| Need | Include |
|---|---|
| Initialize display, dimensions, brightness | `Display.h` |
| Colors and primitive shapes | `Display_GFX.h` |
| Text drawing/alignment/wrapping | `Display_Text.h` |
| Panels, headers, tables, progress, status, battery, signal meter | `Display_Widgets.h` |
| PNG from SD card | `PNG_Image.h` |
| SD-card filesystem/raw-sector access | `SD_Card.h` |
| BOOT button short/long/very-long events | `Buttons.h` |
| RGB LED | `RGB_lamp.h` |
| Logging | `Logger.h` |
| Wi-Fi management/state | `NetworkService.h` |
| MQTT connection/subscribe/publish | `MQTTService.h` |
| NTP synchronization and local date/time | `TimeService.h` |
| Project version macros | `ProjectVersion.h` |

## Display

Header:

```cpp
#include "Display.h"
```

Purpose: board-independent public display facade.

Main API:

```cpp
Display::begin();
Display::setBrightness(percent);
Display::brightness();
Display::width();
Display::height();
```

Example:

```cpp
Display::begin();
Display::setBrightness(90);

const uint16_t w = Display::width();
const uint16_t h = Display::height();
```

Use `Display` rather than calling `LCD_Init()` or backlight driver functions directly.

## Display_GFX

Header:

```cpp
#include "Display_GFX.h"
```

Purpose: primitive graphics and common RGB565 color constants.

Built-in colors:

```cpp
Color::Black
Color::White
Color::Red
Color::Green
Color::Blue
Color::Yellow
Color::Cyan
Color::Magenta
```

Main drawing functions include:

```cpp
Display_FillScreen(...)
Display_DrawPixel(...)
Display_DrawFastHLine(...)
Display_DrawFastVLine(...)
Display_DrawRect(...)
Display_FillRect(...)
Display_DrawLine(...)
Display_DrawTriangle(...)
Display_DrawCircle(...)
Display_DrawRoundRect(...)
Display_FillRoundRect(...)
```

Example:

```cpp
Display_FillScreen(Color::Black);

Display_DrawRect(
    10, 10,
    100, 40,
    Color::White);
```

## Display_Text

Header:

```cpp
#include "Display_Text.h"
```

Purpose: lightweight bitmap text.

Main API includes:

```cpp
Display_DrawChar(...)
Display_DrawText(...)
Display_DrawTextCentered(...)
Display_DrawTextRight(...)
Display_DrawTextWrapped(...)
Display_GetTextWidth(...)
Display_GetLineHeight(...)
Display_SetCursor(...)
Display_SetTextColor(...)
Display_SetTextScale(...)
Display_Print(...)
Display_Println(...)
```

Example:

```cpp
Display_DrawText(
    10,
    50,
    "Connected",
    Color::Green,
    Color::Black,
    1);
```

If using `Color::*`, also include:

```cpp
#include "Display_GFX.h"
```

## Display_Widgets

Header:

```cpp
#include "Display_Widgets.h"
```

The widget implementation is shared across supported display boards. Both
`demo_display_widgets` and `ttgo_display_widgets` compile the same demo source;
the demos adapt positions, spacing, and sizes through `Display::width()` and
`Display::height()` instead of maintaining TTGO-specific widgets.

Purpose: reusable application-independent UI components.

Current widgets include:

```text
Display_DrawIcon
Display_DrawIconLabel
Display_DrawPanel
Display_DrawLabel
Display_DrawValue
Display_DrawLabelValue
Display_DrawStatusIndicator
Display_DrawStatus
Display_DrawHeaderBar
Display_DrawFooterBar
Display_DrawBattery
Display_DrawProgressBar
Display_DrawSignalMeter
Display_DrawTableRow
Display_DrawTable
Display_ShowStatusScreen
```

Supporting types include:

```cpp
Display_StatusMarkerShape
Display_Icon
Display_TableRow
Display_StatusScreenData
```

### Header example

A standalone source file using `Color::*` and a widget should include both
the graphics and widget headers:

```cpp
#include "Display_GFX.h"
#include "Display_Widgets.h"

Display_DrawHeaderBar(
    "Network",
    Color::Blue,
    Color::White,
    Color::White,
    2,
    34);
```

### Table example

This example also uses `Display::width()`, so include `Display.h` in addition
to the graphics/widget headers:

```cpp
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

const Display_TableRow rows[] =
{
    {"IP", "10.0.0.15", Color::Cyan},
    {"MQTT", "Connected", Color::Green}
};

Display_DrawTable(
    8,
    48,
    Display::width() - 16,
    32,
    rows,
    sizeof(rows) / sizeof(rows[0]),
    Color::Black,
    Color::White,
    Color::White,
    Color::Cyan,
    1);
```

The interactive reference environments are:

```text
demo_display_widgets
ttgo_display_widgets
```

Physical TTGO testing verified that the shared widgets render correctly with
the resolution-aware compact demo layouts.

Their shared README is:

```text
src/demos/display_widgets/README.md
```

## Logger

Header:

```cpp
#include "Logger.h"
```

Initialize once:

```cpp
Logger::begin();
```

Macros:

```cpp
LOG("message");
LOGF("value=%d", value);

LOGW("warning");
LOGWF("warning value=%d", value);

LOGE("error");
LOGEF("error value=%d", value);
```

Current output uses `printf()`.

## NetworkService

Header:

```cpp
#include "NetworkService.h"
```

Register one or more networks before `begin()`:

```cpp
NetworkService::addNetwork(
    WIFI_SSID_1,
    WIFI_PASSWORD_1);

NetworkService::begin();
```

Call regularly:

```cpp
NetworkService::loop();
```

Status/data:

```cpp
NetworkService::isConnected();
NetworkService::ssid();
NetworkService::ipAddress();
NetworkService::rssi();
```

Example:

```cpp
if (NetworkService::isConnected())
{
    String ip = NetworkService::ipAddress();
    int32_t rssi = NetworkService::rssi();
}
```

## MQTTService

Header:

```cpp
#include "MQTTService.h"
```

Start:

```cpp
MQTTService::begin(
    "10.0.0.50",
    1883,
    "my-client-id");
```

Register one callback:

```cpp
MQTTService::setMessageCallback(
    handleMqttMessage);
```

Register topics:

```cpp
MQTTService::subscribe(
    "home/dashboard/example");
```

Call regularly:

```cpp
MQTTService::loop();
```

Other API:

```cpp
MQTTService::isConnected();
MQTTService::publish(topic, payload);
```

Subscriptions registered with the service are automatically re-subscribed when the broker connection is restored.

The callback signature is:

```cpp
void handleMqttMessage(
    const char *topic,
    const char *payload);
```

## TimeService

Header:

```cpp
#include "TimeService.h"
```

Start NTP and configure the local timezone after starting Wi-Fi:

```cpp
TimeService::begin(
    "PST8PDT,M3.2.0,M11.1.0");
```

`begin(...)` accepts a POSIX timezone string plus optional primary and
secondary NTP server names. It uses ESP32 `configTzTime(...)`, which applies
the timezone and starts background NTP synchronization. No `TimeService::loop()`
call is required.

Main API:

```cpp
TimeService::begin(timezone, ntpServer1, ntpServer2);
TimeService::isSynced();
TimeService::timeString();
TimeService::dateString();
TimeService::dateTimeString();
TimeService::now();
```

`isSynced()` reports whether the system clock has reached a valid synchronized
time. The formatting helpers return local 12-hour time, date, or combined
date/time strings and return visible placeholders while synchronization is
pending. `now()` returns the current `time_t` system-clock value.

The Home Dashboard initializes the service globally in `main.cpp`, and
`SystemStatusPage` reads it. The `demo_ntp` environment is the focused hardware
test for Wi-Fi + NTP + display output.

## Buttons

Header:

```cpp
#include "Buttons.h"
```

Initialize:

```cpp
Buttons_Begin();
```

Update on every loop:

```cpp
Buttons_Update();
```

Events:

```cpp
Button_WasPressed(Button::Boot)
Button_WasLongPressed(Button::Boot)
Button_WasVeryLongPressed(Button::Boot)
```

Current supported physical button:

```cpp
Button::Boot
```

On the Waveshare board it maps to GPIO 0, active LOW.

Important: in the current Home Dashboard, button initialization is intentionally performed after the other major startup subsystems. Preserve that order unless you have tested a change.

## RGB_Lamp

Header:

```cpp
#include "RGB_lamp.h"
```

Preferred framework API:

```cpp
RGBLamp::begin();
RGBLamp::setColor(red, green, blue);
RGBLamp::off();
```

Example:

```cpp
RGBLamp::begin();
RGBLamp::setColor(0, 64, 64);
```

Legacy demo helpers `Set_Color()` and `RGB_Lamp_Loop()` are retained for original/demo compatibility.

## SD_Card

Header:

```cpp
#include "SD_Card.h"
```

Initialize:

```cpp
SD_Init();
```

Common framework helpers:

```cpp
SD_IsMounted();
SD_FileExists(path);
SD_ReadTextFile(path, contents);
```

Raw-sector helpers:

```cpp
SD_SectorSize();
SD_SectorCount();
SD_ReadSector(buffer, sector);
SD_WriteSector(buffer, sector);
```

Legacy/demo helpers are also retained:

```cpp
Flash_test();
File_Search(...);
Folder_retrieval(...);
remove_file_extension(...);
```

Example text read:

```cpp
String contents;

if (SD_ReadTextFile(
        "/status.json",
        contents))
{
    // Parse or use contents.
}
```

See `SD_CARD_GUIDE.md`.

## PNG_Image

Header:

```cpp
#include "PNG_Image.h"
```

Purpose: decode a PNG from the mounted SD card and draw it at an X/Y coordinate.

API:

```cpp
bool PNGImage_Draw(
    const char *filePath,
    int16_t x = 0,
    int16_t y = 0);
```

Example:

```cpp
PNGImage_Draw(
    "/Network_80x80.png",
    8,
    48);
```

A 140×140 image can be horizontally centered on the 172-pixel Waveshare display at X=16:

```cpp
PNGImage_Draw(
    "/Sunset_140x140.png",
    16,
    48);
```

The renderer currently does not scale images. Prepare assets at the size you want to display.

The current maximum image width is tied to the 172-pixel line buffer used by this board implementation.

## ProjectVersion

Header:

```cpp
#include "ProjectVersion.h"
```

Available macros:

```cpp
PROJECT_NAME
PROJECT_VERSION
PROJECT_VERSION_MAJOR
PROJECT_VERSION_MINOR
PROJECT_VERSION_PATCH
```

Example:

```cpp
LOGF(
    "%s v%s",
    PROJECT_NAME,
    PROJECT_VERSION);
```

Version bumps are intended for meaningful project milestones, not every Git commit.

## Low-Level / Special-Purpose Libraries

### Display_ST7789

Low-level ST7789 driver. Normally do not include it from application/page code.

### Display_Boards

Compile-time board configuration. Normally consumed by the display layer rather than application code.

### Wireless

Legacy Wi-Fi/BLE test helpers retained for original demos. New applications should prefer `NetworkService`.

### LVGL_Driver / lvgl

Retained for LVGL/factory/demo functionality. The current Home Dashboard uses the lighter custom display stack instead.

### PNGdec

Third-party decoder consumed by `PNG_Image`. Application code should include `PNG_Image.h`, not `PNGdec.h`.
