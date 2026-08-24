# Sample SD Card Payload

Copy the contents of this directory to the root of a physical microSD card.
The ESP32 does not read this repository directory directly.

## Included files

| Repository file | Physical SD path | Used by |
|---|---|---|
| `message.txt` | `/message.txt` | `demo_sd` |
| `status.json` | `/status.json` | Home Dashboard `SDCardPage` |
| `status.example.json` | Not required | Editable backup/example for `status.json` |
| `Sunset_140x140.png` | `/Sunset_140x140.png` | Home Dashboard `ImagePage`; `demo_png` |
| `Network_80x80.png` | `/Network_80x80.png` | Home Dashboard `DeviceOverviewPage`; `demo_png` |
| `WaveshareImage1.png` | `/WaveshareImage1.png` | `demo_png` |
| `WaveshareImage2.png` | `/WaveshareImage2.png` | `demo_png` |
| `WaveshareImage4.png` | `/WaveshareImage4.png` | `demo_png` |
| `WaveshareImage6.png` | `/WaveshareImage6.png` | `demo_png` |

`status.json` is ready to copy as-is. `status.example.json` is intentionally
preserved but is not read by the firmware.

## PNG dimensions

`Sunset_140x140.png` is 140×140 pixels and `Network_80x80.png` is 80×80
pixels. Each `WaveshareImage*.png` sample is 172×320 pixels, matching the
primary Waveshare display resolution.

## Demo behavior

- `demo_sd` reads `/message.txt`, which is included.
- `demo_png` enumerates every lowercase `.png` file in the SD-card root; it
  does not require a particular filename. It cycles through all six included
  PNG files.
- `demo_sd_usb` expects no content file. It exposes the entire physical card as
  USB Mass Storage, including the sample files copied from this directory.
- The factory demo initializes and reports the SD card but does not open a
  named content file.

See `docs/SD_CARD_GUIDE.md` for setup and maintenance-mode details.
