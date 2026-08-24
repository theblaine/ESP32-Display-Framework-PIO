#include "NetworkPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Text.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

#include "NetworkService.h"
#include "MQTTService.h"

namespace
{
    struct NetworkData
    {
        String ssid;
        String ip;
        String rssi;
        String wifi;
        String mqtt;
        String broker = "10.0.0.50";
    };

    NetworkData g_networkData;
}

namespace NetworkPage
{
    void draw()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        /*
         * Wi-Fi connection information.
         */
        if (wifiConnected)
        {
            g_networkData.ssid =
                NetworkService::ssid();

            g_networkData.ip =
                NetworkService::ipAddress();

            char rssiText[16];

            snprintf(
                rssiText,
                sizeof(rssiText),
                "%ld dBm",
                static_cast<long>(
                    NetworkService::rssi()));

            g_networkData.rssi =
                rssiText;
        }
        else
        {
            g_networkData.ssid = "-";
            g_networkData.ip = "-";
            g_networkData.rssi = "-";
        }

        g_networkData.wifi =
            wifiConnected
                ? "Connected"
                : "Offline";

        g_networkData.mqtt =
            mqttConnected
                ? "Connected"
                : "Offline";

        /*
         * Convert RSSI into a simple 0-4 signal level.
         *
         * The Display_DrawSignalMeter() widget remains generic;
         * NetworkPage owns the Wi-Fi-specific RSSI thresholds.
         */
        uint8_t signalLevel = 0;

        if (wifiConnected)
        {
            const int32_t rssi =
                NetworkService::rssi();

            if (rssi >= -55)
            {
                signalLevel = 4;
            }
            else if (rssi >= -65)
            {
                signalLevel = 3;
            }
            else if (rssi >= -75)
            {
                signalLevel = 2;
            }
            else if (rssi >= -85)
            {
                signalLevel = 1;
            }
        }

        const uint16_t wifiColor =
            wifiConnected
                ? Color::Green
                : Color::Red;

        const uint16_t mqttColor =
            mqttConnected
                ? Color::Green
                : Color::Red;

        const Display_TableRow rows[] =
            {
                {"SSID",
                 g_networkData.ssid.c_str(),
                 Color::Cyan},

                {"IP",
                 g_networkData.ip.c_str(),
                 Color::Cyan},

                {"RSSI",
                 g_networkData.rssi.c_str(),
                 Color::Yellow},

                {"Wi-Fi",
                 g_networkData.wifi.c_str(),
                 wifiColor},

                {"MQTT",
                 g_networkData.mqtt.c_str(),
                 mqttColor},

                {"Broker",
                 g_networkData.broker.c_str(),
                 Color::Cyan}
            };

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "Network",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        /*
         * Slightly shorter rows leave room for the signal meter
         * while preserving all six network values.
         */
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

        /*
         * Real Wi-Fi signal strength visualization.
         */
        Display_DrawText(
            10,
            250,
            "SIGNAL",
            Color::White,
            Color::Black,
            1);

        Display_DrawSignalMeter(
            70,
            246,
            82,
            30,
            signalLevel,
            4,
            Color::Green,
            0x4208,
            Color::Black);

        RGBLamp::setColor(
            0,
            64,
            64);

        LOG("Displayed Network page.");
    }
}