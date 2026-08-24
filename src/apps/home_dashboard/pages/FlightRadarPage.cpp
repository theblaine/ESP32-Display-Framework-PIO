#include "FlightRadarPage.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

namespace
{
    struct FlightRadarData
    {
        String feed = "Online";
        String aircraft = "18";
        String mlat = "Yes";
        String updated = "5 sec";
    };

    FlightRadarData g_data;

    unsigned long g_lastUpdate = 0;

    uint16_t feedColor(
        const String &state)
    {
        if (state == "Online")
        {
            return Color::Green;
        }

        if (state == "Offline")
        {
            return Color::Red;
        }

        return Color::White;
    }

    uint16_t mlatColor(
        const String &state)
    {
        if (state == "Yes")
        {
            return Color::Green;
        }

        if (state == "No")
        {
            return Color::Red;
        }

        return Color::White;
    }
}

namespace FlightRadarPage
{
    void draw()
    {
        const Display_TableRow rows[] =
            {
                {"Feed",
                 g_data.feed.c_str(),
                 feedColor(
                     g_data.feed)},
                {"Aircraft",
                 g_data.aircraft.c_str(),
                 Color::Cyan},
                {"MLAT",
                 g_data.mlat.c_str(),
                 mlatColor(
                     g_data.mlat)},
                {"Updated",
                 g_data.updated.c_str(),
                 Color::White}
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

        RGBLamp::setColor(
            0,
            32,
            64);

        LOG("Displayed FlightRadar24 page.");
    }

    void handleMessage(
        const char *payload)
    {
        if (payload == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                payload);

        if (error)
        {
            LOGWF(
                "FlightRadar JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *feed =
            document["feed"];

        const char *aircraft =
            document["aircraft"];

        const char *mlat =
            document["mlat"];

        const char *updated =
            document["updated"];

        if (feed != nullptr)
        {
            g_data.feed =
                feed;
        }

        if (aircraft != nullptr)
        {
            g_data.aircraft =
                aircraft;
        }

        if (mlat != nullptr)
        {
            g_data.mlat =
                mlat;
        }

        if (updated != nullptr)
        {
            g_data.updated =
                updated;
        }

        g_lastUpdate =
            millis();

        LOGF(
            "FlightRadar updated | Feed: %s | Aircraft: %s | MLAT: %s | Updated: %s",
            g_data.feed.c_str(),
            g_data.aircraft.c_str(),
            g_data.mlat.c_str(),
            g_data.updated.c_str());
    }

    bool hasReceivedData()
    {
        return g_lastUpdate != 0;
    }
}