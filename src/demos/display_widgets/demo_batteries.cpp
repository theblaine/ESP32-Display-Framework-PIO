/******************************************************************************
 * Demo:
 *     Battery Widgets
 *
 * Demonstrates:
 *     Display_DrawBattery()
 *
 * Related widgets:
 *     Display_DrawHeaderBar()
 *     Display_DrawFooterBar()
 *     Display_DrawLabel()
 ******************************************************************************/

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawBatteriesDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawHeaderBar(
        "BATTERY WIDGETS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        30);

    struct BatteryRow
    {
        int16_t y;
        uint8_t percent;
        uint16_t fillColor;
        const char* label;
    };

    constexpr BatteryRow Rows[] = {
        {46,  100, Color::Green,  "FULL"},
        {96,   75, Color::Green,  "GOOD"},
        {146,  50, Color::Yellow, "HALF"},
        {196,  25, Color::Yellow, "LOW"},
        {246,   5, Color::Red,    "CRITICAL"}
    };

    for (const BatteryRow& row : Rows)
    {
        Display_DrawLabel(
            8,
            row.y,
            52,
            28,
            row.label,
            Color::White,
            Color::Black,
            1);

        Display_DrawBattery(
            62,
            row.y,
            102,
            28,
            row.percent,
            row.fillColor,
            Color::Black,
            Color::White,
            Color::Black,
            true,
            Color::White,
            1);
    }

    Display_DrawFooterBar(
        "HOLD 3S:BRIGHTNESS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        28);
}
