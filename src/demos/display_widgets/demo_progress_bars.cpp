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

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawProgressBarsDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t titleY = 5;
    const int16_t titleHeight = 25;
    const int16_t rowStartY = compactLayout ? 42 : 45;
    const int16_t rowHeight = 24;
    const int16_t rowSpacing = compactLayout ? 39 : 45;
    const int16_t labelWidth = 40;
    const int16_t barX = 50;

    Display_DrawLabel(
        0,
        titleY,
        Display::width(),
        titleHeight,
        "PROGRESS BARS",
        Color::White,
        Color::Black,
        compactLayout ? 1 : 2);

    Display_DrawLabel(5, rowStartY, labelWidth, rowHeight, "0%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        barX, rowStartY, Display::width() - barX - 8, rowHeight, 0,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, rowStartY + rowSpacing, labelWidth, rowHeight, "25%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        barX, rowStartY + rowSpacing, Display::width() - barX - 8, rowHeight, 25,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, rowStartY + (rowSpacing * 2), labelWidth, rowHeight, "50%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        barX, rowStartY + (rowSpacing * 2), Display::width() - barX - 8, rowHeight, 50,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, rowStartY + (rowSpacing * 3), labelWidth, rowHeight, "75%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        barX, rowStartY + (rowSpacing * 3), Display::width() - barX - 8, rowHeight, 75,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, rowStartY + (rowSpacing * 4), labelWidth, rowHeight, "100%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        barX, rowStartY + (rowSpacing * 4), Display::width() - barX - 8, rowHeight, 100,
        Color::Black, Color::Green, Color::White, 8);
}
