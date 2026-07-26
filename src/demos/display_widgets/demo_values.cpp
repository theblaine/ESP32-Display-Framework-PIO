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

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawValuesDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawLabel(
        0,
        5,
        LCD_WIDTH,
        25,
        "VALUES",
        Color::White,
        Color::Black,
        2);

    // Numeric value.
    Display_DrawPanel(
        10, 40, LCD_WIDTH - 20, 55,
        Color::Black, Color::White, 8);

    Display_DrawValue(
        10, 40, LCD_WIDTH - 20, 55,
        "72.4", Color::White, Color::Black, 4);

    // Percentage value.
    Display_DrawPanel(
        10, 110, LCD_WIDTH - 20, 55,
        Color::Black, Color::Cyan, 8);

    Display_DrawValue(
        10, 110, LCD_WIDTH - 20, 55,
        "84%", Color::Cyan, Color::Black, 4);

    // Status value.
    Display_DrawPanel(
        10, 180, LCD_WIDTH - 20, 55,
        Color::Green, Color::White, 8);

    Display_DrawValue(
        10, 180, LCD_WIDTH - 20, 55,
        "ONLINE", Color::White, Color::Green, 3);
}
