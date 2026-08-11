#include "SystemMonitorPage.h"

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
    struct SystemMonitorData
    {
        uint8_t cpu = 18;
        uint8_t memory = 42;
        uint8_t disk = 61;

        String uptime = "3d 14h";
        String wifi = "Connected";
        String mqtt = "Connected";
    };

    SystemMonitorData g_systemMonitorData;

    /*
     * Draws one utilization row consisting of:
     *
     *   label
     *   percentage
     *   progress bar
     *
     * This helper is intentionally page-specific rather than a framework
     * widget because it simply composes existing Display_Text and
     * Display_Widgets functionality.
     */
    void drawUsageRow(
        int16_t y,
        const char *label,
        uint8_t percent,
        uint16_t color)
    {
        char percentText[8];

        snprintf(
            percentText,
            sizeof(percentText),
            "%u%%",
            percent);

        Display_DrawText(
            10,
            y,
            label,
            Color::White,
            Color::Black,
            1);

        Display_DrawTextRight(
            Display::width() - 10,
            y,
            percentText,
            color,
            Color::Black,
            1);

        Display_DrawProgressBar(
            10,
            y + 13,
            Display::width() - 20,
            12,
            percent,
            Color::Black,
            color,
            Color::White,
            3);
    }
}

namespace SystemMonitorPage
{
    void draw()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        g_systemMonitorData.wifi =
            wifiConnected
                ? "Connected"
                : "Offline";

        g_systemMonitorData.mqtt =
            mqttConnected
                ? "Connected"
                : "Offline";

        const uint16_t wifiColor =
            wifiConnected
                ? Color::Green
                : Color::Red;

        const uint16_t mqttColor =
            mqttConnected
                ? Color::Green
                : Color::Red;

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "System Monitor",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        // Resource utilization.
        drawUsageRow(
            48,
            "CPU",
            g_systemMonitorData.cpu,
            Color::Cyan);

        drawUsageRow(
            88,
            "Memory",
            g_systemMonitorData.memory,
            Color::Cyan);

        drawUsageRow(
            128,
            "Disk",
            g_systemMonitorData.disk,
            Color::Yellow);

        // Uptime summary.
        Display_DrawLabelValue(
            10,
            168,
            Display::width() - 20,
            42,
            "UPTIME",
            g_systemMonitorData.uptime.c_str(),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1,
            2,
            4);

        // Side-by-side connectivity status tiles.
        const int16_t tileGap = 6;
        const int16_t tileWidth =
            (Display::width() - 20 - tileGap) / 2;

        const int16_t leftTileX = 10;
        const int16_t rightTileX =
            leftTileX + tileWidth + tileGap;

        const int16_t tileY = 216;
        const int16_t tileHeight = 60;

        // Wi-Fi tile.
        Display_DrawPanel(
            leftTileX,
            tileY,
            tileWidth,
            tileHeight,
            Color::Black,
            Color::White,
            4);

        Display_DrawStatusIndicator(
            leftTileX + 4,
            tileY + 4,
            tileWidth - 8,
            24,
            "WI-FI",
            wifiColor,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        Display_DrawTextCentered(
            leftTileX + (tileWidth / 2),
            tileY + 38,
            wifiConnected
                ? "CONNECTED"
                : "OFFLINE",
            wifiColor,
            Color::Black,
            1);

        // MQTT tile.
        Display_DrawPanel(
            rightTileX,
            tileY,
            tileWidth,
            tileHeight,
            Color::Black,
            Color::White,
            4);

        Display_DrawStatusIndicator(
            rightTileX + 4,
            tileY + 4,
            tileWidth - 8,
            24,
            "MQTT",
            mqttColor,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        Display_DrawTextCentered(
            rightTileX + (tileWidth / 2),
            tileY + 38,
            mqttConnected
                ? "CONNECTED"
                : "OFFLINE",
            mqttColor,
            Color::Black,
            1);

        RGBLamp::setColor(
            64,
            0,
            64);

        LOG("Displayed System Monitor page.");
    }
}