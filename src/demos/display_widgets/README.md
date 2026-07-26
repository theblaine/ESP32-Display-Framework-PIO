# Display Widgets Demo

This PlatformIO environment demonstrates every reusable component currently provided by `Display_Widgets`.

Build environment:

```text
demo_display_widgets
```

Build and upload from a terminal:

```bash
pio run -e demo_display_widgets
pio run -e demo_display_widgets -t upload
pio device monitor -b 115200
```

## BOOT Button Controls

- **Short press:** advance to the next demo page
- **Long press and release:** return to the configured startup page
- **Hold for three seconds:** cycle brightness through 100%, 75%, 50%, and 25%
- **RESET:** restart the board and display the startup page

The reusable button implementation is located in:

```text
lib/Buttons/Buttons.h
lib/Buttons/Buttons.cpp
```

## Selecting the Startup Page

Open `main.cpp` and leave exactly one `StartupDemoPage` declaration uncommented:

```cpp
constexpr WidgetDemoPage StartupDemoPage =
    WidgetDemoPage::StatusIndicators;
```

Available startup pages:

```cpp
WidgetDemoPage::PanelsAndLabels
WidgetDemoPage::ProgressBars
WidgetDemoPage::Values
WidgetDemoPage::LabelValues
WidgetDemoPage::StatusIndicators
WidgetDemoPage::HeaderFooterBars
WidgetDemoPage::IconLabels
WidgetDemoPage::Batteries
WidgetDemoPage::Tables
```

## Source Files

| File | Demonstrates |
|---|---|
| `main.cpp` | Display initialization, navigation, startup-page selection, and brightness control |
| `WidgetDemoPages.h` | Page enumeration and demo function declarations |
| `demo_panels_labels.cpp` | Panels and centered labels |
| `demo_progress_bars.cpp` | Progress bars |
| `demo_values.cpp` | Large standalone values |
| `demo_label_values.cpp` | Dashboard-style label/value cards |
| `demo_status_indicators.cpp` | Circle, square, and rounded-square status markers |
| `demo_header_footer_bars.cpp` | Full-width dashboard header and footer bars |
| `demo_icon_labels.cpp` | Built-in icons and icon/label combinations |
| `demo_batteries.cpp` | Battery level indicators |
| `demo_tables.cpp` | Label/value tables with borders, dividers, and colored values |

## Adding Another Widget Demo Page

1. Add a new value to `WidgetDemoPage` in `WidgetDemoPages.h`.
2. Declare the new drawing function in `WidgetDemoPages.h`.
3. Create a matching `demo_*.cpp` source file.
4. Add the page to `DrawCurrentDemo()` in `main.cpp`.
5. Add the page to `NextDemoPage()` so navigation reaches it.
6. Add an optional startup-page line near the top of `main.cpp`.

The demo pages intentionally double as examples and visual regression tests for the reusable widget library.
