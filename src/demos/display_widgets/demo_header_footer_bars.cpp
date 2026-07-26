/******************************************************************************
 * Demo:
 *     Header and Footer Bars
 *
 * Demonstrates:
 *     Display_DrawHeaderBar()
 *     Display_DrawFooterBar()
 *
 * Notes:
 *     Shows the default bars and a customizable dashboard content area.
 ******************************************************************************/

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawHeaderFooterBarsDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawHeaderBar(
        "DASHBOARD",
        Color::Blue,
        Color::Cyan,
        Color::White,
        2,
        34);

    Display_DrawPanel(
        10,
        55,
        LCD_WIDTH - 20,
        70,
        Color::Black,
        Color::Cyan,
        8);

    Display_DrawLabel(
        10,
        55,
        LCD_WIDTH - 20,
        70,
        "CONTENT AREA",
        Color::Cyan,
        Color::Black,
        2);

    Display_DrawPanel(
        10,
        145,
        LCD_WIDTH - 20,
        70,
        Color::Black,
        Color::White,
        8);

    Display_DrawLabel(
        10,
        145,
        LCD_WIDTH - 20,
        70,
        "REUSABLE BARS",
        Color::White,
        Color::Black,
        1);

    Display_DrawFooterBar(
        "BOOT: NEXT  HOLD: HOME",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        28);
}
