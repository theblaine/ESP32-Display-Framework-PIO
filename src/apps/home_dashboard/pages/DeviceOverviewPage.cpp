#include "DeviceOverviewPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Text.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "PNG_Image.h"

#include "NetworkService.h"
#include "MQTTService.h"

#include <ArduinoJSON.h>

namespace
{
    String g_location = "Waiting...";
    String g_status = "Unknown";

    uint8_t getSignalLevel(
        int32_t rssi)
    {
        if (rssi >= -55)
        {
            return 4;
        }

        if (rssi >= -65)
        {
            return 3;
        }

        if (rssi >= -75)
        {
            return 2;
        }

        if (rssi >= -85)
        {
            return 1;
        }

        return 0;
    }
}

namespace DeviceOverviewPage
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
            LOGE("Device Overview JSON parse failed.");
            return;
        }

        g_location =
            document["location"] |
            "Unknown";

        g_status =
            document["status"] |
            "Unknown";

        LOGF(
            "Device Overview updated | Location: %s | Status: %s",
            g_location.c_str(),
            g_status.c_str());
    }

    void draw()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        String ip = "-";
        String rssiText = "-";

        uint8_t signalLevel = 0;

        if (wifiConnected)
        {
            ip =
                NetworkService::ipAddress();

            const int32_t rssi =
                NetworkService::rssi();

            char buffer[16];

            snprintf(
                buffer,
                sizeof(buffer),
                "%ld dBm",
                static_cast<long>(rssi));

            rssiText = buffer;

            signalLevel =
                getSignalLevel(rssi);
        }

        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "Device Overview",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        /*
         * 80x80 PNG from SD card.
         */
        PNGImage_Draw(
            "/Network_80x80.png",
            8,
            48);

        /*
         * Network information beside the image.
         */
        Display_DrawText(
            96,
            52,
            "IP",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            96,
            66,
            ip.c_str(),
            Color::Cyan,
            Color::Black,
            1);

        Display_DrawText(
            96,
            88,
            "RSSI",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            96,
            102,
            rssiText.c_str(),
            Color::Yellow,
            Color::Black,
            1);

        /*
         * Connection states.
         */
        Display_DrawText(
            10,
            144,
            "Wi-Fi",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            72,
            144,
            wifiConnected
                ? "Connected"
                : "Offline",
            wifiConnected
                ? Color::Green
                : Color::Red,
            Color::Black,
            1);

        Display_DrawText(
            10,
            164,
            "MQTT",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            72,
            164,
            mqttConnected
                ? "Connected"
                : "Offline",
            mqttConnected
                ? Color::Green
                : Color::Red,
            Color::Black,
            1);

        /*
         * MQTT-provided value.
         */
        Display_DrawText(
            10,
            194,
            "LOCATION",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            10,
            210,
            g_location.c_str(),
            Color::Cyan,
            Color::Black,
            1);

        Display_DrawText(
            10,
            230,
            "STATUS",
            Color::White,
            Color::Black,
            1);

        Display_DrawText(
            70,
            230,
            g_status.c_str(),
            g_status == "Online"
                ? Color::Green
                : Color::Red,
            Color::Black,
            1);

        /*
         * Wi-Fi signal meter.
         */
        Display_DrawText(
            10,
            242,
            "SIGNAL",
            Color::White,
            Color::Black,
            1);

        Display_DrawSignalMeter(
            70,
            238,
            82,
            30,
            signalLevel,
            4,
            Color::Green,
            0x4208,
            Color::Black);

        LOG("Displayed Device Overview page.");
    }
}