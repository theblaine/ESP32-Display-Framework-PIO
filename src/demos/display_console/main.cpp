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
    LOG(" ESP32 Display Console Demo");
    LOG("=====================================");

    LOG("Initializing display.");
    Display::begin();

    Display_FillScreen(
        Color::Black);

    const bool compactLayout =
        Display::width() < 160;

    Display_SetCursor(
        compactLayout ? 6 : 10,
        10);

    Display_SetTextColor(
        Color::White,
        Color::Black);

    if (compactLayout)
    {
        // Keep the complete title visible on narrow displays.
        Display_SetTextScale(1);

        Display_Println(
            "SERIAL STYLE");

        Display_Println();

        // Keep the main console output large and readable.
        Display_SetTextScale(2);

        Display_Println(
            "123-ABC");

        Display_Println(
            "TIME 8:42");

        Display_Println(
            "TEMP 72.4");

        Display_Println();
    }
    else
    {
        // Preserve the original larger-display presentation.
        Display_SetTextScale(2);

        Display_Println(
            "SERIAL STYLE");

        Display_Println();

        Display_Print(
            "123-");

        Display_Println(
            "ABC");

        Display_Print(
            "TIME: ");

        Display_Println(
            "8:42 PM");

        Display_Print(
            "TEMP: ");

        Display_Println(
            "72.4 F");

        Display_Println();
    }

    Display_SetTextColor(
        Color::Green,
        Color::Black);

    Display_Println(
        "ONLINE");
}

void loop()
{
}