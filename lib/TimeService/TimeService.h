#pragma once

#include <Arduino.h>
#include <time.h>

namespace TimeService
{
    // Starts ESP32 background SNTP and applies the supplied POSIX timezone.
    void begin(
        const char *timezone,
        const char *ntpServer1 = "pool.ntp.org",
        const char *ntpServer2 = "time.nist.gov");

    bool isSynced();

    String timeString();
    String dateString();
    String dateTimeString();

    time_t now();
}
