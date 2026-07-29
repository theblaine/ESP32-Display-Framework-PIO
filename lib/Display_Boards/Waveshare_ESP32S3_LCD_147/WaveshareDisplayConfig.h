#pragma once

#include <Arduino.h>

/**
 * @brief Hardware configuration for the Waveshare ESP32-S3-LCD-1.47 board.
 *
 * This file contains values that describe the physical board and LCD panel.
 * The ST7789 controller driver should consume these values rather than own
 * board-specific pin assignments and panel geometry itself.
 */
namespace WaveshareDisplayConfig
{
    // Visible panel geometry used by the framework.
    inline constexpr uint16_t Width = 172;
    inline constexpr uint16_t Height = 320;

    // SPI bus configuration.
    inline constexpr uint32_t SpiFrequency = 80000000;
    inline constexpr int8_t PinMiso = -1;
    inline constexpr int8_t PinMosi = 45;
    inline constexpr int8_t PinClock = 40;

    // ST7789 control pins.
    inline constexpr int8_t PinChipSelect = 42;
    inline constexpr int8_t PinDataCommand = 41;
    inline constexpr int8_t PinReset = 39;

    // Board-owned LCD backlight output.
    inline constexpr int8_t PinBacklight = 48;
    inline constexpr uint32_t BacklightPwmFrequency = 1000;
    inline constexpr uint8_t BacklightPwmResolution = 10;

    // Panel addressing and orientation.
    inline constexpr bool Horizontal = true;
    inline constexpr uint8_t MemoryAccessControl = 0xC0;
    inline constexpr uint16_t OffsetX = 34;
    inline constexpr uint16_t OffsetY = 0;
}
