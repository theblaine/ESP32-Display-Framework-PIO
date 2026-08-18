#include <Arduino.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Text.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "NetworkService.h"
#include "TimeService.h"
#include "secrets.h"

namespace
{
    unsigned long g_lastDrawTime = 0;

    constexpr unsigned long REFRESH_INTERVAL_MS =
        5000;

    void drawClock()
    {
        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "NTP Clock",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawText(
            12,
            58,
            "TIME",
            Color::White,
            Color::Black,
            1);

        Display_DrawTextCentered(
            Display::width() / 2,
            82,
            TimeService::timeString().c_str(),
            Color::Cyan,
            Color::Black,
            2);

        Display_DrawText(
            12,
            132,
            "DATE",
            Color::White,
            Color::Black,
            1);

        Display_DrawTextCentered(
            Display::width() / 2,
            156,
            TimeService::dateString().c_str(),
            Color::Yellow,
            Color::Black,
            1);

        Display_DrawText(
            12,
            205,
            "NTP",
            Color::White,
            Color::Black,
            1);

        Display_DrawStatusIndicator(
            12,
            222,
            148,
            28,
            TimeService::isSynced()
                ? "Synced"
                : "Waiting",
            TimeService::isSynced()
                ? Color::Green
                : Color::Red,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        Display_DrawText(
            12,
            270,
            "Wi-Fi",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            70,
            270,
            NetworkService::isConnected()
                ? "Connected"
                : "Offline",
            NetworkService::isConnected()
                ? Color::Green
                : Color::Red,
            Color::Black,
            1);
    }
}

void setup()
{
    Logger::begin();

    Display::begin();

    Display_FillScreen(
        Color::Black);

    LOG("NTP display demo starting.");

    NetworkService::addNetwork(
        WIFI_SSID_1,
        WIFI_PASSWORD_1);

#ifdef WIFI_SSID_2
    NetworkService::addNetwork(
        WIFI_SSID_2,
        WIFI_PASSWORD_2);
#endif

#ifdef WIFI_SSID_3
    NetworkService::addNetwork(
        WIFI_SSID_3,
        WIFI_PASSWORD_3);
#endif

    NetworkService::begin();

    LOG("Waiting for Wi-Fi...");

    while (!NetworkService::isConnected())
    {
        NetworkService::loop();
        delay(100);
    }

    LOGF(
        "Wi-Fi connected | IP: %s",
        NetworkService::ipAddress().c_str());

    TimeService::begin(
        "PST8PDT,M3.2.0,M11.1.0");

    // TimeService needs no loop call; ESP32 synchronizes SNTP in the background.
    LOG("Waiting for NTP sync...");

    while (!TimeService::isSynced())
    {
        delay(250);
    }

    LOG("Time synchronized.");

    drawClock();

    g_lastDrawTime =
        millis();
}

void loop()
{
    NetworkService::loop();

    if (millis() - g_lastDrawTime >=
        REFRESH_INTERVAL_MS)
    {
        drawClock();

        g_lastDrawTime =
            millis();
    }

    delay(1);
}
