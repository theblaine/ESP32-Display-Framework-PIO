#pragma once

#include <Arduino.h>

/**
 * Draws a PNG file from the SD card.
 *
 * x and y specify the upper-left position of the image.
 *
 * Returns true when the PNG was opened and decoded successfully.
 */
bool PNGImage_Draw(
    const char *filePath,
    int16_t x = 0,
    int16_t y = 0);