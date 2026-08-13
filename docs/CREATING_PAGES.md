# Creating Home Dashboard Pages

This guide shows the current pattern for adding a modular page to `app_home_dashboard`.

The best real-world reference is `DeviceOverviewPage`, because it combines:

- SD-card PNG
- local network data
- MQTT connection state
- structured MQTT JSON
- text/widgets
- normal dashboard header/footer

## 1. Create the Page Files

Under:

```text
src/apps/home_dashboard/pages/
```

create:

```text
MyPage.h
MyPage.cpp
```

Minimal header:

```cpp
#pragma once

namespace MyPage
{
    void draw();
}
```

Minimal implementation:

```cpp
#include "MyPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"

namespace MyPage
{
    void draw()
    {
        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "My Page",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        LOG("Displayed My Page.");
    }
}
```

## 2. Include the Page in main.cpp

Add:

```cpp
#include "pages/MyPage.h"
```

## 3. Add an Enum Entry

In the `DashboardPage` enum:

```cpp
enum class DashboardPage : uint8_t
{
    // existing pages...
    MyPage,
};
```

## 4. Add It to Rotation

The current application uses an explicit array:

```cpp
constexpr DashboardPage ROTATION_PAGES[] =
{
    DashboardPage::Network,
    DashboardPage::MyPage,
};
```

This array is the normal page-selection mechanism.

You can:

- reorder pages
- comment pages out
- add new pages
- run only one page while developing

Automatic rotation currently occurs every 10 seconds:

```cpp
constexpr unsigned long PAGE_DURATION_MS =
    10000;
```

## 5. Add the Draw Case

In `drawCurrentPage()`:

```cpp
case DashboardPage::MyPage:
    MyPage::draw();
    drawStatusFooter();
    break;
```

The shared status footer is owned by `main.cpp`.

Do not draw the normal HA/FR/PH footer inside the page itself unless intentionally creating a special full-screen page.

## 6. Use the Correct Include for Each Feature

Common page includes:

```cpp
#include "Display.h"          // width/height, initialization facade
#include "Display_GFX.h"      // Color::* and primitive graphics
#include "Display_Text.h"     // Display_DrawText and alignment helpers
#include "Display_Widgets.h"  // header, tables, status, progress, signal, etc.
#include "Logger.h"
```

Optional:

```cpp
#include "NetworkService.h"
#include "MQTTService.h"
#include "SD_Card.h"
#include "PNG_Image.h"
#include "RGB_lamp.h"
#include <ArduinoJson.h>
```

See `LIBRARY_REFERENCE.md` for the full include map.

## 7. Receiving MQTT Data

If a page needs MQTT, give the page a handler.

Header:

```cpp
namespace MyPage
{
    void draw();

    void handleMessage(
        const char *payload);
}
```

Implementation example:

```cpp
#include <ArduinoJson.h>

namespace
{
    String g_status = "Unknown";
}

namespace MyPage
{
    void handleMessage(
        const char *payload)
    {
        if (payload == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                payload);

        if (error)
        {
            LOGE("MyPage JSON parse failed.");
            return;
        }

        g_status =
            document["status"] |
            "Unknown";
    }
}
```

## 8. Define the MQTT Topic

In `main.cpp`, alongside the other topic constants:

```cpp
constexpr const char *MY_PAGE_TOPIC =
    "home/dashboard/mypage";
```

## 9. Subscribe in setup()

```cpp
MQTTService::subscribe(
    MY_PAGE_TOPIC);
```

Topics may be registered before the broker is connected. `MQTTService` re-subscribes after reconnects.

## 10. Route the Message

The Home Dashboard intentionally uses one central MQTT callback.

Add:

```cpp
if (strcmp(
        topic,
        MY_PAGE_TOPIC) == 0)
{
    MyPage::handleMessage(
        payload);

    if (g_currentPage ==
        DashboardPage::MyPage)
    {
        MyPage::draw();
        drawStatusFooter();
    }

    return;
}
```

Why check the active page?

- the page always stores the new data
- if the page is visible, it updates immediately
- if another page is visible, the screen is not unnecessarily redrawn
- when rotation later reaches the page, it shows the latest stored values

## 11. Local Data and MQTT Can Coexist

A page does not need to get everything from MQTT.

`DeviceOverviewPage` demonstrates this pattern:

```text
PNG from SD
+
IP/RSSI from NetworkService
+
MQTT connection from MQTTService
+
location/status from MQTT JSON
```

Use the source that naturally owns the data.

## 12. PNG in a Page

Example:

```cpp
#include "PNG_Image.h"

PNGImage_Draw(
    "/Network_80x80.png",
    8,
    48);
```

PNG assets are not scaled. Size them appropriately before placing them on the SD card.

## 13. Signal Meter Pattern

`Display_DrawSignalMeter()` accepts a generic level. Page-specific code should convert its own source data to a level.

The current Wi-Fi thresholds used by `NetworkPage` and `DeviceOverviewPage` are:

```text
RSSI >= -55 dBm  → 4 bars
RSSI >= -65 dBm  → 3 bars
RSSI >= -75 dBm  → 2 bars
RSSI >= -85 dBm  → 1 bar
otherwise        → 0 bars
```

The widget itself remains generic.

## 14. Page-Specific Helpers vs. Framework Widgets

Not every repeated-looking shape needs to become a framework widget.

`SystemMonitorPage` has a page-local helper that composes:

- text
- percentage
- progress bar

That helper stays page-specific because it is simply a layout composition rather than a generally needed widget.

Promote something into `Display_Widgets` when it is genuinely reusable across multiple applications/pages.

## 15. Shared Footer

The current footer shows HA, FR, and PH data-received state.

`main.cpp` owns it because it is application-wide UI.

A new page normally calls only its own `draw()`. `drawCurrentPage()` and visible MQTT redraw blocks add the shared footer afterward.

## 16. Button Navigation

Current Home Dashboard behavior:

- short BOOT press: advance page immediately
- page timer resets after manual advance
- long/very-long events are available for future application actions

Avoid direct GPIO reads from a page. Use `Buttons`.

## 17. Page Checklist

Before committing a new page:

- [ ] `.h` and `.cpp` created
- [ ] page header included by `main.cpp`
- [ ] enum entry added
- [ ] rotation entry added if desired
- [ ] `drawCurrentPage()` case added
- [ ] MQTT topic constant added if needed
- [ ] MQTT subscription added if needed
- [ ] MQTT route added if needed
- [ ] page builds
- [ ] page displays correctly
- [ ] off-screen MQTT updates are retained if applicable
- [ ] visible MQTT updates redraw immediately if applicable
- [ ] shared footer still fits
