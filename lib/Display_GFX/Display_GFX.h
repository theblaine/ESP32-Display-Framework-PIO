#pragma once

#include <Arduino.h>

namespace Color
{
    constexpr uint16_t Black = 0x0000;
    constexpr uint16_t White = 0xFFFF;
    constexpr uint16_t Red = 0xF800;
    constexpr uint16_t Green = 0x07E0;
    constexpr uint16_t Blue = 0x001F;
    constexpr uint16_t Yellow = 0xFFE0;
    constexpr uint16_t Cyan = 0x07FF;
    constexpr uint16_t Magenta = 0xF81F;
}

void Display_FillScreen(uint16_t color);

void Display_DrawPixel(
    int16_t x,
    int16_t y,
    uint16_t color);

void Display_DrawFastHLine(
    int16_t x,
    int16_t y,
    int16_t width,
    uint16_t color);

void Display_DrawFastVLine(
    int16_t x,
    int16_t y,
    int16_t height,
    uint16_t color);

void Display_DrawRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color);

void Display_FillRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color);

void Display_DrawLine(
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1,
    uint16_t color);

void Display_DrawTriangle(
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1,
    int16_t x2,
    int16_t y2,
    uint16_t color);

void Display_DrawCircle(
    int16_t centerX,
    int16_t centerY,
    int16_t radius,
    uint16_t color);


/**
 * Draws the outline of a rectangle with rounded corners.
 *
 * Set radius to 0 to draw a standard rectangle.
 */
void Display_DrawRoundRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    int16_t radius,
    uint16_t color);

/**
 * Draws a filled rectangle with rounded corners.
 *
 * Set radius to 0 to draw a standard filled rectangle.
 */
void Display_FillRoundRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    int16_t radius,
    uint16_t color);