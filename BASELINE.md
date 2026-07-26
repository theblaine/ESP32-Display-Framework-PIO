# Version 1.0 Refactor Baseline

This ZIP is the first conservative baseline for `ESP32-Display-Framework-PIO`.

## Changes from the stable template

- Removed generated `.pio` build output.
- Removed machine-specific generated VS Code configuration files.
- Kept `lib/Display_ST7789` as the single authoritative ST7789 implementation.
- Removed the older duplicate `Display_ST7789` files from `lib/Display`.
- Moved the graphics files from `lib/Display` into `lib/Display_GFX`.
- Updated the root README project name and graphics-library folder name.

No display APIs, pin assignments, drawing behavior, widget behavior, or PlatformIO environments were intentionally changed.

## First hardware verification

1. Extract the ZIP directly into the empty `ESP32-Display-Framework-PIO` folder.
2. Open that folder in VS Code.
3. Allow PlatformIO to initialize the project.
4. Select the `demo_display_text` environment.
5. Run **Clean**.
6. Run **Build**.
7. Connect the Waveshare ESP32-S3-LCD-1.47.
8. Run **Upload**.

Expected display output:

- `LEFT` near the top-left
- `CENTER` centered below it
- `RIGHT` aligned near the right edge
- `ESP32-S3` centered lower on the display
