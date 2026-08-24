#include "Display_GFX.h"

#include "Display_ST7789.h"
#include "DisplayConfig.h"

#include <cmath>

void Display_FillScreen(uint16_t color)
{
    static uint16_t rowBuffer[DisplayConfig::Width];

    // Prepare one complete row of the requested color.
    for (uint16_t x = 0; x < DisplayConfig::Width; x++)
    {
        rowBuffer[x] = color;
    }

    // Send that row to every row of the display.
    for (uint16_t y = 0; y < DisplayConfig::Height; y++)
    {
        LCD_AddWindow(
            0,
            y,
            DisplayConfig::Width - 1,
            y,
            rowBuffer);
    }
}

void Display_FillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    static uint16_t rowBuffer[DisplayConfig::Width];

    for (int16_t i = 0; i < width; i++)
    {
        rowBuffer[i] = color;
    }

    for (int16_t row = 0; row < height; row++)
    {
        LCD_AddWindow(
            x,
            y + row,
            x + width - 1,
            y + row,
            rowBuffer);
    }
}

void Display_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
    uint16_t pixel = color;

    LCD_AddWindow(
        x,
        y,
        x,
        y,
        &pixel);
}

void Display_DrawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color)
{
    if (width <= 0)
    {
        return;
    }

    static uint16_t rowBuffer[DisplayConfig::Width];

    for (int16_t i = 0; i < width; i++)
    {
        rowBuffer[i] = color;
    }

    LCD_AddWindow(
        x,
        y,
        x + width - 1,
        y,
        rowBuffer);
}

void Display_DrawFastVLine(int16_t x, int16_t y, int16_t height, uint16_t color)
{
    if (height <= 0)
    {
        return;
    }

    static uint16_t columnBuffer[DisplayConfig::Height];

    for (int16_t i = 0; i < height; i++)
    {
        columnBuffer[i] = color;
    }

    LCD_AddWindow(
        x,
        y,
        x,
        y + height - 1,
        columnBuffer);
}

void Display_DrawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    Display_DrawFastHLine(
        x,
        y,
        width,
        color);

    Display_DrawFastHLine(
        x,
        y + height - 1,
        width,
        color);

    Display_DrawFastVLine(
        x,
        y,
        height,
        color);

    Display_DrawFastVLine(
        x + width - 1,
        y,
        height,
        color);
}

void Display_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    // Use the faster specialized functions for straight lines.
    if (y0 == y1)
    {
        int16_t x = min(x0, x1);
        int16_t width = abs(x1 - x0) + 1;

        Display_DrawFastHLine(
            x,
            y0,
            width,
            color);

        return;
    }

    if (x0 == x1)
    {
        int16_t y = min(y0, y1);
        int16_t height = abs(y1 - y0) + 1;

        Display_DrawFastVLine(
            x0,
            y,
            height,
            color);

        return;
    }

    int16_t deltaX = abs(x1 - x0);
    int16_t stepX = x0 < x1 ? 1 : -1;

    int16_t deltaY = -abs(y1 - y0);
    int16_t stepY = y0 < y1 ? 1 : -1;

    int16_t error = deltaX + deltaY;

    while (true)
    {
        Display_DrawPixel(
            x0,
            y0,
            color);

        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        int16_t twiceError = error * 2;

        if (twiceError >= deltaY)
        {
            error += deltaY;
            x0 += stepX;
        }

        if (twiceError <= deltaX)
        {
            error += deltaX;
            y0 += stepY;
        }
    }
}

void Display_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    Display_DrawLine(
        x0,
        y0,
        x1,
        y1,
        color);

    Display_DrawLine(
        x1,
        y1,
        x2,
        y2,
        color);

    Display_DrawLine(
        x2,
        y2,
        x0,
        y0,
        color);
}

void Display_DrawCircle(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color)
{
    if (radius < 0)
    {
        return;
    }

    int16_t x = radius;
    int16_t y = 0;
    int16_t error = 1 - radius;

    while (x >= y)
    {
        Display_DrawPixel(centerX + x, centerY + y, color);
        Display_DrawPixel(centerX + y, centerY + x, color);
        Display_DrawPixel(centerX - y, centerY + x, color);
        Display_DrawPixel(centerX - x, centerY + y, color);
        Display_DrawPixel(centerX - x, centerY - y, color);
        Display_DrawPixel(centerX - y, centerY - x, color);
        Display_DrawPixel(centerX + y, centerY - x, color);
        Display_DrawPixel(centerX + x, centerY - y, color);

        y++;

        if (error < 0)
        {
            error += 2 * y + 1;
        }
        else
        {
            x--;
            error += 2 * (y - x) + 1;
        }
    }
}



void Display_DrawRoundRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    int16_t radius,
    uint16_t color)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (radius <= 0)
    {
        Display_DrawRect(
            x,
            y,
            width,
            height,
            color);

        return;
    }

    const int16_t maxRadius =
        ((width < height) ? width : height) / 2;

    if (radius > maxRadius)
    {
        radius = maxRadius;
    }

    const int16_t leftCenter =
        x + radius;

    const int16_t rightCenter =
        x + width - radius - 1;

    const int16_t topCenter =
        y + radius;

    const int16_t bottomCenter =
        y + height - radius - 1;

    // Straight top and bottom edges
    Display_DrawLine(
        leftCenter,
        y,
        rightCenter,
        y,
        color);

    Display_DrawLine(
        leftCenter,
        y + height - 1,
        rightCenter,
        y + height - 1,
        color);

    // Straight left and right edges
    Display_DrawLine(
        x,
        topCenter,
        x,
        bottomCenter,
        color);

    Display_DrawLine(
        x + width - 1,
        topCenter,
        x + width - 1,
        bottomCenter,
        color);

    int16_t circleX = radius;
    int16_t circleY = 0;
    int16_t error = 1 - radius;

    while (circleX >= circleY)
    {
        // Top-left corner
        Display_DrawPixel(
            leftCenter - circleX,
            topCenter - circleY,
            color);

        Display_DrawPixel(
            leftCenter - circleY,
            topCenter - circleX,
            color);

        // Top-right corner
        Display_DrawPixel(
            rightCenter + circleX,
            topCenter - circleY,
            color);

        Display_DrawPixel(
            rightCenter + circleY,
            topCenter - circleX,
            color);

        // Bottom-left corner
        Display_DrawPixel(
            leftCenter - circleX,
            bottomCenter + circleY,
            color);

        Display_DrawPixel(
            leftCenter - circleY,
            bottomCenter + circleX,
            color);

        // Bottom-right corner
        Display_DrawPixel(
            rightCenter + circleX,
            bottomCenter + circleY,
            color);

        Display_DrawPixel(
            rightCenter + circleY,
            bottomCenter + circleX,
            color);

        circleY++;

        if (error < 0)
        {
            error +=
                (2 * circleY) + 1;
        }
        else
        {
            circleX--;

            error +=
                (2 * (circleY - circleX)) + 1;
        }
    }
}

void Display_FillRoundRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    int16_t radius,
    uint16_t color)
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
            color);

        return;
    }

    const int16_t maxRadius =
        ((width < height) ? width : height) / 2;

    if (radius > maxRadius)
    {
        radius = maxRadius;
    }

    // Fill the rectangular center section.
    Display_FillRect(
        x,
        y + radius,
        width,
        height - (radius * 2),
        color);

    for (int16_t row = 0; row < radius; row++)
    {
        const int16_t circleY =
            radius - row;

        const int16_t circleX =
            static_cast<int16_t>(
                sqrt(
                    static_cast<float>(
                        (radius * radius) -
                        (circleY * circleY))));

        const int16_t lineX =
            x + radius - circleX;

        const int16_t lineWidth =
            width -
            (radius * 2) +
            (circleX * 2);

        // Top rounded section
        Display_DrawLine(
            lineX,
            y + row,
            lineX + lineWidth - 1,
            y + row,
            color);

        // Bottom rounded section
        Display_DrawLine(
            lineX,
            y + height - row - 1,
            lineX + lineWidth - 1,
            y + height - row - 1,
            color);
    }
}