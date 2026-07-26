# Refactor Step 1: Waveshare Board Configuration

This step moves Waveshare-specific hardware values into one explicit board
configuration file:

```text
lib/Display_Boards/Waveshare_ESP32S3_LCD_147/WaveshareDisplayConfig.h
```

The following values now live there:

- Display width and height
- SPI frequency and pins
- LCD control pins
- Backlight pin and PWM settings
- Panel offsets
- Orientation

The existing names such as `LCD_WIDTH`, `SPIFreq`, and
`EXAMPLE_PIN_NUM_LCD_CS` remain as temporary compatibility aliases in
`Display_ST7789.h`. This means the graphics, text, widgets, and demos do not
need to change yet.

## Expected behavior

There should be no visible behavior change from the baseline.

## Test

Use the default environment:

```text
demo_display_text
```

Then:

1. PlatformIO: Clean
2. PlatformIO: Build
3. PlatformIO: Upload
4. Confirm LEFT, CENTER, RIGHT, and ESP32-S3 appear as before


## Step 1 packaging correction

The board configuration include uses the path relative to the PlatformIO library root:

```cpp
#include "Waveshare_ESP32S3_LCD_147/WaveshareDisplayConfig.h"
```
