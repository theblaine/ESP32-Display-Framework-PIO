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

    Display_DrawHeaderBar(
        "SYSTEM INFO",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        30);

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
        6,
        38,
        160,
        34,
        Rows,
        sizeof(Rows) / sizeof(Rows[0]),
        Color::Black,
        Color::Cyan,
        Color::Cyan,
        Color::White,
        1,
        58,
        true);

    Display_DrawFooterBar(
        "HOLD 3S:BRIGHTNESS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        28);
}
