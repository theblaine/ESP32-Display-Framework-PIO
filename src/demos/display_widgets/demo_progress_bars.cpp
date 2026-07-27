/******************************************************************************
 * Demo:
 *     Progress Bars
 *
 * Demonstrates:
 *     Display_DrawProgressBar()
 *
 * Related widgets:
 *     Display_DrawLabel()
 ******************************************************************************/

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawProgressBarsDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawLabel(
        0,
        5,
        Display::width(),
        25,
        "PROGRESS BARS",
        Color::White,
        Color::Black,
        2);

    Display_DrawLabel(5, 45, 40, 24, "0%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 45, Display::width() - 60, 24, 0,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 90, 40, 24, "25%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 90, Display::width() - 60, 24, 25,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 135, 40, 24, "50%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 135, Display::width() - 60, 24, 50,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 180, 40, 24, "75%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 180, Display::width() - 60, 24, 75,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 225, 40, 24, "100%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 225, Display::width() - 60, 24, 100,
        Color::Black, Color::Green, Color::White, 8);
}
