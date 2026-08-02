#pragma once

#include <Arduino.h>

namespace NetworkService
{
    void begin();

    void addNetwork(
        const char *ssid,
        const char *password);

    void loop();

    bool isConnected();

    String ssid();
    String ipAddress();
    int32_t rssi();
}