#include "NetworkPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

#include "NetworkService.h"
#include "MQTTService.h"

namespace
{
    struct NetworkData
    {
        String text1 = "Test One";
        String text2 = "Test Two";
        String text3 = "Test Three";
        String text4 = "Test Four";
        String text5 = "Test Five";
        String text6 = "10.0.0.50";
    };

    NetworkData g_networkData;
}

namespace NetworkPage
{
    void draw()
    {
        const bool mqttConnected =
            MQTTService::isConnected();

        g_networkData.text5 =
            mqttConnected
                ? "Connected"
                : "Offline";

        const bool wifiConnected =
            NetworkService::isConnected();

        g_networkData.text4 =
            wifiConnected
                ? "Connected"
                : "Offline";

        if (wifiConnected)
        {
            g_networkData.text1 =
                NetworkService::ssid();
        }
        else
        {
            g_networkData.text1 =
                "-";
        }

        if (wifiConnected)
        {
            g_networkData.text2 =
                NetworkService::ipAddress();
        }
        else
        {
            g_networkData.text2 =
                "-";
        }

        if (wifiConnected)
        {
            char rssiText[16];

            snprintf(
                rssiText,
                sizeof(rssiText),
                "%ld dBm",
                static_cast<long>(
                    NetworkService::rssi()));

            g_networkData.text3 =
                rssiText;
        }
        else
        {
            g_networkData.text3 =
                "-";
        }

        const Display_TableRow rows[] =
            {
                {"SSID",
                 g_networkData.text1.c_str(),
                 wifiConnected
                     ? Color::Cyan
                     : Color::Red},
                {"IP",
                 g_networkData.text2.c_str(),
                 Color::Cyan},
                {"RSSI",
                 g_networkData.text3.c_str(),
                 Color::Yellow},
                {"Wi-Fi",
                 g_networkData.text4.c_str(),
                 wifiConnected
                     ? Color::Green
                     : Color::Red},
                {"MQTT",
                 g_networkData.text5.c_str(),
                 mqttConnected
                     ? Color::Green
                     : Color::Red},
                {"Broker",
                 g_networkData.text6.c_str(),
                 Color::Cyan}};

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Network Page",
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

        LOG("Displayed Network page.");
    }
}