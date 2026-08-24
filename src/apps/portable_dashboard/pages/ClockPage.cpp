#include "ClockPage.h"

#include <Arduino.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "TimeService.h"


namespace ClockPage
{
    void draw()
    {
        const bool compactLayout =
            Display::height() < 300;

        const bool timeSynced =
            TimeService::isSynced();

        const String currentTime =
            TimeService::timeString();

        const String currentDate =
            TimeService::dateString();

        const int16_t headerHeight =
            compactLayout ? 30 : 34;

        const int16_t cardX =
            compactLayout ? 6 : 10;

        const int16_t cardWidth =
            Display::width() - (cardX * 2);

        const int16_t timeY =
            compactLayout ? 40 : 50;

        const int16_t timeHeight =
            compactLayout ? 66 : 84;

        const int16_t dateY =
            compactLayout ? 112 : 146;

        const int16_t dateHeight =
            compactLayout ? 54 : 64;

        const int16_t statusY =
            compactLayout ? 174 : 224;

        const int16_t statusHeight =
            compactLayout ? 34 : 42;

        const int16_t footerHeight =
            compactLayout ? 24 : 28;

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "CLOCK",
            Color::Blue,
            Color::Cyan,
            Color::White,
            2,
            headerHeight);

        Display_DrawLabelValue(
            cardX,
            timeY,
            cardWidth,
            timeHeight,
            "TIME",
            currentTime.c_str(),
            Color::Black,
            Color::Cyan,
            Color::White,
            Color::Cyan,
            1,
            compactLayout ? 2 : 3,
            8);

        Display_DrawLabelValue(
            cardX,
            dateY,
            cardWidth,
            dateHeight,
            "DATE",
            currentDate.c_str(),
            Color::Black,
            Color::White,
            Color::White,
            Color::Yellow,
            1,
            2,
            8);

        Display_DrawStatusIndicator(
            cardX,
            statusY,
            cardWidth,
            statusHeight,
            timeSynced ? "SYNCED" : "WAITING",
            timeSynced ? Color::Green : Color::Yellow,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            2,
            true,
            Color::White);

        Display_DrawFooterBar(
            "AUTO 10S  2/3",
            Color::Blue,
            Color::Cyan,
            Color::White,
            1,
            footerHeight);

    }

}
