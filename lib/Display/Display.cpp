/**
 * @file Display.cpp
 * @brief Implementation of the public display facade.
 */

#include "Display.h"

#include "DisplayConfig.h"
#include "Display_ST7789.h"

namespace Display
{
    void begin()
    {
        LCD_Init();
    }

    void setBrightness(std::uint8_t percent)
    {
        if (percent > 100)
        {
            percent = 100;
        }

        Display_SetBrightness(percent);
    }

    std::uint16_t width()
    {
        return DisplayConfig::Width;
    }

    std::uint16_t height()
    {
        return DisplayConfig::Height;
    }
}