# SD Card Guide

## Overview

The Waveshare ESP32-S3-LCD-1.47 SD slot is supported through `SD_MMC`.

The framework uses the SD card for:

- application data/configuration files
- JSON-driven display data
- PNG image assets
- raw-sector access
- Windows USB Mass Storage maintenance mode

## SD Pin Mapping

Defined in `lib/SD_Card/SD_Card.h`:

| Signal | GPIO |
|---|---:|
| CLK | 14 |
| CMD | 15 |
| D0 | 16 |
| D1 | 18 |
| D2 | 17 |
| D3 | 21 |

## Initialization

Include:

```cpp
#include "SD_Card.h"
```

Initialize once:

```cpp
SD_Init();
```

The current Home Dashboard does this during `setup()`.

## Mount Check

```cpp
if (SD_IsMounted())
{
    // Card available.
}
```

## File Existence

```cpp
if (SD_FileExists(
        "/status.json"))
{
    // File exists.
}
```

## Read a Text File

```cpp
String contents;

if (SD_ReadTextFile(
        "/status.json",
        contents))
{
    // contents now holds the complete file.
}
```

Despite the helper name, this can be used to load JSON text before parsing it.

## JSON Application Data

`SDCardPage` demonstrates structured configuration/data using:

```text
/status.json
```

Current fields:

```json
{
  "name": "Storage Test",
  "location": "Desk",
  "battery": 83,
  "status": "Online"
}
```

The page:

1. verifies the SD card is mounted
2. checks for `/status.json`
3. reads it as text
4. parses it with ArduinoJson
5. displays the values using table, battery, and status widgets

Using JSON for application configuration/data is preferred when the file may grow beyond one value.

Plain text files remain supported by the framework.

## PNG Assets

The reusable renderer is:

```cpp
#include "PNG_Image.h"

PNGImage_Draw(
    "/image.png",
    x,
    y);
```

Examples proven in the Home Dashboard:

```cpp
PNGImage_Draw(
    "/Sunset_140x140.png",
    16,
    48);
```

and:

```cpp
PNGImage_Draw(
    "/Network_80x80.png",
    8,
    48);
```

Current PNG behavior:

- file lives on the mounted SD card
- PNGdec decodes scanlines
- scanlines are converted to RGB565
- scanlines are drawn directly to the ST7789
- X/Y positioning is supported
- automatic scaling is not supported

Prepare images at the intended display size.

Full-screen 172×320 and positioned 140×140/80×80 assets have been tested.

## Raw Sector Access

The SD library also exposes:

```cpp
SD_SectorSize();
SD_SectorCount();
SD_ReadSector(buffer, sector);
SD_WriteSector(buffer, sector);
```

These exist primarily to support USB Mass Storage experiments/utilities.

Normal application code should prefer filesystem-level helpers.

## Windows USB Mass Storage Maintenance Mode

Environment:

```text
demo_sd_usb
```

Purpose:

> Temporarily flash a maintenance firmware that exposes the physical SD card to Windows like a removable USB drive.

This avoids physically removing the SD card merely to edit configuration or image files.

The USB maintenance mode is intentionally **not integrated into the Home Dashboard runtime**.

That keeps normal application USB behavior simple and isolates native USB Mass Storage behavior in a dedicated utility.

### Normal Mode

Firmware:

```text
app_home_dashboard
```

The ESP32 owns and accesses the SD card.

### Enter Windows SD Maintenance Mode

1. Put the ESP32-S3 in download mode:
   - hold top-right BOOT
   - tap top-left RESET
   - release BOOT
2. Upload:
   ```text
   demo_sd_usb
   ```
3. Allow the board to reboot.
4. Windows should mount the SD card as a removable drive.
5. Read/write files normally.

Windows may assign different COM and drive letters on different systems.

### Safe Eject

Before removing power or switching firmware:

1. close files/applications using the removable drive
2. use Windows **Eject**
3. confirm the drive disappears
4. unplug or switch firmware promptly

Observed during development: Windows may re-present the SD media after a delay. If the drive reappears, eject it again before unplugging.

Safe rule:

> Only unplug while the Windows SD drive is successfully ejected and absent.

### Return to the Home Dashboard

1. safely eject the Windows SD drive
2. enter download mode again:
   - hold BOOT
   - tap RESET
   - release BOOT
3. upload:
   ```text
   app_home_dashboard
   ```
4. the board normally reboots directly into the dashboard

A separate RESET press is usually unnecessary after a successful upload.

### Upload Behavior in USB Maintenance Firmware

Normal PlatformIO auto-upload may not reliably connect while `demo_sd_usb` is running.

The BOOT+RESET download-mode sequence is the documented reliable procedure.

Do not document a fixed COM number because Windows can assign different values.

## Demo References

```text
demo_sd
demo_png
demo_sd_usb
```

Use these before modifying the Home Dashboard when testing a new SD/image capability.
