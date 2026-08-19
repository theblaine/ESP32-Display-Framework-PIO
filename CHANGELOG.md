# Changelog

All notable changes to the ESP32 Display Framework will be documented in this file.

This project follows Semantic Versioning (SemVer):

- **MAJOR** version when making incompatible API changes.
- **MINOR** version when adding functionality in a backward-compatible manner.
- **PATCH** version for backward-compatible bug fixes, documentation, and cleanup.

---

## [0.2.14] - 2026-08-18

### Added

- Physically validated TTGO T-Display environments for graphics, text, console, calibration, widgets, Wi-Fi, MQTT, and NTP/`TimeService`.

### Changed

- Expanded TTGO demo coverage for Wi-Fi, MQTT, and NTP.
- Refined resolution-aware demo and status-screen layouts while retaining the shared `Display` and `Display_Widgets` implementations.

## [0.2.13] - 2026-08-17

### Added

- TTGO T-Display support for the shared `display_widgets` demo through the `ttgo_display_widgets` environment.

### Changed

- Made widget-demo layouts resolution-aware using display geometry while retaining one shared `Display_Widgets` implementation for Waveshare and TTGO.

## [0.2.12] - 2026-08-17

### Added

- Reusable `TimeService` for background NTP synchronization and local date/time formatting.
- `demo_ntp` to verify Wi-Fi, NTP synchronization, and display output.

### Changed

- Integrated globally initialized date/time into the Home Dashboard `SystemStatusPage`.

## [0.2.0] - 2026-08-08

### Added

- Modular dashboard page architecture.
- Configurable dashboard operating modes (Rotate and Locked).
- Configurable page rotation list.
- Centralized project versioning (`ProjectVersion.h`).

### Changed

- Extracted dashboard pages from `main.cpp`.
- Simplified dashboard configuration.
- Improved project organization.
- Updated project documentation.

### Fixed

- Removed page-specific code from `main.cpp`.
- Improved ownership and separation of dashboard components.
