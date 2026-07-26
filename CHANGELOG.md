# Changelog

## Refactor Step 2

- Updated the ST7789 implementation to consume Waveshare board settings directly.
- Removed temporary aliases for pins, SPI speed, offsets, orientation, and PWM settings.
- Retained only temporary display-width and display-height aliases for Display_GFX compatibility.
- No intended runtime or API changes.

## 1.0.0

Stable release of the Waveshare ESP32-S3-LCD-1.47 PlatformIO template.

### Added

- Layered ST7789 display, graphics, text, and widget libraries
- Positioned, centered, and right-aligned text rendering
- Cursor-based `Display_Print()` and `Display_Println()` support
- Panels, labels, values, label/value cards, and progress bars
- Status indicators with multiple marker shapes
- Header and footer bars
- Built-in icons and icon/label widgets
- Battery indicators
- Label/value table widget
- Reusable BOOT-button handling for short, long, and very-long presses
- Runtime LCD brightness control
- Focused PlatformIO demo environments

### Fixed

- Preserved the table widget's bottom border by drawing the outer border after row content
- Corrected backlight percentage mapping to the full 10-bit PWM range

### Notes

This release marks the stable completion of the Waveshare-specific template. Multi-board work continues in the separate `ESP32-Display-Framework-PIO` project.

## Refactor Step 1

- Added an explicit Waveshare ESP32-S3-LCD-1.47 board configuration.
- Moved display geometry, pins, SPI frequency, offsets, orientation, and
  backlight PWM settings into `WaveshareDisplayConfig.h`.
- Retained the existing ST7789 public API and temporary compatibility names.
- No intentional runtime behavior changes.
