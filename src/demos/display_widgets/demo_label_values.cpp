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

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawLabelValuesDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t titleY = 5;
    const int16_t titleHeight = 25;
    const int16_t cardStartY = compactLayout ? 38 : 40;
    const int16_t cardHeight = 60;
    const int16_t cardSpacing = compactLayout ? 65 : 70;

    Display_DrawLabel(
        0,
        titleY,
        Display::width(),
        titleHeight,
        "LABEL + VALUE",
        Color::White,
        Color::Black,
        2);

    // Temperature card.
    Display_DrawLabelValue(
        10,
        cardStartY,
        Display::width() - 20,
        cardHeight,
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
        cardStartY + cardSpacing,
        Display::width() - 20,
        cardHeight,
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
        cardStartY + (cardSpacing * 2),
        Display::width() - 20,
        cardHeight,
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
