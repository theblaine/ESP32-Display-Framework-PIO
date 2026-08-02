#include "MQTTService.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include "Logger.h"
#include "NetworkService.h"

namespace
{
    constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;
    constexpr size_t MAX_SUBSCRIPTIONS = 8;

    WiFiClient g_wifiClient;
    PubSubClient g_mqttClient(g_wifiClient);

    String g_broker;
    String g_clientId;

    uint16_t g_port = 1883;
    unsigned long g_lastConnectionAttemptTime = 0;

    MQTTService::MessageCallback g_messageCallback = nullptr;

    String g_subscriptions[MAX_SUBSCRIPTIONS];
    size_t g_subscriptionCount = 0;

    void handleIncomingMessage(
        char *topic,
        byte *payload,
        unsigned int length)
    {
        String message;
        message.reserve(length);

        for (unsigned int index = 0;
             index < length;
             index++)
        {
            message +=
                static_cast<char>(payload[index]);
        }

        LOGF(
            "MQTT [%s] %s",
            topic,
            message.c_str());

        if (g_messageCallback != nullptr)
        {
            g_messageCallback(
                topic,
                message.c_str());
        }
    }

    void subscribeToConfiguredTopics()
    {
        for (size_t index = 0;
             index < g_subscriptionCount;
             index++)
        {
            const String &topic =
                g_subscriptions[index];

            if (g_mqttClient.subscribe(topic.c_str()))
            {
                LOGF(
                    "Subscribed to MQTT topic: %s",
                    topic.c_str());
            }
            else
            {
                LOGWF(
                    "Failed to subscribe to MQTT topic: %s",
                    topic.c_str());
            }
        }
    }

    void connectToBroker()
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

            subscribeToConfiguredTopics();
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
        const char *broker,
        uint16_t port,
        const char *clientId)
    {
        if (broker == nullptr ||
            clientId == nullptr)
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

        g_mqttClient.setCallback(
            handleIncomingMessage);

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
            connectToBroker();
            return;
        }

        g_mqttClient.loop();
    }

    bool isConnected()
    {
        return g_mqttClient.connected();
    }

    void setMessageCallback(
        MessageCallback callback)
    {
        g_messageCallback = callback;
    }

    bool subscribe(
        const char *topic)
    {
        if (topic == nullptr ||
            topic[0] == '\0')
        {
            return false;
        }

        for (size_t index = 0;
             index < g_subscriptionCount;
             index++)
        {
            if (g_subscriptions[index] == topic)
            {
                return true;
            }
        }

        if (g_subscriptionCount >=
            MAX_SUBSCRIPTIONS)
        {
            LOGE("MQTT subscription limit reached.");
            return false;
        }

        g_subscriptions[g_subscriptionCount] =
            topic;

        g_subscriptionCount++;

        LOGF(
            "Registered MQTT topic: %s",
            topic);

        if (g_mqttClient.connected())
        {
            return g_mqttClient.subscribe(topic);
        }

        return true;
    }
}