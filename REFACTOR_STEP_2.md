# Refactor Step 2

## Goal

Make the ST7789 implementation consume the Waveshare board configuration directly.

## What changed

- Removed temporary aliases for SPI pins, LCD control pins, backlight settings,
  offsets, orientation, and SPI frequency from `Display_ST7789.h`.
- Updated `Display_ST7789.cpp` to read those values directly from
  `WaveshareDisplayConfig`.
- Retained only `LCD_WIDTH` and `LCD_HEIGHT` as temporary compatibility aliases
  because the current `Display_GFX` implementation still uses them.

## What did not change

- Public display functions
- Demo code
- ST7789 initialization command sequence
- Board pins or panel geometry
- Backlight behavior
- Expected on-screen output

## Test

Use the `demo_display_text` environment:

1. PlatformIO Clean
2. PlatformIO Build
3. PlatformIO Upload
4. Confirm LEFT, CENTER, RIGHT, and ESP32-S3 appear as before
