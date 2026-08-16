# SD Card Files

This folder documents the files used by the current framework/application.

These files are examples for preparing a physical microSD card. The ESP32 does not read files directly from this repository folder.

## Current Home Dashboard Files

### `/status.json`

Used by:

`SDCardPage`

Example:

```json
{
  "name": "Storage Test",
  "location": "Desk",
  "battery": 83,
  "status": "Online"
}