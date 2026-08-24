#include "PiHolePage.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

namespace
{
    struct PiHoleData
    {
        String status = "Online";
        String queries = "12,438";
        String blocked = "8.4%";
        String clients = "27";
    };

    PiHoleData g_data;

    unsigned long g_lastUpdate = 0;

    uint16_t statusColor(
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
}

namespace PiHolePage
{
    void draw()
    {
        const Display_TableRow rows[] =
            {
                {"Status",
                 g_data.status.c_str(),
                 statusColor(
                     g_data.status)},
                {"Queries",
                 g_data.queries.c_str(),
                 Color::Cyan},
                {"Blocked",
                 g_data.blocked.c_str(),
                 Color::Yellow},
                {"Clients",
                 g_data.clients.c_str(),
                 Color::White}
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

        RGBLamp::setColor(
            64,
            0,
            64);

        LOG("Displayed Pi-hole page.");
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
                "Pi-hole JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *status =
            document["status"];

        const char *queries =
            document["queries"];

        const char *blocked =
            document["blocked"];

        const char *clients =
            document["clients"];

        if (status != nullptr)
        {
            g_data.status =
                status;
        }

        if (queries != nullptr)
        {
            g_data.queries =
                queries;
        }

        if (blocked != nullptr)
        {
            g_data.blocked =
                blocked;
        }

        if (clients != nullptr)
        {
            g_data.clients =
                clients;
        }

        g_lastUpdate =
            millis();

        LOGF(
            "Pi-hole updated | Status: %s | Queries: %s | Blocked: %s | Clients: %s",
            g_data.status.c_str(),
            g_data.queries.c_str(),
            g_data.blocked.c_str(),
            g_data.clients.c_str());
    }

    bool hasReceivedData()
    {
        return g_lastUpdate != 0;
    }
}