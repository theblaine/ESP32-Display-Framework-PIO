# SD Card Guide

## Overview

The Waveshare ESP32-S3-LCD-1.47 SD slot is supported through `SD_MMC`.

The framework uses the SD card for:

- application data/configuration files
- JSON-driven display data
- PNG image assets
- raw-sector access
- Windows USB Mass Storage maintenance mode

---

## SD Pin Mapping

Defined in:

```text
lib/SD_Card/SD_Card.h
```

| Signal | GPIO |
|---|---:|
| CLK | 14 |
| CMD | 15 |
| D0 | 16 |
| D1 | 18 |
| D2 | 17 |
| D3 | 21 |

---

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

---

## Mount Check

```cpp
if (SD_IsMounted())
{
    // Card available.
}
```

---

## File Existence

```cpp
if (SD_FileExists(
        "/status.json"))
{
    // File exists.
}
```

---

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

---

# Current Home Dashboard SD Files

The Home Dashboard currently uses or has tested several files stored on the
physical microSD card.

These files are stored in the **root directory of the SD card** unless
otherwise noted.

| File | Used By | Required | Purpose |
|---|---|---:|---|
| `/status.json` | `SDCardPage` | Yes, for that page | Structured JSON page data |
| `/Network_80x80.png` | `DeviceOverviewPage` | Yes, for that page | 80×80 network image |
| `/Sunset_140x140.png` | `ImagePage` | Yes, for that page image | Positioned PNG image |
| `/message.txt` | `demo_sd` | Yes, for that demo | Plain-text read test |

The repository contains an:

```text
sdcard/
```

directory that documents files intended for the physical SD card.

The repository directory is only a reference/template.

Files are **not automatically copied** from:

```text
sdcard/
```

to the physical microSD card during a PlatformIO build or upload.

---

## `/status.json`

Used by:

```text
SDCardPage
```

The page expects a JSON file named:

```text
/status.json
```

in the root of the physical SD card.

A ready-to-copy file and an editable example are provided as:

```text
sdcard/status.json
sdcard/status.example.json
```

Copy `sdcard/status.json` to the root of the physical SD card without
renaming it:

```text
status.json
```

Example contents:

```json
{
  "name": "Storage Test",
  "location": "Desk",
  "battery": 83,
  "status": "Online"
}
```

Current fields:

| Field | Type | Example |
|---|---|---|
| `name` | String | `"Storage Test"` |
| `location` | String | `"Desk"` |
| `battery` | Number | `83` |
| `status` | String | `"Online"` |

The current `SDCardPage`:

1. verifies the SD card is mounted
2. checks for `/status.json`
3. reads the file as text
4. parses the JSON with ArduinoJson
5. displays the values using framework widgets

Possible page-level errors include conditions such as:

```text
SD Missing
File Missing
Read Failed
JSON Error
```

Using JSON for application configuration or data is preferred when a file
contains multiple related values.

Plain text files remain supported by the framework.

---

## `/Network_80x80.png`

Used by:

```text
DeviceOverviewPage
```

Expected image size:

```text
80 × 80 pixels
```

Current location on the display:

```text
x = 8
y = 48
```

Current draw call:

```cpp
PNGImage_Draw(
    "/Network_80x80.png",
    8,
    48);
```

This image is part of the current Device Overview proof-of-concept page.

If the image is not present on the physical SD card, the remainder of the
page can still contain locally generated information, but the PNG itself
cannot be displayed.

The image is included as `sdcard/Network_80x80.png` and can be copied directly
to the physical SD-card root.

---

## `/Sunset_140x140.png`

Used by:

```text
ImagePage
```

The filename indicates an image size of:

```text
140 × 140 pixels
```

Current draw call:

```cpp
PNGImage_Draw(
    "/Sunset_140x140.png",
    16,
    48);
```

On a 172-pixel-wide display:

```text
(172 - 140) / 2 = 16
```

so an X position of `16` horizontally centers the image.

This file is included as `sdcard/Sunset_140x140.png` and can be copied directly
to the physical SD-card root.

---

## `/message.txt`

Used by:

```text
demo_sd
```

The demo mounts the card, reads this file from the card root, logs its
contents, and displays the contents on its status screen.

A ready-to-copy file is included as:

```text
sdcard/message.txt
```

---

# JSON Application Data

`SDCardPage` demonstrates structured configuration/data using:

```text
/status.json
```

Current example:

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

Using JSON for application configuration/data is preferred when the file may
grow beyond one value.

Plain text files remain supported by the framework.

---

# PNG Assets

The reusable renderer is:

```cpp
#include "PNG_Image.h"

PNGImage_Draw(
    "/image.png",
    x,
    y);
```

Examples proven in the Home Dashboard include:

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
- PNGdec decodes the PNG
- decoded scanlines are converted to RGB565
- scanlines are written directly to the ST7789 display
- X/Y positioning is supported
- automatic image scaling is not supported

Prepare PNG assets at the size they should appear on the display.

The sample payload includes the positioned `140×140` and `80×80` assets used
by the Home Dashboard, plus four full-screen 172×320 slideshow images.

---

# Repository SD Card Template

The repository contains:

```text
sdcard/
├── README.md
├── message.txt
├── Network_80x80.png
├── status.json
├── status.example.json
├── Sunset_140x140.png
├── WaveshareImage1.png
├── WaveshareImage2.png
├── WaveshareImage4.png
└── WaveshareImage6.png
```

This directory exists to help someone preparing a physical SD card understand
what files the application expects.

It should not be confused with the physical SD-card filesystem.

Copy the directory contents to the physical card root. For example:

```text
Repository:
sdcard/status.json

Physical SD card:
/status.json
```

`status.example.json` is preserved as an editable example but is not read by
the firmware.

The payload includes all files referenced by the Home Dashboard and SD demo.
It also includes these four full-screen samples for `demo_png`:

```text
/WaveshareImage1.png
/WaveshareImage2.png
/WaveshareImage4.png
/WaveshareImage6.png
```

---

# Raw Sector Access

The SD library also exposes:

```cpp
SD_SectorSize();
SD_SectorCount();
SD_ReadSector(buffer, sector);
SD_WriteSector(buffer, sector);
```

These functions exist primarily to support USB Mass Storage experiments and
utilities.

Normal application code should prefer filesystem-level helpers such as:

```cpp
SD_FileExists(...)
SD_ReadTextFile(...)
```

The current `demo_sd_usb` implementation talks directly to `SD_MMC` inside
its USB Mass Storage callbacks rather than using these framework wrappers.

---

# Windows USB Mass Storage Maintenance Mode

PlatformIO environment:

```text
demo_sd_usb
```

Purpose:

> Temporarily flash maintenance firmware that exposes the physical microSD
> card to Windows as a removable USB drive.

This avoids physically removing the SD card merely to edit configuration,
JSON, or image files.

USB Mass Storage functionality is intentionally **not integrated into the
Home Dashboard runtime**.

Keeping it separate avoids complicated ownership switching between:

- the ESP32 filesystem
- Windows USB Mass Storage

and keeps normal application USB behavior simple.

---

## Normal Mode

Firmware:

```text
app_home_dashboard
```

In this mode:

```text
ESP32 → owns the SD card
```

The Home Dashboard can read:

- JSON files
- text files
- PNG images
- other application data

Windows does not directly own the card.

---

# Enter Windows SD Maintenance Mode

## 1. Enter ESP32-S3 Download Mode

With the display facing you and the USB connector at the top:

- top-right button = BOOT
- top-left button = RESET

Procedure:

1. Hold **BOOT**.
2. While holding BOOT, press and release **RESET**.
3. Release BOOT.

The ESP32-S3 is now in download mode.

---

## 2. Upload the USB SD Firmware

Upload:

```text
demo_sd_usb
```

For example:

```powershell
pio run -e demo_sd_usb -t upload
```

After the upload completes, allow the board to reboot.

Windows should detect the SD card as a removable drive.

Windows may assign different:

- COM port numbers
- drive letters

on different computers or between operating modes.

Do not depend permanently on a specific COM port or drive letter.

---

## 3. Edit the SD Card

Once Windows mounts the card, files can be managed normally.

Examples:

```text
status.json
message.txt
Network_80x80.png
Sunset_140x140.png
```

You can:

- create files
- modify files
- replace PNG images
- delete files
- copy files to/from the card

---

# Safe Eject

Before removing power or switching firmware:

1. close any files or applications using the removable drive
2. use Windows **Eject**
3. confirm the drive disappears
4. unplug the ESP32 or switch firmware promptly

During development, Windows was observed to occasionally re-present the SD
media after a delay.

If the drive reappears:

1. eject it again
2. wait for it to disappear
3. unplug or switch firmware promptly

The safe rule is:

> Only unplug or change operating modes while the Windows SD drive is
> successfully ejected and absent.

---

# Return to the Home Dashboard

After editing the SD card:

1. safely eject the removable drive in Windows
2. hold the top-right **BOOT** button
3. tap and release the top-left **RESET** button
4. release BOOT
5. upload:

```text
app_home_dashboard
```

For example:

```powershell
pio run -e app_home_dashboard -t upload
```

After a successful upload, the board normally reboots directly into the Home
Dashboard.

A separate RESET press is usually unnecessary.

---

# Upload Behavior While USB Maintenance Firmware Is Running

Normal PlatformIO automatic upload may not reliably connect while
`demo_sd_usb` is running.

The documented reliable procedure is:

```text
BOOT + RESET
→ enter download mode
→ upload firmware
```

Do not document or depend on a fixed COM number because Windows can assign a
different COM port in normal runtime mode and ESP32-S3 download mode.

---

# SD Card Ownership Summary

Normal Home Dashboard:

```text
Physical SD card
       │
       ▼
     ESP32
```

USB maintenance mode:

```text
Physical SD card
       │
       ▼
USB Mass Storage
       │
       ▼
    Windows
```

Only one side should own/use the filesystem at a time.

That is why USB maintenance remains a separate firmware environment rather
than being dynamically enabled from inside the Home Dashboard.

---

# Demo References

Relevant PlatformIO environments:

```text
demo_sd
demo_png
demo_sd_usb
```

Use these demos before modifying the Home Dashboard when testing a new SD or
image capability.

- `demo_sd` — reads `/message.txt` from the card root
- `demo_png` — enumerates and renders lowercase `.png` files from the card root; no specific filename is required
- `demo_sd_usb` — exposes the complete SD card to Windows and requires no named content file

The factory demo also initializes the SD card and displays its capacity, but
does not expect a named file on the card.

---

# Related Documentation

```text
sdcard/README.md
docs/GETTING_STARTED.md
docs/LIBRARY_REFERENCE.md
docs/CREATING_PAGES.md
docs/BOARD_SUPPORT.md
src/demos/sd_usb/README.md
```
