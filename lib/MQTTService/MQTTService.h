#pragma once

#include <Arduino.h>

namespace MQTTService
{
    void begin(
        const char* broker,
        uint16_t port,
        const char* clientId);

    void loop();

    bool isConnected();
}