# Getting Started

This guide takes a new user from a fresh repository checkout to a working display demo and then to the Home Dashboard.

## 1. Requirements

Recommended development environment:

- Windows 10/11, Linux, or macOS
- Visual Studio Code
- PlatformIO extension
- USB data cable
- supported ESP32 display board

Primary tested hardware:

- Waveshare ESP32-S3-LCD-1.47

The project uses the Arduino framework through PlatformIO.

## 2. Open the Repository

Open the repository root in VS Code. The root must contain:

```text
platformio.ini
src/
lib/
include/
docs/
```

Do not open only `src/` or an individual demo directory.

PlatformIO will create `.pio/` build output locally as needed.

## 3. Build a Display-Only Demo First

A good first verification is:

```text
demo_display_text
```

From a terminal in the repository root:

```powershell
pio run -e demo_display_text
```

Then upload:

```powershell
pio run -e demo_display_text -t upload
```

Open the serial monitor if needed:

```powershell
pio device monitor -e demo_display_text
```

The shared monitor speed is 115200 baud.

## 4. PlatformIO Environments

The default environment is:

```ini
[platformio]
default_envs = app_home_dashboard
```

Useful Waveshare environments:

```text
app_home_dashboard
demo_display_widgets
demo_display_console
demo_display_text
demo_display_graphics
demo_factory
demo_png
demo_wifi
demo_mqtt
demo_sd
demo_sd_usb
demo_rgb
```

Selected TTGO environments are documented in `BOARD_SUPPORT.md`.

## 5. Configure Wi-Fi for the Home Dashboard

The repository includes:

```text
include/secrets.example.h
```

Create a private copy:

```text
include/secrets.h
```

Example:

```cpp
#pragma once

#define WIFI_SSID_1     "your-wifi-name"
#define WIFI_PASSWORD_1 "your-wifi-password"

// Optional:
// #define WIFI_SSID_2     "SecondNetwork"
// #define WIFI_PASSWORD_2 "second-password"
//
// #define WIFI_SSID_3     "ThirdNetwork"
// #define WIFI_PASSWORD_3 "third-password"
```

`app_home_dashboard` directly includes `secrets.h`, so the application will not build without it.

Do not commit real credentials.

## 6. Build the Home Dashboard

```powershell
pio run -e app_home_dashboard
```

Upload:

```powershell
pio run -e app_home_dashboard -t upload
```

Monitor:

```powershell
pio device monitor -e app_home_dashboard
```

Normal Home Dashboard serial logging is provided by `Logger` through `printf()`.

## 7. BOOT and RESET Buttons on the Waveshare Board

With the USB connector at the top and the display facing you:

- top-right button: **BOOT**
- top-left button: **RESET**

In the normal Home Dashboard:

- short BOOT press: advances to the next configured dashboard page
- long BOOT press: detected/logged
- very-long BOOT press: detected/logged

The `Buttons` library uses GPIO 0 and active-low input.

### Entering ESP32-S3 Download Mode

Some native-USB firmware, especially `demo_sd_usb`, may require manual download mode.

1. Hold **BOOT** (top-right).
2. While holding BOOT, press and release **RESET** (top-left).
3. Release BOOT.
4. Upload from PlatformIO.

Windows may assign a different COM port in download mode. Do not depend permanently on a specific COM number.

## 8. Important Button Initialization Detail

In the current Home Dashboard, `Buttons_Begin()` is deliberately called near the end of `setup()`.

This preserves the working GPIO 0 configuration after other subsystems have initialized.

If button handling suddenly stops working after reorganizing startup code, restore the existing initialization order before debugging the Buttons library itself.

`Buttons_Update()` should run early and frequently in `loop()`.

## 9. SD Card

The Home Dashboard calls:

```cpp
SD_Init();
```

during setup.

The primary board uses SD_MMC. SD features include:

- existence checks
- text-file reading
- JSON data files at the application level
- raw-sector access
- PNG image loading
- a dedicated USB Mass Storage maintenance demo

See `SD_CARD_GUIDE.md`.

## 10. First Safe Modification

The simplest Home Dashboard customization is the rotation list in:

```text
src/apps/home_dashboard/main.cpp
```

Example:

```cpp
constexpr DashboardPage ROTATION_PAGES[] =
{
    DashboardPage::Network,
    DashboardPage::SDCard,
    DashboardPage::DeviceOverview,
};
```

Comment out pages you do not want in normal rotation.

The automatic page interval is controlled by:

```cpp
constexpr unsigned long PAGE_DURATION_MS = 10000;
```

## 11. Where to Go Next

- Want to draw? Read `LIBRARY_REFERENCE.md`.
- Want a new page? Read `CREATING_PAGES.md`.
- Want MQTT data? Read `MQTT-Protocol.md`.
- Want SD/PNG/Windows access? Read `SD_CARD_GUIDE.md`.
- Want another board? Read `BOARD_SUPPORT.md`.
