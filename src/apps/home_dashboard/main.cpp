#include <Arduino.h>

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "RGB_lamp.h"

namespace
{
    constexpr unsigned long PAGE_DURATION_MS = 10000;

    enum class DashboardPage : uint8_t
    {
        HomeAssistant,
        FlightRadar,
        PiHole,
        Count
    };

    DashboardPage g_currentPage = DashboardPage::HomeAssistant;
    unsigned long g_lastPageChangeTime = 0;

    void drawHomeAssistantPage()
    {
        const Display_TableRow rows[] =
        {
            {"Garage", "Closed", Color::Green},
            {"Alarm", "Disarmed", Color::Green},
            {"Indoor", "72 F", Color::Cyan},
            {"Humidity", "41%", Color::Cyan}
        };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Home Assistant",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(0, 64, 0);

        LOG("Displayed Home Assistant page.");
    }

    void drawFlightRadarPage()
    {
        const Display_TableRow rows[] =
        {
            {"Feed", "Online", Color::Green},
            {"Aircraft", "18", Color::Cyan},
            {"MLAT", "Yes", Color::Green},
            {"Updated", "5 sec", Color::White}
        };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "FlightRadar24",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(0, 32, 64);

        LOG("Displayed FlightRadar24 page.");
    }

    void drawPiHolePage()
    {
        const Display_TableRow rows[] =
        {
            {"Status", "Online", Color::Green},
            {"Queries", "12,438", Color::Cyan},
            {"Blocked", "8.4%", Color::Yellow},
            {"Clients", "27", Color::White}
        };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Pi-hole",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(64, 0, 64);

        LOG("Displayed Pi-hole page.");
    }

    void drawCurrentPage()
    {
        switch (g_currentPage)
        {
            case DashboardPage::HomeAssistant:
                drawHomeAssistantPage();
                break;

            case DashboardPage::FlightRadar:
                drawFlightRadarPage();
                break;

            case DashboardPage::PiHole:
                drawPiHolePage();
                break;

            case DashboardPage::Count:
                break;
        }
    }

    void advancePage()
    {
        uint8_t nextPage =
            static_cast<uint8_t>(g_currentPage) + 1;

        if (nextPage >=
            static_cast<uint8_t>(DashboardPage::Count))
        {
            nextPage = 0;
        }

        g_currentPage =
            static_cast<DashboardPage>(nextPage);

        drawCurrentPage();
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" Home Dashboard");
    LOG("=====================================");

    Display::begin();
    RGBLamp::begin();

    drawCurrentPage();

    g_lastPageChangeTime = millis();
}

void loop()
{
    const unsigned long currentTime = millis();

    if (currentTime - g_lastPageChangeTime >= PAGE_DURATION_MS)
    {
        g_lastPageChangeTime = currentTime;
        advancePage();
    }

    delay(10);
}