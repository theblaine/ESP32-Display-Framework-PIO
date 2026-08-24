#pragma once

#include <Arduino.h>

/*
 * MQTTService
 *
 * Provides MQTT client management for framework applications.
 *
 * Topics may be registered before a connection is established.
 * Registered topics are automatically re-subscribed whenever the
 * broker connection is restored.
 */

namespace MQTTService
{
    /*
     * Receives an incoming MQTT message.
     *
     * Payload is presented as a null-terminated string.
     */
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

    bool publish(
        const char *topic,
        const char *payload);
}