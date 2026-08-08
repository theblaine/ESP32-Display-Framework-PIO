#pragma once

#include <Arduino.h>

/**
 * Initializes the ST7789 controller and LCD backlight.
 */
void LCD_Init(void);

/**
 * Defines the active LCD drawing window.
 */
void LCD_SetCursor(
    uint16_t xStart,
    uint16_t yStart,
    uint16_t xEnd,
    uint16_t yEnd);

/**
 * Writes pixel data to a rectangular LCD window.
 */
void LCD_AddWindow(
    uint16_t xStart,
    uint16_t yStart,
    uint16_t xEnd,
    uint16_t yEnd,
    uint16_t* color);

/**
 * Sets the LCD backlight brightness.
 *
 * @param percent Brightness from 0 to 100 percent.
 */
void Display_SetBrightness(uint8_t percent);

/**
 * Returns the current LCD backlight brightness.
 *
 * @return Brightness from 0 to 100 percent.
 */
uint8_t Display_GetBrightness(void);