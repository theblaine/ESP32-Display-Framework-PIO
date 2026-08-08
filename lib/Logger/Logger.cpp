#include "Logger.h"

#include <Arduino.h>
#include <cstdarg>
#include <cstdio>

namespace
{
    void printMessage(
        const char *level,
        const char *message)
    {
        if (message == nullptr)
        {
            message = "";
        }

        printf(
            "[%s] %s\r\n",
            level,
            message);
    }

    void printFormatted(
        const char *level,
        const char *format,
        va_list arguments)
    {
        if (format == nullptr)
        {
            return;
        }

        printf(
            "[%s] ",
            level);

        vprintf(
            format,
            arguments);

        printf("\r\n");
    }
}

namespace Logger
{
    void begin()
    {
        // printf() is currently the verified output method.
        //
        // This function intentionally remains even though no setup is
        // required. Future outputs such as MQTT, SD card, display, or
        // network logging may require initialization here.
    }

    void info(
        const char *message)
    {
        printMessage(
            "INFO",
            message);
    }

    void infof(
        const char *format,
        ...)
    {
        va_list arguments;
        va_start(
            arguments,
            format);

        printFormatted(
            "INFO",
            format,
            arguments);

        va_end(arguments);
    }

    void warning(
        const char *message)
    {
        printMessage(
            "WARN",
            message);
    }

    void warningf(
        const char *format,
        ...)
    {
        va_list arguments;
        va_start(
            arguments,
            format);

        printFormatted(
            "WARN",
            format,
            arguments);

        va_end(arguments);
    }

    void error(
        const char *message)
    {
        printMessage(
            "ERROR",
            message);
    }

    void errorf(
        const char *format,
        ...)
    {
        va_list arguments;
        va_start(
            arguments,
            format);

        printFormatted(
            "ERROR",
            format,
            arguments);

        va_end(arguments);
    }
}