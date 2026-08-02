#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "secrets.h"

namespace
{
    constexpr unsigned long WIFI_SERVICE_INTERVAL_MS = 2000;
    constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 10000;

    constexpr const char *MQTT_BROKER = "10.0.0.50";
    constexpr uint16_t MQTT_PORT = 1883;
    constexpr const char *MQTT_CLIENT_ID = "waveshare-display-demo";
    constexpr const char *MQTT_TOPIC = "test/display";

    WiFiMulti g_wifiMulti;
    WiFiClient g_wifiClient;
    PubSubClient g_mqttClient(g_wifiClient);

    unsigned long g_lastWiFiServiceTime = 0;
    unsigned long g_lastMqttAttemptTime = 0;
    String g_lastMqttMessage = "Waiting...";

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

    void addConfiguredNetworks()
    {
        g_wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWORD_1);

#ifdef WIFI_SSID_2
        g_wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWORD_2);
#endif

#ifdef WIFI_SSID_3
        g_wifiMulti.addAP(WIFI_SSID_3, WIFI_PASSWORD_3);
#endif
    }

    bool serviceWiFi()
    {
        const unsigned long currentTime = millis();

        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }

        if (currentTime - g_lastWiFiServiceTime < WIFI_SERVICE_INTERVAL_MS)
        {
            return false;
        }

        g_lastWiFiServiceTime = currentTime;

        showStatus(
            "WiFi",
            "Connecting",
            Color::Yellow);

        const wl_status_t status =
            static_cast<wl_status_t>(
                g_wifiMulti.run(WIFI_CONNECT_TIMEOUT_MS));

        if (status == WL_CONNECTED)
        {
            const String ipAddress = WiFi.localIP().toString();

            LOGF(
                "Wi-Fi connected | SSID: %s | IP: %s",
                WiFi.SSID().c_str(),
                ipAddress.c_str());

            showStatus(
                "WiFi",
                "Connected",
                Color::Green,
                ipAddress.c_str());

            return true;
        }

        return false;
    }

    void serviceMqtt()
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            return;
        }

        if (g_mqttClient.connected())
        {
            g_mqttClient.loop();
            return;
        }

        const unsigned long currentTime = millis();

        if (currentTime - g_lastMqttAttemptTime < MQTT_RETRY_INTERVAL_MS)
        {
            return;
        }

        g_lastMqttAttemptTime = currentTime;

        LOGF(
            "Connecting to MQTT broker %s:%u",
            MQTT_BROKER,
            MQTT_PORT);

        showStatus(
            "MQTT",
            "Connecting",
            Color::Yellow,
            MQTT_BROKER);

        if (g_mqttClient.connect(MQTT_CLIENT_ID))
        {
            LOG("MQTT connected.");

            if (g_mqttClient.subscribe(MQTT_TOPIC))
            {
                LOGF("Subscribed to %s", MQTT_TOPIC);
            }
            else
            {
                LOGW("Subscription failed.");
            }

            Display_StatusScreenData screen =
                {
                    "MQTT",
                    "Connected",
                    Color::Green,
                    g_lastMqttMessage.c_str()};

            Display_ShowStatusScreen(screen);
        }
        else
        {
            LOGWF(
                "MQTT connection failed. State: %d",
                g_mqttClient.state());

            showStatus(
                "MQTT",
                "Failed",
                Color::Red,
                MQTT_BROKER);
        }
    }

    void mqttCallback(char *topic, byte *payload, unsigned int length)
    {
        String receivedMessage;

        for (unsigned int i = 0; i < length; i++)
        {
            receivedMessage += static_cast<char>(payload[i]);
        }

        g_lastMqttMessage = receivedMessage;

        LOGF(
            "MQTT [%s] %s",
            topic,
            receivedMessage.c_str());

        showStatus(
            "MQTT",
            "Connected",
            Color::Green,
            g_lastMqttMessage.c_str());
    }

}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 MQTT Demo");
    LOG("=====================================");

    Display::begin();

    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFi.setAutoReconnect(true);

    addConfiguredNetworks();

    g_mqttClient.setServer(
        MQTT_BROKER,
        MQTT_PORT);

    g_mqttClient.setCallback(mqttCallback);

    showStatus(
        "WiFi",
        "Connecting",
        Color::Yellow);
}

void loop()
{
    serviceWiFi();
    serviceMqtt();

    delay(10);
}