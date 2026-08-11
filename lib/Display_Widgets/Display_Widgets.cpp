#include "Display_Widgets.h"

#include "Display_GFX.h"
#include "Display_Text.h"

#include "DisplayConfig.h"

namespace
{
    constexpr uint8_t IconWidth = 8;
    constexpr uint8_t IconHeight = 8;

    // Returns the bitmap for a built-in 8x8 icon.
    const uint8_t *GetIconBitmap(Display_Icon icon)
    {
        // Each byte represents one 8-pixel row, most-significant bit first.
        static constexpr uint8_t HomeBitmap[IconHeight] = {
            0b00011000,
            0b00111100,
            0b01111110,
            0b11111111,
            0b01111110,
            0b01100110,
            0b01100110,
            0b01100110};

        static constexpr uint8_t InfoBitmap[IconHeight] = {
            0b00111100,
            0b01100110,
            0b01100110,
            0b00011000,
            0b00011000,
            0b00011000,
            0b01111110,
            0b00111100};

        static constexpr uint8_t CheckBitmap[IconHeight] = {
            0b00000000,
            0b00000001,
            0b00000011,
            0b01000110,
            0b01101100,
            0b00111000,
            0b00010000,
            0b00000000};

        static constexpr uint8_t WarningBitmap[IconHeight] = {
            0b00011000,
            0b00111100,
            0b00111100,
            0b01111110,
            0b01100110,
            0b11111111,
            0b11011011,
            0b11111111};

        static constexpr uint8_t PowerBitmap[IconHeight] = {
            0b00011000,
            0b00011000,
            0b01011010,
            0b11011011,
            0b11000011,
            0b01100110,
            0b00111100,
            0b00011000};

        switch (icon)
        {
        case Display_Icon::Home:
            return HomeBitmap;

        case Display_Icon::Info:
            return InfoBitmap;

        case Display_Icon::Check:
            return CheckBitmap;

        case Display_Icon::Warning:
            return WarningBitmap;

        case Display_Icon::Power:
            return PowerBitmap;
        }

        return InfoBitmap;
    }
}

void Display_DrawIcon(
    int16_t x,
    int16_t y,
    int16_t size,
    Display_Icon icon,
    uint16_t iconColor,
    uint16_t backgroundColor)
{
    if (size <= 0)
    {
        return;
    }

    Display_FillRect(x, y, size, size, backgroundColor);

    const uint8_t *bitmap = GetIconBitmap(icon);
    int16_t pixelSize = size / IconWidth;

    if (pixelSize < 1)
    {
        pixelSize = 1;
    }

    const int16_t renderedSize = pixelSize * IconWidth;
    const int16_t offsetX = x + ((size - renderedSize) / 2);
    const int16_t offsetY = y + ((size - renderedSize) / 2);

    for (uint8_t row = 0; row < IconHeight; row++)
    {
        for (uint8_t column = 0; column < IconWidth; column++)
        {
            const uint8_t mask = static_cast<uint8_t>(0x80U >> column);

            if ((bitmap[row] & mask) == 0)
            {
                continue;
            }

            Display_FillRect(
                offsetX + (column * pixelSize),
                offsetY + (row * pixelSize),
                pixelSize,
                pixelSize,
                iconColor);
        }
    }
}

void Display_DrawIconLabel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    Display_Icon icon,
    const char *text,
    uint16_t iconColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale,
    int16_t iconSize)
{
    if (width <= 0 ||
        height <= 0 ||
        text == nullptr ||
        textScale == 0)
    {
        return;
    }

    Display_FillRect(x, y, width, height, backgroundColor);

    constexpr int16_t Padding = 6;

    if (iconSize <= 0)
    {
        iconSize = height - (Padding * 2);
    }

    const int16_t maximumIconSize = height - (Padding * 2);

    if (iconSize > maximumIconSize)
    {
        iconSize = maximumIconSize;
    }

    if (iconSize < 8)
    {
        iconSize = 8;
    }

    const int16_t iconX = x + Padding;
    const int16_t iconY = y + ((height - iconSize) / 2);

    Display_DrawIcon(
        iconX,
        iconY,
        iconSize,
        icon,
        iconColor,
        backgroundColor);

    const int16_t textX = iconX + iconSize + Padding;
    const int16_t textWidth = width - (textX - x) - Padding;

    if (textWidth <= 0)
    {
        return;
    }

    Display_DrawLabel(
        textX,
        y,
        textWidth,
        height,
        text,
        textColor,
        backgroundColor,
        textScale);
}

void Display_DrawPanel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t backgroundColor,
    uint16_t borderColor,
    int16_t radius)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (radius <= 0)
    {
        Display_FillRect(
            x,
            y,
            width,
            height,
            backgroundColor);

        Display_DrawRect(
            x,
            y,
            width,
            height,
            borderColor);

        return;
    }

    Display_FillRoundRect(
        x,
        y,
        width,
        height,
        radius,
        backgroundColor);

    Display_DrawRoundRect(
        x,
        y,
        width,
        height,
        radius,
        borderColor);
}

void Display_DrawLabel(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale)
{
    if (width <= 0 ||
        height <= 0 ||
        text == nullptr ||
        textScale == 0)
    {
        return;
    }

    constexpr int16_t FontHeight = 7;

    const int16_t textHeight =
        FontHeight * textScale;

    const int16_t textY =
        y + ((height - textHeight) / 2);

    const int16_t centerX =
        x + (width / 2);

    Display_DrawTextCentered(
        centerX,
        textY,
        text,
        textColor,
        backgroundColor,
        textScale);
}

void Display_DrawValue(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *value,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale)
{
    Display_DrawLabel(
        x,
        y,
        width,
        height,
        value,
        textColor,
        backgroundColor,
        textScale);
}

void Display_DrawProgressBar(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint8_t percent,
    uint16_t backgroundColor,
    uint16_t fillColor,
    uint16_t borderColor,
    int16_t radius)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (percent > 100)
    {
        percent = 100;
    }

    // Draw the outer panel.
    Display_DrawPanel(
        x,
        y,
        width,
        height,
        backgroundColor,
        borderColor,
        radius);

    // Leave the border visible.
    constexpr int16_t padding = 2;

    const int16_t innerX = x + padding;
    const int16_t innerY = y + padding;
    const int16_t innerWidth = width - (padding * 2);
    const int16_t innerHeight = height - (padding * 2);

    if (innerWidth <= 0 || innerHeight <= 0)
    {
        return;
    }

    const int16_t fillWidth =
        (innerWidth * percent) / 100;

    if (fillWidth <= 0)
    {
        return;
    }

    int16_t innerRadius = radius - padding;

    if (innerRadius < 0)
    {
        innerRadius = 0;
    }

    Display_FillRoundRect(
        innerX,
        innerY,
        fillWidth,
        innerHeight,
        innerRadius,
        fillColor);
}

void Display_DrawSignalMeter(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint8_t level,
    uint8_t barCount,
    uint16_t activeColor,
    uint16_t inactiveColor,
    uint16_t backgroundColor)
{
    if (width <= 0 ||
        height <= 0 ||
        barCount == 0)
    {
        return;
    }

    if (level > barCount)
    {
        level = barCount;
    }

    Display_FillRect(
        x,
        y,
        width,
        height,
        backgroundColor);

    constexpr int16_t Gap = 2;

    const int16_t totalGapWidth =
        Gap * (barCount - 1);

    const int16_t barWidth =
        (width - totalGapWidth) / barCount;

    if (barWidth <= 0)
    {
        return;
    }

    for (uint8_t index = 0;
         index < barCount;
         index++)
    {
        const int16_t barHeight =
            ((index + 1) * height) /
            barCount;

        const int16_t barX =
            x + (index * (barWidth + Gap));

        const int16_t barY =
            y + height - barHeight;

        const uint16_t barColor =
            index < level
                ? activeColor
                : inactiveColor;

        Display_FillRect(
            barX,
            barY,
            barWidth,
            barHeight,
            barColor);
    }
}

void Display_DrawLabelValue(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *label,
    const char *value,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t labelScale,
    uint8_t valueScale,
    int16_t radius)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    Display_DrawPanel(
        x,
        y,
        width,
        height,
        backgroundColor,
        borderColor,
        radius);

    const int16_t labelHeight = height / 3;
    const int16_t valueHeight = height - labelHeight;

    Display_DrawLabel(
        x,
        y,
        width,
        labelHeight,
        label,
        labelColor,
        backgroundColor,
        labelScale);

    Display_DrawValue(
        x,
        y + labelHeight,
        width,
        valueHeight,
        value,
        valueColor,
        backgroundColor,
        valueScale);
}

void Display_DrawStatusIndicator(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *text,
    uint16_t statusColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    Display_StatusMarkerShape markerShape,
    uint8_t textScale,
    bool drawMarkerBorder,
    uint16_t markerBorderColor)
{
    if (width <= 0 ||
        height <= 0 ||
        text == nullptr ||
        textScale == 0)
    {
        return;
    }

    Display_FillRect(
        x,
        y,
        width,
        height,
        backgroundColor);

    constexpr int16_t Padding = 6;

    int16_t markerSize = height - (Padding * 2);

    if (markerSize < 4)
    {
        markerSize = 4;
    }

    if (markerSize > 18)
    {
        markerSize = 18;
    }

    const int16_t markerX = x + Padding;
    const int16_t markerY = y + ((height - markerSize) / 2);

    int16_t markerRadius = 0;

    switch (markerShape)
    {
    case Display_StatusMarkerShape::Circle:
        markerRadius = markerSize / 2;
        break;

    case Display_StatusMarkerShape::Square:
        markerRadius = 0;
        break;

    case Display_StatusMarkerShape::RoundedSquare:
        markerRadius = markerSize / 4;
        break;
    }

    if (markerRadius <= 0)
    {
        Display_FillRect(
            markerX,
            markerY,
            markerSize,
            markerSize,
            statusColor);

        if (drawMarkerBorder)
        {
            Display_DrawRect(
                markerX,
                markerY,
                markerSize,
                markerSize,
                markerBorderColor);
        }
    }
    else
    {
        Display_FillRoundRect(
            markerX,
            markerY,
            markerSize,
            markerSize,
            markerRadius,
            statusColor);

        if (drawMarkerBorder)
        {
            Display_DrawRoundRect(
                markerX,
                markerY,
                markerSize,
                markerSize,
                markerRadius,
                markerBorderColor);
        }
    }

    const int16_t textX =
        markerX + markerSize + Padding;

    const int16_t textWidth =
        width - (textX - x) - Padding;

    if (textWidth <= 0)
    {
        return;
    }

    Display_DrawLabel(
        textX,
        y,
        textWidth,
        height,
        text,
        textColor,
        backgroundColor,
        textScale);
}

void Display_DrawStatus(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *text,
    uint16_t statusColor,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t textScale)
{
    Display_DrawStatusIndicator(
        x,
        y,
        width,
        height,
        text,
        statusColor,
        textColor,
        backgroundColor,
        Display_StatusMarkerShape::Circle,
        textScale,
        false,
        0xFFFF);
}

void Display_DrawHeaderBar(const char *title)
{
    Display_DrawHeaderBar(
        title,
        0x0000,
        0x07FF,
        0xFFFF,
        1,
        30);
}

void Display_DrawHeaderBar(
    const char *title,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t textColor,
    uint8_t textScale,
    int16_t height)
{
    if (title == nullptr || textScale == 0 || height <= 0)
    {
        return;
    }

    if (height > DisplayConfig::Height)
    {
        height = DisplayConfig::Height;
    }

    Display_FillRect(0, 0, DisplayConfig::Width, height, backgroundColor);
    Display_DrawFastHLine(0, height - 1, DisplayConfig::Width, borderColor);

    constexpr int16_t FontHeight = 7;
    constexpr int16_t HorizontalPadding = 6;

    const int16_t textHeight = FontHeight * textScale;
    const int16_t textY = (height - textHeight) / 2;

    Display_DrawText(
        HorizontalPadding,
        textY,
        title,
        textColor,
        backgroundColor,
        textScale);
}

void Display_DrawFooterBar(const char *text)
{
    Display_DrawFooterBar(
        text,
        0x0000,
        0x07FF,
        0xFFFF,
        1,
        28);
}

void Display_DrawFooterBar(
    const char *text,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t textColor,
    uint8_t textScale,
    int16_t height)
{
    if (text == nullptr || textScale == 0 || height <= 0)
    {
        return;
    }

    if (height > DisplayConfig::Height)
    {
        height = DisplayConfig::Height;
    }

    const int16_t y = DisplayConfig::Height - height;

    Display_FillRect(0, y, DisplayConfig::Width, height, backgroundColor);
    Display_DrawFastHLine(0, y, DisplayConfig::Width, borderColor);

    constexpr int16_t FontHeight = 7;

    const int16_t textHeight = FontHeight * textScale;
    const int16_t textY = y + ((height - textHeight) / 2);

    Display_DrawTextCentered(
        DisplayConfig::Width / 2,
        textY,
        text,
        textColor,
        backgroundColor,
        textScale);
}

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
    bool showPercent,
    uint16_t textColor,
    uint8_t textScale)
{
    if (width < 12 || height < 8)
    {
        return;
    }

    if (percent > 100)
    {
        percent = 100;
    }

    Display_FillRect(x, y, width, height, backgroundColor);

    int16_t terminalWidth = width / 10;

    if (terminalWidth < 3)
    {
        terminalWidth = 3;
    }
    else if (terminalWidth > 8)
    {
        terminalWidth = 8;
    }

    constexpr int16_t TerminalGap = 1;
    const int16_t bodyWidth = width - terminalWidth - TerminalGap;

    if (bodyWidth < 8)
    {
        return;
    }

    const int16_t radius = height / 5;

    Display_FillRoundRect(
        x,
        y,
        bodyWidth,
        height,
        radius,
        emptyColor);

    Display_DrawRoundRect(
        x,
        y,
        bodyWidth,
        height,
        radius,
        borderColor);

    int16_t terminalHeight = height / 2;

    if (terminalHeight < 4)
    {
        terminalHeight = 4;
    }

    const int16_t terminalX = x + bodyWidth + TerminalGap;
    const int16_t terminalY = y + ((height - terminalHeight) / 2);

    Display_FillRect(
        terminalX,
        terminalY,
        terminalWidth,
        terminalHeight,
        borderColor);

    constexpr int16_t InnerPadding = 3;
    const int16_t innerX = x + InnerPadding;
    const int16_t innerY = y + InnerPadding;
    const int16_t innerWidth = bodyWidth - (InnerPadding * 2);
    const int16_t innerHeight = height - (InnerPadding * 2);

    if (innerWidth > 0 && innerHeight > 0 && percent > 0)
    {
        const int16_t fillWidth =
            (static_cast<int32_t>(innerWidth) * percent) / 100;

        if (fillWidth > 0)
        {
            int16_t innerRadius = radius - InnerPadding;

            if (innerRadius < 0)
            {
                innerRadius = 0;
            }

            Display_FillRoundRect(
                innerX,
                innerY,
                fillWidth,
                innerHeight,
                innerRadius,
                fillColor);
        }
    }

    if (showPercent && textScale > 0)
    {
        char percentText[6];
        snprintf(percentText, sizeof(percentText), "%u%%", percent);

        Display_DrawLabel(
            x,
            y,
            bodyWidth,
            height,
            percentText,
            textColor,
            backgroundColor,
            textScale);
    }
}

void Display_DrawTableRow(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char *label,
    const char *value,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t textScale,
    int16_t labelColumnWidth,
    bool drawTopBorder,
    bool drawBottomBorder)
{
    if (width <= 0 || height <= 0 || label == nullptr || value == nullptr || textScale == 0)
    {
        return;
    }

    constexpr int16_t HorizontalPadding = 5;
    constexpr int16_t FontHeight = 7;

    if (labelColumnWidth <= 0)
    {
        labelColumnWidth = (width * 42) / 100;
    }

    if (labelColumnWidth < HorizontalPadding * 2)
    {
        labelColumnWidth = HorizontalPadding * 2;
    }
    else if (labelColumnWidth > width - (HorizontalPadding * 2))
    {
        labelColumnWidth = width - (HorizontalPadding * 2);
    }

    Display_FillRect(x, y, width, height, backgroundColor);

    if (drawTopBorder)
    {
        Display_DrawFastHLine(x, y, width, borderColor);
    }

    if (drawBottomBorder)
    {
        Display_DrawFastHLine(x, y + height - 1, width, borderColor);
    }

    const int16_t textHeight = FontHeight * textScale;
    const int16_t textY = y + ((height - textHeight) / 2);

    Display_DrawText(
        x + HorizontalPadding,
        textY,
        label,
        labelColor,
        backgroundColor,
        textScale);

    Display_DrawTextRight(
        x + width - HorizontalPadding - 1,
        textY,
        value,
        valueColor,
        backgroundColor,
        textScale);
}

void Display_DrawTable(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t rowHeight,
    const Display_TableRow *rows,
    size_t rowCount,
    uint16_t backgroundColor,
    uint16_t borderColor,
    uint16_t labelColor,
    uint16_t valueColor,
    uint8_t textScale,
    int16_t labelColumnWidth,
    bool drawColumnDivider)
{
    if (width <= 0 || rowHeight <= 0 || rows == nullptr || rowCount == 0 || textScale == 0)
    {
        return;
    }

    if (labelColumnWidth <= 0)
    {
        labelColumnWidth = (width * 42) / 100;
    }

    const int32_t calculatedHeight = static_cast<int32_t>(rowHeight) * rowCount;

    if (calculatedHeight > INT16_MAX)
    {
        return;
    }

    const int16_t tableHeight = static_cast<int16_t>(calculatedHeight);

    Display_FillRect(x, y, width, tableHeight, backgroundColor);

    for (size_t index = 0; index < rowCount; index++)
    {
        const int16_t rowY = y + static_cast<int16_t>(index * rowHeight);
        const uint16_t rowValueColor =
            rows[index].valueColor == 0 ? valueColor : rows[index].valueColor;

        Display_DrawTableRow(
            x + 1,
            rowY + 1,
            width - 2,
            rowHeight - 1,
            rows[index].label,
            rows[index].value,
            backgroundColor,
            borderColor,
            labelColor,
            rowValueColor,
            textScale,
            labelColumnWidth - 1,
            false,
            index + 1 < rowCount);
    }

    if (drawColumnDivider)
    {
        Display_DrawFastVLine(
            x + labelColumnWidth,
            y,
            tableHeight,
            borderColor);
    }

    // Draw the outer border last so row background fills cannot overwrite
    // the bottom edge of the table.
    Display_DrawRect(x, y, width, tableHeight, borderColor);
}

void Display_ShowStatusScreen(
    const char *title,
    const char *statusText,
    uint16_t statusColor,
    const char *detailText)
{
    if (title == nullptr || statusText == nullptr)
    {
        return;
    }

    Display_FillScreen(Color::Black);

    Display_DrawHeaderBar(
        title,
        Color::Blue,
        Color::White,
        Color::White,
        2,
        34);

    Display_DrawStatusIndicator(
        12,
        60,
        DisplayConfig::Width - 24,
        42,
        statusText,
        statusColor,
        Color::White,
        Color::Black,
        Display_StatusMarkerShape::Circle,
        2);

    if (detailText != nullptr)
    {
        Display_DrawTextWrapped(
            10,
            120,
            DisplayConfig::Width - 20,
            detailText,
            Color::Cyan,
            Color::Black,
            2);
    }
}

void Display_ShowStatusScreen(
    const Display_StatusScreenData &screen)
{
    Display_ShowStatusScreen(
        screen.title.c_str(),
        screen.status.c_str(),
        screen.statusColor,
        screen.detail.c_str());
}