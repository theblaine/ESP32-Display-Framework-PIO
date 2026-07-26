#include <Arduino.h>

#include "Logger.h"
#include "Display.h"
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
    Display::begin();

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
        Display::width() / 2,
        55,
        "CENTER",
        Color::Green,
        Color::Black,
        2);

    Display_DrawTextRight(
        Display::width() - 10,
        100,
        "RIGHT",
        Color::Yellow,
        Color::Black,
        2);

    Display_DrawTextCentered(
        Display::width() / 2,
        160,
        "ESP32-S3",
        Color::Cyan,
        Color::Black,
        3);
}

void loop()
{
}