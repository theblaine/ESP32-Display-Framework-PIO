#pragma once

#include <Arduino.h>

/**
 * @brief Hardware configuration for the TTGO T-Display V1.1 board.
 *
 * This file contains values that describe the physical board and LCD panel.
 * The ST7789 controller driver should consume these values rather than own
 * board-specific pin assignments and panel geometry itself.
 */
namespace TTGODisplayConfig
{
    // Visible panel geometry used by the framework.
    inline constexpr uint16_t Width = 135;
    inline constexpr uint16_t Height = 240;

    // SPI bus configuration.
    inline constexpr uint32_t SpiFrequency = 20000000;
    inline constexpr int8_t PinMiso = -1;
    inline constexpr int8_t PinMosi = 19;
    inline constexpr int8_t PinClock = 18;

    // ST7789 control pins.
    inline constexpr int8_t PinChipSelect = 5;
    inline constexpr int8_t PinDataCommand = 16;
    inline constexpr int8_t PinReset = 23;

    // Board-owned LCD backlight output.
    inline constexpr int8_t PinBacklight = 4;
    inline constexpr uint32_t BacklightPwmFrequency = 1000;
    inline constexpr uint8_t BacklightPwmResolution = 10;

    // Panel addressing and orientation.
    inline constexpr bool Horizontal = true;
    inline constexpr uint16_t OffsetX = 52;
    inline constexpr uint16_t OffsetY = 40;
}