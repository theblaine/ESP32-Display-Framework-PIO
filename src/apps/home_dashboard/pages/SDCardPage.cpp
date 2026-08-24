#include "SDCardPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

#include <ArduinoJson.h>

#include "SD_Card.h"

namespace
{
    constexpr const char *STATUS_FILE =
        "/status.json";

    struct SDCardData
    {
        String name = "Unknown";
        String location = "Unknown";
        String status = "Unknown";

        uint8_t batteryPercent = 0;
    };

    SDCardData g_sdCardData;

    void loadStatusData()
    {
        /*
         * Reset the displayed values first so stale data is not
         * left on screen if the card or JSON file becomes unavailable.
         */
        g_sdCardData.name = "-";
        g_sdCardData.location = "-";
        g_sdCardData.status = "Unavailable";
        g_sdCardData.batteryPercent = 0;

        if (!SD_IsMounted())
        {
            g_sdCardData.status =
                "SD Missing";

            return;
        }

        if (!SD_FileExists(
                STATUS_FILE))
        {
            g_sdCardData.status =
                "File Missing";

            return;
        }

        String jsonContents;

        if (!SD_ReadTextFile(
                STATUS_FILE,
                jsonContents))
        {
            g_sdCardData.status =
                "Read Failed";

            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                jsonContents);

        if (error)
        {
            g_sdCardData.status =
                "JSON Error";

            LOGWF(
                "SD Card JSON parse failed: %s",
                error.c_str());

            return;
        }

        g_sdCardData.name =
            document["name"] |
            "Unknown";

        g_sdCardData.location =
            document["location"] |
            "Unknown";

        g_sdCardData.status =
            document["status"] |
            "Unknown";

        int batteryPercent =
            document["battery"] |
            0;

        if (batteryPercent < 0)
        {
            batteryPercent = 0;
        }
        else if (batteryPercent > 100)
        {
            batteryPercent = 100;
        }

        g_sdCardData.batteryPercent =
            static_cast<uint8_t>(
                batteryPercent);
    }
}

namespace SDCardPage
{
    void draw()
    {
        loadStatusData();

        const Display_TableRow rows[] =
            {
                {"Name",
                 g_sdCardData.name.c_str(),
                 Color::White},

                {"Location",
                 g_sdCardData.location.c_str(),
                 Color::White}
            };

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "SD Card",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            32,
            rows,
            sizeof(rows) /
                sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        Display_DrawBattery(
            10,
            128,
            Display::width() - 20,
            42,
            g_sdCardData.batteryPercent,
            Color::Green,
            0x4208,
            Color::White,
            Color::Black,
            true,
            Color::White,
            1);

        const bool statusOnline =
            g_sdCardData.status == "Online";

        Display_DrawStatusIndicator(
            10,
            184,
            Display::width() - 20,
            36,
            g_sdCardData.status.c_str(),
            statusOnline
                ? Color::Green
                : Color::Red,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        RGBLamp::setColor(
            64,
            32,
            0);

        LOG("Displayed SD Card page.");
    }
}