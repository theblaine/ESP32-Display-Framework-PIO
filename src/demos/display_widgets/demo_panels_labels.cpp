/******************************************************************************
 * Demo:
 *     Panels and Labels
 *
 * Demonstrates:
 *     Display_DrawPanel()
 *     Display_DrawLabel()
 *
 * Notes:
 *     Shows square and rounded panels with centered text.
 ******************************************************************************/

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawPanelsAndLabelsDemo()
{
    Display_FillScreen(Color::Black);

    // Square panel with centered label.
    Display_DrawPanel(
        10,
        10,
        Display::width() - 20,
        45,
        Color::Black,
        Color::White);

    Display_DrawLabel(
        10,
        10,
        Display::width() - 20,
        45,
        "RADIUS 0",
        Color::White,
        Color::Black,
        2);

    // Slightly rounded panel with centered label.
    Display_DrawPanel(
        10,
        70,
        Display::width() - 20,
        45,
        Color::Black,
        Color::Cyan,
        4);

    Display_DrawLabel(
        10,
        70,
        Display::width() - 20,
        45,
        "RADIUS 4",
        Color::Cyan,
        Color::Black,
        2);

    // Medium rounded panel with centered label.
    Display_DrawPanel(
        10,
        130,
        Display::width() - 20,
        45,
        Color::Black,
        Color::Yellow,
        8);

    Display_DrawLabel(
        10,
        130,
        Display::width() - 20,
        45,
        "RADIUS 8",
        Color::Yellow,
        Color::Black,
        2);

    // Heavily rounded panel with centered label.
    Display_DrawPanel(
        10,
        190,
        Display::width() - 20,
        45,
        Color::Green,
        Color::White,
        16);

    Display_DrawLabel(
        10,
        190,
        Display::width() - 20,
        45,
        "RADIUS 16",
        Color::White,
        Color::Green,
        2);
}
