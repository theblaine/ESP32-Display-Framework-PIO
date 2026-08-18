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

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawStatusIndicatorsDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t headerHeight = compactLayout ? 28 : 30;
    const int16_t footerHeight = 28;
    const int16_t rowStartY = compactLayout ? 34 : 45;
    const int16_t rowHeight = 42;
    const int16_t rowSpacing = compactLayout ? 44 : 55;
    const int16_t indicatorInset = 3;

    Display_DrawHeaderBar(
        "STATUS INDICATORS",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        headerHeight);

    // Circle marker without a marker border.
    Display_DrawPanel(
        10, rowStartY, Display::width() - 20, rowHeight,
        Color::Black, Color::Green, 8);

    Display_DrawStatusIndicator(
        14, rowStartY + indicatorInset, Display::width() - 28, rowHeight - (indicatorInset * 2),
        "ONLINE",
        Color::Green,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::Circle,
        2);

    // Rounded-square marker with a border.
    Display_DrawPanel(
        10, rowStartY + rowSpacing, Display::width() - 20, rowHeight,
        Color::Black, Color::Yellow, 8);

    Display_DrawStatusIndicator(
        14, rowStartY + rowSpacing + indicatorInset, Display::width() - 28, rowHeight - (indicatorInset * 2),
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
        10, rowStartY + (rowSpacing * 2), Display::width() - 20, rowHeight,
        Color::Black, Color::Red, 8);

    Display_DrawStatusIndicator(
        14, rowStartY + (rowSpacing * 2) + indicatorInset, Display::width() - 28, rowHeight - (indicatorInset * 2),
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
        10, rowStartY + (rowSpacing * 3), Display::width() - 20, rowHeight,
        Color::Black, Color::Cyan, 8);

    Display_DrawStatusIndicator(
        14, rowStartY + (rowSpacing * 3) + indicatorInset, Display::width() - 28, rowHeight - (indicatorInset * 2),
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
        footerHeight);
}
