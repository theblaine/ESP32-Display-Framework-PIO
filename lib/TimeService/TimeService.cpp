#include "TimeService.h"

namespace
{
    // Reject the Unix epoch and other startup values before SNTP sets the clock.
    constexpr time_t MIN_VALID_TIME =
        1704067200; // 2024-01-01 UTC

    bool getLocalTimeValue(
        struct tm &timeInfo)
    {
        if (!TimeService::isSynced())
        {
            return false;
        }

        return getLocalTime(
            &timeInfo,
            0);
    }
}

namespace TimeService
{
    void begin(
        const char *timezone,
        const char *ntpServer1,
        const char *ntpServer2)
    {
        if (timezone == nullptr ||
            ntpServer1 == nullptr)
        {
            return;
        }

        configTzTime(
            timezone,
            ntpServer1,
            ntpServer2);
    }

    bool isSynced()
    {
        return time(nullptr) >=
               MIN_VALID_TIME;
    }

    String timeString()
    {
        struct tm timeInfo;

        if (!getLocalTimeValue(
                timeInfo))
        {
            return "--:-- --";
        }

        char buffer[16];

        strftime(
            buffer,
            sizeof(buffer),
            "%I:%M %p",
            &timeInfo);

        return String(buffer);
    }

    String dateString()
    {
        struct tm timeInfo;

        if (!getLocalTimeValue(
                timeInfo))
        {
            return "--/--/----";
        }

        char buffer[16];

        strftime(
            buffer,
            sizeof(buffer),
            "%m/%d/%Y",
            &timeInfo);

        return String(buffer);
    }

    String dateTimeString()
    {
        struct tm timeInfo;

        if (!getLocalTimeValue(
                timeInfo))
        {
            return "--/--/---- --:-- --";
        }

        char buffer[32];

        strftime(
            buffer,
            sizeof(buffer),
            "%m/%d/%Y %I:%M %p",
            &timeInfo);

        return String(buffer);
    }

    time_t now()
    {
        return time(nullptr);
    }
}
