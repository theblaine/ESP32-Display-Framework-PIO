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

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawHeaderFooterBarsDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t headerHeight = compactLayout ? 28 : 34;
    const int16_t footerHeight = 28;
    const int16_t panelHeight = compactLayout ? 76 : 70;
    const int16_t firstPanelY = compactLayout ? 34 : 55;
    const int16_t secondPanelY = compactLayout ? 120 : 145;

    Display_DrawHeaderBar(
        "DASHBOARD",
        Color::Blue,
        Color::Cyan,
        Color::White,
        2,
        headerHeight);

    Display_DrawPanel(
        10,
        firstPanelY,
        Display::width() - 20,
        panelHeight,
        Color::Black,
        Color::Cyan,
        8);

    Display_DrawLabel(
        10,
        firstPanelY,
        Display::width() - 20,
        panelHeight,
        "CONTENT AREA",
        Color::Cyan,
        Color::Black,
        compactLayout ? 1 : 2);

    Display_DrawPanel(
        10,
        secondPanelY,
        Display::width() - 20,
        panelHeight,
        Color::Black,
        Color::White,
        8);

    Display_DrawLabel(
        10,
        secondPanelY,
        Display::width() - 20,
        panelHeight,
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
        footerHeight);
}
