#include <Arduino.h>

#include "Display_GFX.h"
#include "Display_ST7789.h"
#include "Logger.h"

namespace
{
    constexpr unsigned long DEMO_DURATION_MS = 3000;

    enum class DisplayDemo
    {
        SolidColors,
        FilledRectangles,
        Pixels,
        HorizontalLines,
        VerticalLines,
        Rectangles,
        Lines,
        Triangles,
        Circles,
    };

    DisplayDemo currentDemo = DisplayDemo::Lines;
    unsigned long lastDemoChange = 0;

    void showSolidColors()
    {
        static size_t colorIndex = 0;

        constexpr uint16_t COLORS[] = {
            Color::Black,
            Color::Red,
            Color::Green,
            Color::Blue,
            Color::Yellow,
            Color::Cyan,
            Color::Magenta,
            Color::White,
        };

        constexpr const char *COLOR_NAMES[] = {
            "Black",
            "Red",
            "Green",
            "Blue",
            "Yellow",
            "Cyan",
            "Magenta",
            "White",
        };

        constexpr size_t COLOR_COUNT =
            sizeof(COLORS) / sizeof(COLORS[0]);

        Display_FillScreen(COLORS[colorIndex]);

        LOGF(
            "Display demo: Solid color | %s",
            COLOR_NAMES[colorIndex]);

        colorIndex++;

        if (colorIndex >= COLOR_COUNT)
        {
            colorIndex = 0;
            currentDemo = DisplayDemo::FilledRectangles;
        }
    }

    void showFilledRectangles()
    {
        Display_FillScreen(Color::Black);

        Display_FillRect(
            10,
            10,
            60,
            50,
            Color::Yellow);

        Display_FillRect(
            100,
            10,
            60,
            50,
            Color::Red);

        Display_FillRect(
            40,
            90,
            90,
            60,
            Color::Blue);

        Display_FillRect(
            10,
            180,
            60,
            50,
            Color::Cyan);

        Display_FillRect(
            100,
            180,
            60,
            50,
            Color::Green);

        LOG("Display demo: Filled rectangles");

        currentDemo = DisplayDemo::Pixels;
    }

    void showPixels()
    {
        Display_FillScreen(Color::Black);

        for (int16_t y = 20; y < LCD_HEIGHT - 20; y += 10)
        {
            for (int16_t x = 10; x < LCD_WIDTH - 10; x += 10)
            {
                uint16_t color = Color::White;

                if ((x / 10 + y / 10) % 3 == 0)
                {
                    color = Color::Red;
                }
                else if ((x / 10 + y / 10) % 3 == 1)
                {
                    color = Color::Green;
                }
                else
                {
                    color = Color::Blue;
                }

                Display_DrawPixel(x, y, color);
            }
        }

        LOG("Display demo: Pixels");

        currentDemo = DisplayDemo::HorizontalLines;
    }

    void showHorizontalLines()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount = sizeof(colors) / sizeof(colors[0]);

        int colorIndex = 0;

        for (int16_t y = 10; y < LCD_HEIGHT; y += 8)
        {
            Display_DrawFastHLine(
                10,
                y,
                LCD_WIDTH - 20,
                colors[colorIndex]);

            colorIndex++;

            if (colorIndex >= colorCount)
            {
                colorIndex = 0;
            }
        }

        LOG("Display demo: Horizontal Lines");

        currentDemo = DisplayDemo::VerticalLines;
    }

    void showVerticalLines()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount =
            sizeof(colors) / sizeof(colors[0]);

        size_t colorIndex = 0;

        for (int16_t x = 10; x < LCD_WIDTH; x += 8)
        {
            Display_DrawFastVLine(
                x,
                10,
                LCD_HEIGHT - 20,
                colors[colorIndex]);

            colorIndex++;

            if (colorIndex >= colorCount)
            {
                colorIndex = 0;
            }
        }

        LOG("Display demo: Vertical Lines");

        currentDemo = DisplayDemo::Rectangles;
    }

    void showRectangles()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount =
            sizeof(colors) / sizeof(colors[0]);

        int16_t x = 10;
        int16_t y = 10;
        int16_t width = LCD_WIDTH - 20;
        int16_t height = LCD_HEIGHT - 20;

        size_t colorIndex = 0;

        while (width > 10 && height > 10)
        {
            Display_DrawRect(
                x,
                y,
                width,
                height,
                colors[colorIndex]);

            x += 8;
            y += 8;
            width -= 16;
            height -= 16;

            colorIndex++;

            if (colorIndex >= colorCount)
            {
                colorIndex = 0;
            }
        }

        LOG("Display demo: Rectangles");

        currentDemo = DisplayDemo::Lines;
    }

    void showLines()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount =
            sizeof(colors) / sizeof(colors[0]);

        constexpr int16_t spacing = 12;

        const int16_t centerX = LCD_WIDTH / 2;
        const int16_t centerY = LCD_HEIGHT / 2;

        size_t colorIndex = 0;

        // Center to top and bottom edges.
        for (int16_t x = 0; x < LCD_WIDTH; x += spacing)
        {
            Display_DrawLine(
                centerX,
                centerY,
                x,
                0,
                colors[colorIndex]);

            colorIndex = (colorIndex + 1) % colorCount;

            Display_DrawLine(
                centerX,
                centerY,
                x,
                LCD_HEIGHT - 1,
                colors[colorIndex]);

            colorIndex = (colorIndex + 1) % colorCount;
        }

        // Center to left and right edges.
        for (int16_t y = 0; y < LCD_HEIGHT; y += spacing)
        {
            Display_DrawLine(
                centerX,
                centerY,
                0,
                y,
                colors[colorIndex]);

            colorIndex = (colorIndex + 1) % colorCount;

            Display_DrawLine(
                centerX,
                centerY,
                LCD_WIDTH - 1,
                y,
                colors[colorIndex]);

            colorIndex = (colorIndex + 1) % colorCount;
        }

        LOG("Display demo: Lines");

        currentDemo = DisplayDemo::Triangles;
    }

    void showTriangles()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount =
            sizeof(colors) / sizeof(colors[0]);

        const int16_t centerX = LCD_WIDTH / 2;

        int16_t leftX = 10;
        int16_t rightX = LCD_WIDTH - 11;
        int16_t topY = 10;
        int16_t bottomY = LCD_HEIGHT - 11;

        size_t colorIndex = 0;

        while (leftX < rightX && topY < bottomY)
        {
            Display_DrawTriangle(
                centerX,
                topY,
                leftX,
                bottomY,
                rightX,
                bottomY,
                colors[colorIndex]);

            leftX += 8;
            rightX -= 8;
            topY += 8;
            bottomY -= 8;

            colorIndex++;

            if (colorIndex >= colorCount)
            {
                colorIndex = 0;
            }
        }

        LOG("Display demo: Triangles");

        currentDemo = DisplayDemo::Circles;
    }

    void showCircles()
    {
        Display_FillScreen(Color::Black);

        constexpr uint16_t colors[] =
            {
                Color::Red,
                Color::Green,
                Color::Blue,
                Color::Yellow,
                Color::Cyan,
                Color::Magenta,
                Color::White};

        constexpr size_t colorCount =
            sizeof(colors) / sizeof(colors[0]);

        const int16_t centerX = LCD_WIDTH / 2;
        const int16_t centerY = LCD_HEIGHT / 2;

        const int16_t maximumRadius =
            min(LCD_WIDTH, LCD_HEIGHT) / 2 - 5;

        size_t colorIndex = 0;

        for (int16_t radius = maximumRadius;
             radius >= 5;
             radius -= 8)
        {
            Display_DrawCircle(
                centerX,
                centerY,
                radius,
                colors[colorIndex]);

            colorIndex++;

            if (colorIndex >= colorCount)
            {
                colorIndex = 0;
            }
        }

        LOG("Display demo: Circles");

        currentDemo = DisplayDemo::Lines;
    }

    void showCurrentDemo()
    {
        switch (currentDemo)
        {
        case DisplayDemo::SolidColors:
            showSolidColors();
            break;

        case DisplayDemo::FilledRectangles:
            showFilledRectangles();
            break;

        case DisplayDemo::Pixels:
            showPixels();
            break;

        case DisplayDemo::HorizontalLines:
            showHorizontalLines();
            break;

        case DisplayDemo::VerticalLines:
            showVerticalLines();
            break;

        case DisplayDemo::Rectangles:
            showRectangles();
            break;

        case DisplayDemo::Lines:
            showLines();
            break;

        case DisplayDemo::Triangles:
            showTriangles();
            break;

        case DisplayDemo::Circles:
            showCircles();
            break;
        }
    }

}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 Display Demo");
    LOG("=====================================");
    LOGF("Display size: %d x %d", LCD_WIDTH, LCD_HEIGHT);
    LOGF("Demo duration: %lu ms", DEMO_DURATION_MS);

    LOG("Initializing display.");
    LCD_Init();

    showCurrentDemo();
    lastDemoChange = millis();
}

void loop()
{
    if (millis() - lastDemoChange < DEMO_DURATION_MS)
    {
        return;
    }

    lastDemoChange = millis();
    showCurrentDemo();
}