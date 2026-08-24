#include "MqttPage.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Text.h"
#include "Display_Widgets.h"
#include "Logger.h"

namespace
{
    String g_title = "MQTT";
    String g_status = "WAITING";
    String g_detail = "Waiting for test/display";
    uint16_t g_statusColor = Color::Yellow;

    uint16_t colorFromText(const char *colorText)
    {
        if (colorText == nullptr)
        {
            return Color::Green;
        }

        if (strcasecmp(colorText, "red") == 0)
        {
            return Color::Red;
        }

        if (strcasecmp(colorText, "yellow") == 0)
        {
            return Color::Yellow;
        }

        if (strcasecmp(colorText, "blue") == 0)
        {
            return Color::Blue;
        }

        if (strcasecmp(colorText, "cyan") == 0)
        {
            return Color::Cyan;
        }

        if (strcasecmp(colorText, "magenta") == 0)
        {
            return Color::Magenta;
        }

        if (strcasecmp(colorText, "white") == 0)
        {
            return Color::White;
        }

        return Color::Green;
    }
}

namespace MqttPage
{
    void update(const char *payload)
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
            g_title = "MQTT";
            g_status = "MESSAGE";
            g_detail = payload;
            g_statusColor = Color::Cyan;

            LOG("Portable dashboard received plain-text MQTT payload.");
            return;
        }

        g_title =
            document["title"] | "MQTT";

        g_status =
            document["status"] | "MESSAGE";

        g_detail =
            document["detail"] | "";

        g_statusColor =
            colorFromText(
                document["color"] | "green");

        LOG("Portable dashboard received JSON MQTT payload.");
    }

    void draw()
    {
        const bool compactLayout =
            Display::height() < 300;

        const int16_t headerHeight =
            compactLayout ? 30 : 34;

        const int16_t statusX =
            compactLayout ? 6 : 10;

        const int16_t statusY =
            compactLayout ? 44 : 54;

        const int16_t statusWidth =
            Display::width() - (statusX * 2);

        const int16_t statusHeight =
            compactLayout ? 50 : 58;

        const int16_t detailLabelY =
            compactLayout ? 112 : 134;

        const int16_t detailY =
            compactLayout ? 132 : 158;

        const int16_t detailX =
            compactLayout ? 6 : 10;

        const int16_t detailWidth =
            Display::width() - (detailX * 2);

        const int16_t footerHeight =
            compactLayout ? 24 : 28;

        uint8_t headerScale = 2;
        uint8_t statusScale = 2;
        uint8_t detailScale = 2;

        if (Display_GetTextWidth(
                g_title.c_str(),
                headerScale) > Display::width() - 8)
        {
            headerScale = 1;
        }

        if (compactLayout &&
            Display_GetTextWidth(
                g_status.c_str(),
                statusScale) > statusWidth - 36)
        {
            statusScale = 1;
        }

        if (Display_GetTextWidth(
                g_detail.c_str(),
                detailScale) > detailWidth)
        {
            detailScale = 1;
        }

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            g_title.c_str(),
            Color::Blue,
            Color::Cyan,
            Color::White,
            headerScale,
            headerHeight);

        Display_DrawStatusIndicator(
            statusX,
            statusY,
            statusWidth,
            statusHeight,
            g_status.c_str(),
            g_statusColor,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            statusScale,
            true,
            Color::White);

        Display_DrawText(
            detailX,
            detailLabelY,
            "DETAIL",
            Color::White,
            Color::Black,
            1);

        Display_DrawTextWrapped(
            detailX,
            detailY,
            detailWidth,
            g_detail.c_str(),
            Color::Cyan,
            Color::Black,
            detailScale);

        Display_DrawFooterBar(
            "AUTO 10S  3/3",
            Color::Blue,
            Color::Cyan,
            Color::White,
            1,
            footerHeight);

        LOG("Displayed portable MQTT page.");
    }
}
