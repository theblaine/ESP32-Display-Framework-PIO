/******************************************************************************
 * Demo:
 *     Values
 *
 * Demonstrates:
 *     Display_DrawValue()
 *
 * Related widgets:
 *     Display_DrawPanel()
 *     Display_DrawLabel()
 ******************************************************************************/

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawValuesDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t titleY = 5;
    const int16_t titleHeight = 25;
    const int16_t valueStartY = 40;
    const int16_t valueHeight = 55;
    const int16_t valueSpacing = compactLayout ? 65 : 70;

    Display_DrawLabel(
        0,
        titleY,
        Display::width(),
        titleHeight,
        "VALUES",
        Color::White,
        Color::Black,
        2);

    // Numeric value.
    Display_DrawPanel(
        10, valueStartY, Display::width() - 20, valueHeight,
        Color::Black, Color::White, 8);

    Display_DrawValue(
        10, valueStartY, Display::width() - 20, valueHeight,
        "72.4", Color::White, Color::Black, compactLayout ? 3 : 4);

    // Percentage value.
    Display_DrawPanel(
        10, valueStartY + valueSpacing, Display::width() - 20, valueHeight,
        Color::Black, Color::Cyan, 8);

    Display_DrawValue(
        10, valueStartY + valueSpacing, Display::width() - 20, valueHeight,
        "84%", Color::Cyan, Color::Black, compactLayout ? 3 : 4);

    // Status value.
    Display_DrawPanel(
        10, valueStartY + (valueSpacing * 2), Display::width() - 20, valueHeight,
        Color::Green, Color::White, 8);

    Display_DrawValue(
        10, valueStartY + (valueSpacing * 2), Display::width() - 20, valueHeight,
        "ONLINE", Color::White, Color::Green, compactLayout ? 2 : 3);
}
