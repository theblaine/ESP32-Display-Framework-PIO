#include "MQTTService.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include "Logger.h"
#include "NetworkService.h"

namespace
{
    constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;

    WiFiClient g_wifiClient;
    PubSubClient g_mqttClient(g_wifiClient);

    String g_broker;
    String g_clientId;

    uint16_t g_port = 1883;
    unsigned long g_lastConnectionAttemptTime = 0;

    void connect()
    {
        if (!NetworkService::isConnected())
        {
            return;
        }

        if (g_mqttClient.connected())
        {
            return;
        }

        const unsigned long currentTime = millis();

        if (currentTime - g_lastConnectionAttemptTime <
            MQTT_RETRY_INTERVAL_MS)
        {
            return;
        }

        g_lastConnectionAttemptTime = currentTime;

        LOGF(
            "Connecting to MQTT broker %s:%u",
            g_broker.c_str(),
            g_port);

        if (g_mqttClient.connect(g_clientId.c_str()))
        {
            LOG("MQTT connection established.");
        }
        else
        {
            LOGWF(
                "MQTT connection failed. State: %d",
                g_mqttClient.state());
        }
    }
}

namespace MQTTService
{
    void begin(
        const char* broker,
        uint16_t port,
        const char* clientId)
    {
        if (broker == nullptr || clientId == nullptr)
        {
            LOGE("MQTT configuration is invalid.");
            return;
        }

        g_broker = broker;
        g_port = port;
        g_clientId = clientId;

        g_mqttClient.setServer(
            g_broker.c_str(),
            g_port);

        g_lastConnectionAttemptTime = 0;

        LOGF(
            "MQTT configured | Broker: %s:%u | Client: %s",
            g_broker.c_str(),
            g_port,
            g_clientId.c_str());
    }

    void loop()
    {
        if (!NetworkService::isConnected())
        {
            return;
        }

        if (!g_mqttClient.connected())
        {
            connect();
            return;
        }

        g_mqttClient.loop();
    }

    bool isConnected()
    {
        return g_mqttClient.connected();
    }
}