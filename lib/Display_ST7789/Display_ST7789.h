#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "Waveshare_ESP32S3_LCD_147/WaveshareDisplayConfig.h"

/**
 * @brief Temporary geometry aliases used by the existing graphics stack.
 *
 * Only these two aliases remain because Display_GFX still uses them. The
 * ST7789 implementation now reads all board-specific values directly from
 * WaveshareDisplayConfig. A later refactor will remove these final aliases
 * when Display_GFX is separated from the concrete driver.
 */
#define LCD_WIDTH  WaveshareDisplayConfig::Width
#define LCD_HEIGHT WaveshareDisplayConfig::Height

void LCD_Init(void);
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t* color);

/**
 * Initializes PWM control for the LCD backlight.
 *
 * Normally called automatically by LCD_Init().
 */
void Backlight_Init(void);

/**
 * Legacy backlight API retained for compatibility.
 *
 * @param light Brightness from 0 to 100 percent.
 */
void Set_Backlight(uint8_t light);

/**
 * Sets the LCD backlight brightness.
 *
 * @param percent Brightness from 0 to 100 percent.
 */
void Display_SetBrightness(uint8_t percent);

/**
 * Returns the current LCD backlight brightness percentage.
 *
 * @return Brightness from 0 to 100 percent.
 */
uint8_t Display_GetBrightness();
