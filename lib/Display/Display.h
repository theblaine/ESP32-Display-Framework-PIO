/**
 * @file Display.h
 * @brief Public application-facing interface for the display framework.
 */

#pragma once

#include <cstdint>

/**
 * @brief High-level display interface.
 *
 * Application code should use this namespace instead of directly initializing
 * or configuring the underlying display driver.
 */
namespace Display
{
    /**
     * @brief Initializes the display hardware.
     */
    void begin();

    /**
     * @brief Sets the display backlight brightness.
     *
     * @param percent Brightness percentage from 0 to 100.
     */
    void setBrightness(std::uint8_t percent);

    /**
     * @brief Returns the current display backlight brightness.
     *
     * @return Brightness percentage from 0 to 100.
     */
    std::uint8_t brightness();

    /**
     * @brief Returns the logical display width.
     *
     * @return Display width in pixels.
     */
    std::uint16_t width();

    /**
     * @brief Returns the logical display height.
     *
     * @return Display height in pixels.
     */
    std::uint16_t height();
}