/******************************************************************************
 * Demo:
 *     Status Indicators
 *
 * Demonstrates:
 *     Display_DrawStatusIndicator()
 *
 * Related widgets:
 *     Display_DrawHeaderBar()
 *     Display_DrawFooterBar()
 *     Display_DrawPanel()
 ******************************************************************************/

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawStatusIndicatorsDemo()
{
    Display_FillScreen(Color::Black);

    Display_DrawHeaderBar(
        "STATUS INDICATORS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        30);

    // Circle marker without a marker border.
    Display_DrawPanel(
        10, 45, Display::width() - 20, 42,
        Color::Black, Color::Green, 8);

    Display_DrawStatusIndicator(
        14, 48, Display::width() - 28, 36,
        "ONLINE",
        Color::Green,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::Circle,
        2);

    // Rounded-square marker with a border.
    Display_DrawPanel(
        10, 100, Display::width() - 20, 42,
        Color::Black, Color::Yellow, 8);

    Display_DrawStatusIndicator(
        14, 103, Display::width() - 28, 36,
        "WARNING",
        Color::Yellow,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::RoundedSquare,
        1,
        true,
        Color::White);

    // Square marker with a border.
    Display_DrawPanel(
        10, 155, Display::width() - 20, 42,
        Color::Black, Color::Red, 8);

    Display_DrawStatusIndicator(
        14, 158, Display::width() - 28, 36,
        "OFFLINE",
        Color::Red,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::Square,
        2,
        true,
        Color::White);

    // Another circular marker demonstrates a neutral/info state.
    Display_DrawPanel(
        10, 210, Display::width() - 20, 42,
        Color::Black, Color::Cyan, 8);

    Display_DrawStatusIndicator(
        14, 213, Display::width() - 28, 36,
        "STANDBY",
        Color::Cyan,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::Circle,
        1,
        true,
        Color::White);

    Display_DrawFooterBar(
        "BOOT: NEXT  HOLD: HOME",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        28);
}
