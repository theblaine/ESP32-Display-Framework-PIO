#include "DeathStarPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

#include <ArduinoJson.h>

#include "MQTTService.h"

namespace
{
    struct DeathStarData
    {
        String text1 = "Test One";
        String text2 = "Test Two";
        String text3 = "Test Three";
        String text4 = "Test Four";
        String text5 = "Test Five";
        String text6 = "Test Six";
    };

    DeathStarData g_deathStarData;
}

namespace DeathStarPage
{

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
                "DeathStar JSON parse failed: %s",
                error.c_str());

            return;
        }

        g_deathStarData.text1 =
            document["hostname"] |
            "Unknown";

        g_deathStarData.text2 =
            document["internal_ip"] |
            "-";

        g_deathStarData.text3 =
            document["external_ip"] |
            "-";

        g_deathStarData.text4 =
            document["vpn_status"] |
            "Unknown";

        g_deathStarData.text5 =
            document["timestamp"] |
            "-";

        g_deathStarData.text6 =
            "Online";

        LOG("DeathStar status updated.");
    }

    void draw()
    {
        MQTTService::publish(
            "home/deathstar/ping",
            "status");

        const Display_TableRow rows[] =
            {
                {"Computer",
                 g_deathStarData.text1.c_str()},
                {"Int IP",
                 g_deathStarData.text2.c_str()},
                {"Ext IP",
                 g_deathStarData.text3.c_str()},
                {"VPN",
                 g_deathStarData.text4.c_str()},
                {"Last Seen",
                 g_deathStarData.text5.c_str()},
                {"Status",
                 g_deathStarData.text6.c_str()},
            };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "DeathStar",
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

        LOG("Displayed DeathStar page.");
    }
}