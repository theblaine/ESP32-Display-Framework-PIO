#include <Arduino.h>
#include <ArduinoJson.h>

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"

#include "NetworkService.h"
#include "MQTTService.h"

#include "secrets.h"

namespace
{
    constexpr const char *MQTT_BROKER = "10.0.0.50";
    constexpr uint16_t MQTT_PORT = 1883;

    // Generic client name because this demo now targets multiple boards.
    constexpr const char *MQTT_CLIENT_ID = "display-mqtt-demo";

    constexpr const char *MQTT_TOPIC = "test/display";

    String g_lastMqttMessage = "Waiting...";

    bool g_wasWiFiConnected = false;
    bool g_wasMqttConnected = false;

    void showStatus(
        const char *title,
        const char *statusText,
        uint16_t statusColor,
        const char *detailText = nullptr)
    {
        Display_ShowStatusScreen(
            title,
            statusText,
            statusColor,
            detailText);
    }

    uint16_t statusColorFromText(
        const char *colorText)
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

    void processMqttMessage(
        const char *message)
    {
        if (message == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                message);

        /*
         * Plain-text payloads are also valid for this demo.
         * If the payload is not JSON, display it directly.
         */
        if (error)
        {
            LOGWF(
                "JSON parse failed; displaying plain text: %s",
                error.c_str());

            g_lastMqttMessage = message;

            Display_StatusScreenData screen =
            {
                "MQTT",
                "Connected",
                g_lastMqttMessage,
                Color::Green
            };

            Display_ShowStatusScreen(screen);

            return;
        }

        const char *title =
            document["title"] | "MQTT";

        const char *status =
            document["status"] | "Connected";

        const char *detail =
            document["detail"] | "";

        const char *colorText =
            document["color"] | "green";

        const uint16_t statusColor =
            statusColorFromText(colorText);

        LOGF(
            "JSON display | Title: %s | Status: %s | Color: %s",
            title,
            status,
            colorText);

        Display_StatusScreenData screen =
        {
            title,
            status,
            detail,
            statusColor
        };

        Display_ShowStatusScreen(screen);
    }

    /*
     * MQTTService converts the PubSubClient payload into a normal
     * null-terminated string before calling this function.
     */
    void handleMqttMessage(
        const char *topic,
        const char *payload)
    {
        LOGF(
            "Demo received MQTT topic: %s",
            topic);

        processMqttMessage(payload);
    }

    void addConfiguredNetworks()
    {
        NetworkService::addNetwork(
            WIFI_SSID_1,
            WIFI_PASSWORD_1);

#ifdef WIFI_SSID_2
        NetworkService::addNetwork(
            WIFI_SSID_2,
            WIFI_PASSWORD_2);
#endif

#ifdef WIFI_SSID_3
        NetworkService::addNetwork(
            WIFI_SSID_3,
            WIFI_PASSWORD_3);
#endif
    }

    /*
     * Only redraw connection status when something changes.
     * This prevents normal service loops from overwriting an MQTT
     * message currently being displayed.
     */
    void updateConnectionStatus()
    {
        const bool wifiConnected =
            NetworkService::isConnected();

        const bool mqttConnected =
            MQTTService::isConnected();

        if (wifiConnected != g_wasWiFiConnected)
        {
            g_wasWiFiConnected =
                wifiConnected;

            if (!wifiConnected)
            {
                showStatus(
                    "WiFi",
                    "Connecting",
                    Color::Yellow);

                g_wasMqttConnected = false;
                return;
            }

            LOGF(
                "Demo Wi-Fi ready | SSID: %s | IP: %s",
                NetworkService::ssid().c_str(),
                NetworkService::ipAddress().c_str());

            showStatus(
                "MQTT",
                "Connecting",
                Color::Yellow,
                MQTT_BROKER);
        }

        if (mqttConnected != g_wasMqttConnected)
        {
            g_wasMqttConnected =
                mqttConnected;

            if (mqttConnected)
            {
                Display_StatusScreenData screen =
                {
                    "MQTT",
                    "Connected",
                    g_lastMqttMessage,
                    Color::Green
                };

                Display_ShowStatusScreen(screen);
            }
            else if (wifiConnected)
            {
                showStatus(
                    "MQTT",
                    "Connecting",
                    Color::Yellow,
                    MQTT_BROKER);
            }
        }
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" MQTT Service Demo");
    LOG("=====================================");

    Display::begin();

    /*
     * Candidate networks belong to NetworkService.
     * The same application code is used on every supported board.
     */
    addConfiguredNetworks();

    NetworkService::begin();

    MQTTService::begin(
        MQTT_BROKER,
        MQTT_PORT,
        MQTT_CLIENT_ID);

    MQTTService::setMessageCallback(
        handleMqttMessage);

    /*
     * MQTTService remembers this subscription even before the
     * broker is connected and automatically re-subscribes later.
     */
    MQTTService::subscribe(
        MQTT_TOPIC);

    showStatus(
        "WiFi",
        "Connecting",
        Color::Yellow);
}

void loop()
{
    NetworkService::loop();
    MQTTService::loop();

    updateConnectionStatus();

    delay(10);
}