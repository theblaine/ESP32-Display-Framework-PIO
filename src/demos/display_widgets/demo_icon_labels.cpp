/******************************************************************************
 * Demo:
 *     Icon + Label
 *
 * Demonstrates:
 *     Display_DrawIcon()
 *     Display_DrawIconLabel()
 *
 * Related widgets:
 *     Display_DrawHeaderBar()
 *     Display_DrawFooterBar()
 *     Display_DrawPanel()
 ******************************************************************************/

#include "Display_ST7789.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawIconLabelsDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawHeaderBar(
        "ICON + LABEL",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        30);

    struct DemoRow
    {
        int16_t y;
        Display_Icon icon;
        const char* text;
        uint16_t color;
    };

    constexpr DemoRow Rows[] = {
        {42,  Display_Icon::Home,    "HOME",    Color::Cyan},
        {91,  Display_Icon::Info,    "INFO",    Color::Blue},
        {140, Display_Icon::Check,   "READY",   Color::Green},
        {189, Display_Icon::Warning, "WARNING", Color::Yellow},
        {238, Display_Icon::Power,   "POWER",   Color::Red}
    };

    for (const DemoRow& row : Rows)
    {
        Display_DrawPanel(
            10,
            row.y,
            Display::width() - 20,
            38,
            Color::Black,
            row.color,
            7);

        Display_DrawIconLabel(
            13,
            row.y + 3,
            Display::width() - 26,
            32,
            row.icon,
            row.text,
            row.color,
            Color::White,
            Color::Black,
            2,
            24);
    }

    Display_DrawFooterBar(
        "BOOT:NEXT HOLD:HOME",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        28);
}
