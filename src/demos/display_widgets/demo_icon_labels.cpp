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

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "WidgetDemoPages.h"

void DrawIconLabelsDemo()
{
    Display_FillScreen(
        Color::Black);

    const bool compactLayout =
        Display::height() < 300;

    const int16_t headerHeight =
        compactLayout ? 28 : 30;

    const int16_t footerHeight =
        28;

    const int16_t rowStartY =
        compactLayout ? 34 : 42;

    const int16_t rowHeight =
        compactLayout ? 30 : 38;

    const int16_t rowGap =
        compactLayout ? 3 : 11;

    const int16_t panelX =
        compactLayout ? 6 : 10;

    const int16_t panelWidth =
        Display::width() -
        (panelX * 2);

    const int16_t iconLabelX =
        panelX + 3;

    const int16_t iconLabelWidth =
        panelWidth - 6;

    const int16_t iconSize =
        compactLayout ? 18 : 24;

    Display_DrawHeaderBar(
        "ICON + LABEL",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        headerHeight);

    struct DemoRow
    {
        Display_Icon icon;
        const char *text;
        uint16_t color;
    };

    constexpr DemoRow Rows[] =
        {
            {
                Display_Icon::Home,
                "HOME",
                Color::Cyan
            },
            {
                Display_Icon::Info,
                "INFO",
                Color::Blue
            },
            {
                Display_Icon::Check,
                "READY",
                Color::Green
            },
            {
                Display_Icon::Warning,
                "WARNING",
                Color::Yellow
            },
            {
                Display_Icon::Power,
                "POWER",
                Color::Red
            }
        };

    int16_t y =
        rowStartY;

    for (const DemoRow &row : Rows)
    {
        Display_DrawPanel(
            panelX,
            y,
            panelWidth,
            rowHeight,
            Color::Black,
            row.color,
            7);

        Display_DrawIconLabel(
            iconLabelX,
            y + 3,
            iconLabelWidth,
            rowHeight - 6,
            row.icon,
            row.text,
            row.color,
            Color::White,
            Color::Black,
            2,
            iconSize);

        y +=
            rowHeight +
            rowGap;
    }

    Display_DrawFooterBar(
        "BOOT:NEXT HOLD:HOME",
        Color::Blue,
        Color::Cyan,
        Color::White,
        1,
        footerHeight);
}
