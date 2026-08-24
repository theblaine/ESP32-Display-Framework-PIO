#pragma once

#include <Arduino.h>

/*
 * NetworkService
 *
 * Provides Wi-Fi management for framework applications.
 *
 * Multiple candidate networks may be configured before begin().
 *
 * Call loop() regularly to maintain the connection.
 */


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