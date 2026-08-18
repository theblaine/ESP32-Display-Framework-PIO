/******************************************************************************
 * Demo:
 *     Panels and Labels
 *
 * Demonstrates:
 *     Display_DrawPanel()
 *     Display_DrawLabel()
 *
 * Notes:
 *     Shows square and rounded panels with centered text.
 ******************************************************************************/

#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Display.h"

#include "WidgetDemoPages.h"

void DrawPanelsAndLabelsDemo()
{
    Display_FillScreen(Color::Black);

    const bool compactLayout = Display::height() < 300;
    const int16_t panelX = compactLayout ? 8 : 10;
    const int16_t panelWidth = Display::width() - (panelX * 2);
    const int16_t panelHeight = compactLayout ? 48 : 45;
    const int16_t panelStartY = 10;
    const int16_t panelSpacing = compactLayout ? 56 : 60;

    // Square panel with centered label.
    Display_DrawPanel(
        panelX,
        panelStartY,
        panelWidth,
        panelHeight,
        Color::Black,
        Color::White);

    Display_DrawLabel(
        panelX,
        panelStartY,
        panelWidth,
        panelHeight,
        "RADIUS 0",
        Color::White,
        Color::Black,
        2);

    // Slightly rounded panel with centered label.
    Display_DrawPanel(
        panelX,
        panelStartY + panelSpacing,
        panelWidth,
        panelHeight,
        Color::Black,
        Color::Cyan,
        4);

    Display_DrawLabel(
        panelX,
        panelStartY + panelSpacing,
        panelWidth,
        panelHeight,
        "RADIUS 4",
        Color::Cyan,
        Color::Black,
        2);

    // Medium rounded panel with centered label.
    Display_DrawPanel(
        panelX,
        panelStartY + (panelSpacing * 2),
        panelWidth,
        panelHeight,
        Color::Black,
        Color::Yellow,
        8);

    Display_DrawLabel(
        panelX,
        panelStartY + (panelSpacing * 2),
        panelWidth,
        panelHeight,
        "RADIUS 8",
        Color::Yellow,
        Color::Black,
        2);

    // Heavily rounded panel with centered label.
    Display_DrawPanel(
        panelX,
        panelStartY + (panelSpacing * 3),
        panelWidth,
        panelHeight,
        Color::Green,
        Color::White,
        compactLayout ? 14 : 16);

    Display_DrawLabel(
        panelX,
        panelStartY + (panelSpacing * 3),
        panelWidth,
        panelHeight,
        "RADIUS 16",
        Color::White,
        Color::Green,
        2);
}
