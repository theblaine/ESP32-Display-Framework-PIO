/******************************************************************************
 * Demo:
 *     Label and Value Cards
 *
 * Demonstrates:
 *     Display_DrawLabelValue()
 *
 * Related widgets:
 *     Display_DrawPanel()
 *     Display_DrawLabel()
 *     Display_DrawValue()
 ******************************************************************************/

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawLabelValuesDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawLabel(
        0,
        5,
        Display::width(),
        25,
        "LABEL + VALUE",
        Color::White,
        Color::Black,
        2);

    // Temperature card.
    Display_DrawLabelValue(
        10,
        40,
        Display::width() - 20,
        60,
        "TEMPERATURE",
        "72.4 F",
        Color::Black,
        Color::White,
        Color::Cyan,
        Color::White,
        1,
        3,
        8);

    // Humidity card.
    Display_DrawLabelValue(
        10,
        110,
        Display::width() - 20,
        60,
        "HUMIDITY",
        "48%",
        Color::Black,
        Color::Cyan,
        Color::Cyan,
        Color::White,
        1,
        3,
        8);

    // Network status card.
    Display_DrawLabelValue(
        10,
        180,
        Display::width() - 20,
        60,
        "NETWORK",
        "ONLINE",
        Color::Green,
        Color::White,
        Color::White,
        Color::White,
        1,
        3,
        8);
}
