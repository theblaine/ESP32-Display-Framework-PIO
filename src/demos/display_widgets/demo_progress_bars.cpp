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

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawProgressBarsDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawLabel(
        0,
        5,
        LCD_WIDTH,
        25,
        "PROGRESS BARS",
        Color::White,
        Color::Black,
        2);

    Display_DrawLabel(5, 45, 40, 24, "0%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 45, LCD_WIDTH - 60, 24, 0,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 90, 40, 24, "25%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 90, LCD_WIDTH - 60, 24, 25,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 135, 40, 24, "50%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 135, LCD_WIDTH - 60, 24, 50,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 180, 40, 24, "75%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 180, LCD_WIDTH - 60, 24, 75,
        Color::Black, Color::Green, Color::White, 8);

    Display_DrawLabel(5, 225, 40, 24, "100%", Color::White, Color::Black, 1);
    Display_DrawProgressBar(
        50, 225, LCD_WIDTH - 60, 24, 100,
        Color::Black, Color::Green, Color::White, 8);
}
