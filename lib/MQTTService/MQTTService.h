#pragma once

#include <Arduino.h>

namespace MQTTService
{
    using MessageCallback =
        void (*)(
            const char *topic,
            const char *payload);

    void begin(
        const char *broker,
        uint16_t port,
        const char *clientId);

    void loop();

    bool isConnected();

    void setMessageCallback(
        MessageCallback callback);

    bool subscribe(
        const char *topic);
}