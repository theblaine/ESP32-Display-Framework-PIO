/**
 * @file DisplayConfig.h
 * @brief Selects the active board display configuration.
 */

#pragma once

#include "Waveshare_ESP32S3_LCD_147/WaveshareDisplayConfig.h"

/**
 * @brief Board-neutral name for the active display configuration.
 *
 * This alias allows the graphics and application layers to consume display
 * geometry without depending directly on a display-controller driver.
 */
namespace DisplayConfig = WaveshareDisplayConfig;