# SD USB Mass Storage Demo

This demo exposes the Waveshare ESP32-S3-LCD-1.47 SD card to a
Windows computer as a USB Mass Storage device.

It is intended as a maintenance utility for reading and modifying
SD card files without physically removing the card from the ESP32.

## Purpose

The normal Home Dashboard owns and accesses the SD card directly.

When SD card files need to be edited from Windows, this demo can be
temporarily flashed to the ESP32. Windows then mounts the SD card as
a removable drive.

This keeps USB Mass Storage support isolated from the normal
Home Dashboard firmware.

## Entering Download Mode

The USB Mass Storage firmware does not reliably support normal
PlatformIO uploads while it is running.

To enter ESP32-S3 download mode:

1. Press and hold the top-right **BOOT** button.
2. While holding BOOT, press and release the top-left **RESET** button.
3. Release BOOT.
4. Windows should detect the ESP32 download-mode COM port.
5. Upload the desired PlatformIO environment.

COM port numbers are assigned by Windows and may change.

During testing, the board typically appeared as:

- COM5 while running the USB SD demo
- COM3 while in download mode

Do not depend on these specific COM numbers.

## Using the SD Card from Windows

1. Enter download mode using BOOT + RESET.
2. Upload the `demo_sd_usb` PlatformIO environment.
3. Allow the ESP32 to reboot.
4. Windows should mount the SD card as a removable drive.
5. Read, copy, or modify files normally.
6. Close all files and applications using the SD card.
7. Use Windows **Eject** on the removable drive.
8. Confirm the drive disappears before unplugging the ESP32.

### Important

Windows may present the SD card again approximately 45–60 seconds
after it has been ejected.

If the drive reappears, eject it again before unplugging the ESP32.

Only unplug the ESP32 while the Windows SD drive is successfully
ejected and absent.

## Returning to the Home Dashboard

1. Safely eject the SD card from Windows.
2. Enter download mode:
   - Hold BOOT.
   - Tap RESET.
   - Release BOOT.
3. Upload the `app_home_dashboard` PlatformIO environment.
4. Allow the ESP32 to reboot.

The Home Dashboard will again own the SD card and normal operation
will resume.

A separate RESET press is normally not required after a successful
upload.

## Verified Behavior

The following functionality has been tested successfully:

- SD card exposed to Windows as USB Mass Storage
- Existing SD files readable from Windows
- Files writable from Windows
- Changes persist on the SD card
- Windows eject works
- ESP32 download mode works using BOOT + RESET
- `demo_sd_usb` can be reflashed from download mode
- `app_home_dashboard` can be restored from download mode
- Home Dashboard reads files modified through Windows
- Normal dashboard display, buttons, Wi-Fi, MQTT, SD access, and
  Serial Monitor operation return after restoring the dashboard

## Architecture

The USB demo uses raw SD card sector access for USB Mass Storage.

The current `demo_sd_usb` implementation talks directly to `SD_MMC` and
uses `SD_MMC.readRAW()` / `SD_MMC.writeRAW()` inside the USB Mass Storage
callbacks.

The framework `SD_Card` library also exposes raw-sector wrapper functions for
future reusable code, but this demo does not currently call those wrappers.

The USB Mass Storage functionality intentionally remains a separate
demo/maintenance environment rather than being integrated into the
Home Dashboard runtime.