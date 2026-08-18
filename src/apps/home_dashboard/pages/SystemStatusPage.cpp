#include "SystemStatusPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

#include "NetworkService.h"
#include "MQTTService.h"
#include "TimeService.h"

namespace
{
    struct SystemStatusData
    {
        String cpu = "18%";
        String memory = "42%";
        String disk = "61%";
        String uptime = "3d 14h";
        String wifi = "Connected";
        String mqtt = "Connected";
    };

    SystemStatusData g_systemStatusData;
}

namespace SystemStatusPage
{
    void draw()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        const bool timeSynced =
            TimeService::isSynced();

        g_systemStatusData.wifi =
            wifiConnected
                ? "Connected"
                : "Offline";

        g_systemStatusData.mqtt =
            mqttConnected
                ? "Connected"
                : "Offline";

        const String currentTime =
            TimeService::timeString();

        const String currentDate =
            TimeService::dateString();

        const uint16_t wifiColor =
            wifiConnected
                ? Color::Green
                : Color::Red;

        const uint16_t mqttColor =
            mqttConnected
                ? Color::Green
                : Color::Red;

        const uint16_t timeColor =
            timeSynced
                ? Color::Cyan
                : Color::Yellow;

        const Display_TableRow rows[] =
            {
                {"CPU",
                 g_systemStatusData.cpu.c_str(),
                 Color::Cyan},

                {"Memory",
                 g_systemStatusData.memory.c_str(),
                 Color::Cyan},

                {"Disk",
                 g_systemStatusData.disk.c_str(),
                 Color::Yellow},

                {"Uptime",
                 g_systemStatusData.uptime.c_str(),
                 Color::White},

                {"Time",
                 currentTime.c_str(),
                 timeColor},

                {"Date",
                 currentDate.c_str(),
                 timeColor},

                {"Wi-Fi",
                 g_systemStatusData.wifi.c_str(),
                 wifiColor},

                {"MQTT",
                 g_systemStatusData.mqtt.c_str(),
                 mqttColor}};

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "System Status",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            30,
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

        LOG("Displayed System Status page.");
    }
}