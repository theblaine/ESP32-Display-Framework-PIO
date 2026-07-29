#include <Arduino.h>

#include "Display_GFX.h"
#include "Display.h"
#include "Logger.h"

namespace
{
void drawCalibrationPattern()
{
    const int16_t width = Display::width();
    const int16_t height = Display::height();

    Display_FillScreen(Color::Black);

    // Left-side bands.
    Display_FillRect(
        0,
        0,
        8,
        height,
        Color::Red);

    Display_FillRect(
        8,
        0,
        8,
        height,
        Color::Blue);

    // Right-side bands.
    Display_FillRect(
        width - 16,
        0,
        8,
        height,
        Color::Blue);

    Display_FillRect(
        width - 8,
        0,
        8,
        height,
        Color::Red);
}
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" Display Calibration Demo");
    LOG("=====================================");

    Display::begin();

    LOGF(
        "Logical display size: %d x %d",
        Display::width(),
        Display::height());

    drawCalibrationPattern();
}

void loop()
{
}