#include "HomeAssistantPage.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

namespace
{
    struct HomeAssistantData
    {
        String sparePlug13 = "Unknown";
        String alarm = "Disarmed";
        String indoorTemperature = "72 F";
        String humidity = "41%";
    };

    HomeAssistantData g_data;

    unsigned long g_lastUpdate = 0;

    uint16_t plugColor(
        const String &state)
    {
        if (state == "ON")
        {
            return Color::Green;
        }

        if (state == "OFF")
        {
            return Color::Red;
        }

        return Color::White;
    }

    uint16_t alarmColor(
        const String &state)
    {
        if (state == "Disarmed")
        {
            return Color::Red;
        }

        if (state == "Armed Home")
        {
            return Color::Green;
        }

        if (state == "Armed Away")
        {
            return Color::Green;
        }

        if (state == "Triggered")
        {
            return Color::Yellow;
        }

        return Color::White;
    }
}

namespace HomeAssistantPage
{
    void draw()
    {
        const Display_TableRow rows[] =
            {
                {"Plug 13",
                 g_data.sparePlug13.c_str(),
                 plugColor(
                     g_data.sparePlug13)},
                {"Alarm",
                 g_data.alarm.c_str(),
                 alarmColor(
                     g_data.alarm)},
                {"Indoor",
                 g_data.indoorTemperature.c_str(),
                 Color::Cyan},
                {"Humidity",
                 g_data.humidity.c_str(),
                 Color::Cyan}
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

        RGBLamp::setColor(
            0,
            64,
            0);

        LOG("Displayed Home Assistant page.");
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
                "Home Assistant JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *sparePlug13 =
            document["sparePlug13"];

        if (sparePlug13 != nullptr)
        {
            g_data.sparePlug13 =
                sparePlug13;
        }

        g_lastUpdate =
            millis();

        LOGF(
            "Home Assistant updated | Spare Plug 13: %s",
            g_data.sparePlug13.c_str());
    }

    bool hasReceivedData()
    {
        return g_lastUpdate != 0;
    }
}