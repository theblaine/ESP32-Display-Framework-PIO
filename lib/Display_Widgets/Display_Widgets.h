#pragma once

#include <Arduino.h>

/**
 * Marker shapes supported by Display_DrawStatusIndicator().
 */
enum class Display_StatusMarkerShape : uint8_t
{
    Circle,
    Square,
    RoundedSquare
};


/**
 * Built-in monochrome icons supported by Display_DrawIcon().
 */
enum class Display_Icon : uint8_t
{
    Home,
    Info,
    Check,
    Warning,
    Power
};

/**
 * Draws a built-in monochrome icon inside a square area.
 *
 * The icon is scaled from an 8x8 bitmap to fit the requested size.
 * The complete square is first filled with backgroundColor.
 */
void Display_DrawIcon(
    int16_t x,
    int16_t y,
    int16_t size,
    Display_Icon icon,
    uint16_t iconColor,
    uint16_t backgroundColor);

/**
 * Draws an icon followed by a vertically centered text label.
 *
 * This function draws only the icon-and-label content. It does not draw
 * an outer panel or border around the complete widget.
 */
void Display_DrawIconLabel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    Display_Icon icon,
    const char* text,
    uint16_t iconColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale = 1,
    int16_t iconSize = 24);

/**
 * Draws a filled rectangular panel with a border.
 *
 * Set radius to 0 for square corners.
 * Use a positive radius for rounded corners.
 */
void Display_DrawPanel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t backgroundColor,
    uint16_t borderColor,
    int16_t radius = 0);

/**
 * Draws text centered horizontally and vertically inside a rectangular area.
 *
 * This function draws only the text. It does not draw a panel or border.
 */
void Display_DrawLabel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale = 1);


/**
 * Draws a value centered horizontally and vertically inside a rectangular area.
 *
 * This function draws only the value text. It does not draw a panel or border.
 * Use a larger text scale to emphasize numeric readings or status values.
 */
void Display_DrawValue(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* value,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale = 3);

/**
 * Draws a dashboard-style card containing a label and a value.
 *
 * The label is shown near the top of the supplied area.
 * The value is shown below it using a larger text scale.
 */
void Display_DrawLabelValue(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* label,
    const char* value,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t labelScale = 1,
    uint8_t valueScale = 3,
    int16_t radius = 0);

/**
 * Draws a configurable status indicator with a colored marker and text.
 *
 * The marker may be circular, square, or a rounded square.
 * Set drawMarkerBorder to true to outline the marker.
 * This function does not draw an outer panel around the complete widget.
 */
void Display_DrawStatusIndicator(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* text,
    uint16_t statusColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    Display_StatusMarkerShape markerShape = Display_StatusMarkerShape::Circle,
    uint8_t textScale = 1,
    bool drawMarkerBorder = false,
    uint16_t markerBorderColor = 0xFFFF);

/**
 * Draws the original circular status indicator.
 *
 * This compatibility wrapper is equivalent to Display_DrawStatusIndicator()
 * using a circular marker without a marker border.
 */
void Display_DrawStatus(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* text,
    uint16_t statusColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale = 1);


/**
 * Draws a full-width header bar at the top of the display.
 *
 * This convenience overload uses the default dashboard colors and sizing.
 */
void Display_DrawHeaderBar(const char* title);

/**
 * Draws a customizable full-width header bar at the top of the display.
 *
 * The title is left aligned. A separator line is drawn along the bottom edge.
 */
void Display_DrawHeaderBar(
    const char* title,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t textColor,
    uint8_t textScale = 1,
    int16_t height = 30);

/**
 * Draws a full-width footer bar at the bottom of the display.
 *
 * This convenience overload uses the default dashboard colors and sizing.
 */
void Display_DrawFooterBar(const char* text);

/**
 * Draws a customizable full-width footer bar at the bottom of the display.
 *
 * The text is centered. A separator line is drawn along the top edge.
 */
void Display_DrawFooterBar(
    const char* text,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t textColor,
    uint8_t textScale = 1,
    int16_t height = 28);


/**
 * Draws a horizontal battery level indicator.
 *
 * percent is limited to the range 0 through 100. The battery body is drawn
 * inside the supplied area with a small positive terminal on the right.
 * Set showPercent to true to center the numeric percentage over the battery.
 */
void Display_DrawBattery(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint8_t percent,
    uint16_t fillColor,
    uint16_t emptyColor,
    uint16_t borderColor,
    uint16_t backgroundColor,
    bool showPercent = false,
    uint16_t textColor = 0xFFFF,
    uint8_t textScale = 1);

/**
 * Draws a horizontal progress bar.
 *
 * percent is limited to the range 0 through 100.
 * Set radius to 0 for square corners.
 */
void Display_DrawProgressBar(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint8_t percent,
    uint16_t backgroundColor,
    uint16_t fillColor,
    uint16_t borderColor,
    int16_t radius = 0);
/**
 * One label/value row used by Display_DrawTable().
 *
 * Set valueColor to 0 to use the table's default value color.
 */
struct Display_TableRow
{
    const char* label;
    const char* value;
    uint16_t valueColor = 0;
};



struct Display_StatusScreenData
{
    String title;
    String status;
    String detail;
    uint16_t statusColor;
};


/**
 * Draws one two-column label/value table row.
 *
 * labelColumnWidth controls the width of the left column. Set it to 0 to
 * use 42 percent of the row width. The label is left aligned and the value
 * is right aligned.
 */
void Display_DrawTableRow(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* label,
    const char* value,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t textScale = 1,
    int16_t labelColumnWidth = 0,
    bool drawTopBorder = true,
    bool drawBottomBorder = true);

/**
 * Draws a two-column label/value table.
 *
 * Each row may optionally override the default value color. Set
 * labelColumnWidth to 0 to use 42 percent of the table width.
 */
void Display_DrawTable(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t rowHeight,
    const Display_TableRow* rows,
    size_t rowCount,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t textScale = 1,
    int16_t labelColumnWidth = 0,
    bool drawColumnDivider = true);




/**
 * Draws a simple full-screen service status page.
 *
 * title is shown at the top of the screen.
 * statusText is shown beneath it using statusColor.
 * detailText is optional and may contain an IP address, topic, filename,
 * or other supporting information.
 */
void Display_ShowStatusScreen(
    const char* title,
    const char* statusText,
    uint16_t statusColor,
    const char* detailText = nullptr);

/**
 * Convenience overload that accepts a status screen structure.
 */
void Display_ShowStatusScreen(
    const Display_StatusScreenData& screen);