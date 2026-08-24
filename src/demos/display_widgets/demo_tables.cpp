/******************************************************************************
 * Demo:
 *     Table Widget
 *
 * Demonstrates:
 *     Display_DrawTable()
 *     Display_TableRow
 *
 * Related widgets:
 *     Display_DrawHeaderBar()
 *     Display_DrawFooterBar()
 ******************************************************************************/

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawTablesDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t headerHeight = compactLayout ? 28 : 30;
    const int16_t footerHeight = 28;
    const int16_t tableY = compactLayout ? 34 : 38;
    const int16_t rowHeight = compactLayout ? 24 : 34;
    const int16_t tableX = compactLayout ? 4 : 6;

    Display_DrawHeaderBar(
        "SYSTEM INFO",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        headerHeight);

    constexpr Display_TableRow Rows[] = {
        {"WIFI",   "CONNECTED", Color::Green},
        {"RSSI",   "-61 DBM",   Color::Cyan},
        {"IP",     "10.0.0.25", Color::White},
        {"HEAP",   "243 KB",    Color::White},
        {"FLASH",  "61%",       Color::Yellow},
        {"CPU",    "240 MHZ",   Color::White},
        {"UPTIME", "3D 14H",    Color::Green}
    };

    Display_DrawTable(
        tableX,
        tableY,
        Display::width() - (tableX * 2),
        rowHeight,
        Rows,
        sizeof(Rows) / sizeof(Rows[0]),
        Color::Black,
        Color::Cyan,
        Color::Cyan,
        Color::White,
        1,
        compactLayout ? 70 : 58,
        true);

    Display_DrawFooterBar(
        "HOLD 3S:BRIGHTNESS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        footerHeight);
}
