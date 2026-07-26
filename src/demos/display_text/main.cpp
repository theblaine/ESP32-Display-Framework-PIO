#include <Arduino.h>

#include "Logger.h"
#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Text.h"

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 Display Text Demo");
    LOG("=====================================");

    LOG("Initializing display.");
    LCD_Init();

    Display_FillScreen(Color::Black);

    // Absolute positioning examples
    Display_DrawText(
        10,
        10,
        "LEFT",
        Color::White,
        Color::Black,
        2);

    Display_DrawTextCentered(
        LCD_WIDTH / 2,
        55,
        "CENTER",
        Color::Green,
        Color::Black,
        2);

    Display_DrawTextRight(
        LCD_WIDTH - 10,
        100,
        "RIGHT",
        Color::Yellow,
        Color::Black,
        2);

    Display_DrawTextCentered(
        LCD_WIDTH / 2,
        160,
        "ESP32-S3",
        Color::Cyan,
        Color::Black,
        3);
}

void loop()
{
}