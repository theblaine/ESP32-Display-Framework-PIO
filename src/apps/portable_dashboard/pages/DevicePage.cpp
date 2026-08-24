#include "DevicePage.h"

#include <Arduino.h>

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "MQTTService.h"
#include "NetworkService.h"

namespace
{
    constexpr unsigned long RSSI_REFRESH_INTERVAL_MS = 5000;
    constexpr int32_t RSSI_CHANGE_THRESHOLD_DBM = 4;

    bool g_hasDisplayedState = false;
    bool g_displayedWifiConnected = false;
    bool g_displayedMqttConnected = false;
    String g_displayedIpAddress;
    int32_t g_displayedRssi = 0;
    unsigned long g_lastRssiDrawTime = 0;
}

namespace DevicePage
{
    void draw()
    {
        const bool compactLayout =
            Display::height() < 300;

        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        const String ipAddress =
            wifiConnected
                ? NetworkService::ipAddress()
                : "-";

        const int32_t rssi =
            wifiConnected
                ? NetworkService::rssi()
                : 0;

        char rssiText[16] = "-";

        if (wifiConnected)
        {
            snprintf(
                rssiText,
                sizeof(rssiText),
                "%ld dBm",
                static_cast<long>(rssi));
        }

        const Display_TableRow rows[] =
            {
                {"WiFi",
                 wifiConnected ? "ONLINE" : "OFFLINE",
                 wifiConnected ? Color::Green : Color::Red},

                {"IP",
                 ipAddress.c_str(),
                 wifiConnected ? Color::Cyan : Color::White},

                {"RSSI",
                 rssiText,
                 wifiConnected ? Color::Yellow : Color::White},

                {"MQTT",
                 mqttConnected ? "ONLINE" : "OFFLINE",
                 mqttConnected ? Color::Green : Color::Red}
            };

        const int16_t headerHeight =
            compactLayout ? 30 : 34;

        const int16_t tableX =
            compactLayout ? 6 : 8;

        const int16_t tableY =
            compactLayout ? 38 : 48;

        const int16_t rowHeight =
            compactLayout ? 42 : 50;

        const int16_t labelColumnWidth =
            compactLayout ? 34 : 54;

        const int16_t footerHeight =
            compactLayout ? 24 : 28;

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            compactLayout ? "DEVICE" : "DEVICE STATUS",
            Color::Blue,
            Color::Cyan,
            Color::White,
            2,
            headerHeight);

        Display_DrawTable(
            tableX,
            tableY,
            Display::width() - (tableX * 2),
            rowHeight,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::Cyan,
            Color::White,
            Color::Cyan,
            1,
            labelColumnWidth,
            true);

        Display_DrawFooterBar(
            "AUTO 10S  1/3",
            Color::Blue,
            Color::Cyan,
            Color::White,
            1,
            footerHeight);

        g_hasDisplayedState = true;
        g_displayedWifiConnected = wifiConnected;
        g_displayedMqttConnected = mqttConnected;
        g_displayedIpAddress = ipAddress;
        g_displayedRssi = rssi;
        g_lastRssiDrawTime = millis();

        LOG("Displayed portable Device page.");
    }

    bool needsRedraw()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        const String ipAddress =
            wifiConnected
                ? NetworkService::ipAddress()
                : "-";

        if (!g_hasDisplayedState ||
            wifiConnected != g_displayedWifiConnected ||
            mqttConnected != g_displayedMqttConnected ||
            ipAddress != g_displayedIpAddress)
        {
            return true;
        }

        if (wifiConnected &&
            millis() - g_lastRssiDrawTime >=
                RSSI_REFRESH_INTERVAL_MS)
        {
            const int32_t rssiDifference =
                NetworkService::rssi() - g_displayedRssi;

            return rssiDifference >= RSSI_CHANGE_THRESHOLD_DBM ||
                   rssiDifference <= -RSSI_CHANGE_THRESHOLD_DBM;
        }

        return false;
    }
}
