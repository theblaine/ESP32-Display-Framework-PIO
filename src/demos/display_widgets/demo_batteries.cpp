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

    const bool compactLayout = Display::height() < 300;
    const int16_t headerHeight = compactLayout ? 28 : 30;
    const int16_t footerHeight = 28;
    const int16_t rowStartY = compactLayout ? 34 : 46;
    const int16_t rowHeight = 28;
    const int16_t rowSpacing = compactLayout ? 36 : 50;
    const int16_t labelWidth = compactLayout ? 48 : 52;
    const int16_t batteryX = compactLayout ? 56 : 62;

    Display_DrawHeaderBar(
        "BATTERY WIDGETS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        headerHeight);

    struct BatteryRow
    {
        uint8_t percent;
        uint16_t fillColor;
        const char* label;
    };

    constexpr BatteryRow Rows[] = {
        {100, Color::Green,  "FULL"},
        { 75, Color::Green,  "GOOD"},
        { 50, Color::Yellow, "HALF"},
        { 25, Color::Yellow, "LOW"},
        {  5, Color::Red,    "CRITICAL"}
    };

    int16_t y = rowStartY;
    for (const BatteryRow& row : Rows)
    {
        Display_DrawLabel(
            8,
            y,
            labelWidth,
            rowHeight,
            row.label,
            Color::White,
            Color::Black,
            1);

        Display_DrawBattery(
            batteryX,
            y,
            Display::width() - batteryX - 8,
            rowHeight,
            row.percent,
            row.fillColor,
            Color::Black,
            Color::White,
            Color::Black,
            true,
            Color::White,
            1);

        y += rowSpacing;
    }

    Display_DrawFooterBar(
        "HOLD 3S:BRIGHTNESS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        footerHeight);
}
