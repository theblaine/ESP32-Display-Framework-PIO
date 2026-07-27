/**
 * @file DisplayConfig.h
 * @brief Selects the active board display configuration.
 */

#pragma once

#if defined(DISPLAY_BOARD_WAVESHARE_147)

#include "Waveshare_ESP32S3_LCD_147/WaveshareDisplayConfig.h"
namespace DisplayConfig = WaveshareDisplayConfig;

#elif defined(DISPLAY_BOARD_TTGO_T_DISPLAY)

#include "TTGO_T_Display/TTGODisplayConfig.h"
namespace DisplayConfig = TTGODisplayConfig;

#else

#error "No display board selected. Define DISPLAY_BOARD_WAVESHARE_147 or DISPLAY_BOARD_TTGO_T_DISPLAY."

#endif