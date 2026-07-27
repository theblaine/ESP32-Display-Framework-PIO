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
    LOG(" ESP32-S3 Display Console Demo");
    LOG("=====================================");

    LOG("Initializing display.");
    Display::begin();

    Display_FillScreen(Color::Black);

    Display_SetCursor(10, 10);

    Display_SetTextColor(
        Color::White,
        Color::Black);

    Display_SetTextScale(2);

    Display_Println("SERIAL STYLE");
    Display_Println();

    Display_Print("123-");
    Display_Println("ABC");

    Display_Print("TIME: ");
    Display_Println("8:42 PM");

    Display_Print("TEMP: ");
    Display_Println("72.4 F");

    Display_Println();

    Display_SetTextColor(
        Color::Green,
        Color::Black);

    Display_Println("ONLINE");
}

void loop()
{
}