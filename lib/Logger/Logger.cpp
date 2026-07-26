#include "Logger.h"

#include <Arduino.h>
#include <cstdarg>
#include <cstdio>

namespace
{
void printFormatted(
    const char* level,
    const char* format,
    va_list arguments
)
{
    if (level != nullptr)
    {
        printf("[%s] ", level);
    }

    vprintf(format, arguments);
    printf("\r\n");
}
}

namespace Logger
{
void begin()
{
    // printf() is currently the verified output method for this board.
    //
    // This function is intentionally present even though no initialization
    // is required yet. Future logger outputs such as LCD, MQTT, SD card, or
    // network logging may need initialization here.
}

void info(const char* message)
{
    printf("[INFO] %s\r\n", message);
}

void infof(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    printFormatted("INFO", format, arguments);

    va_end(arguments);
}

void warning(const char* message)
{
    printf("[WARN] %s\r\n", message);
}

void warningf(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    printFormatted("WARN", format, arguments);

    va_end(arguments);
}

void error(const char* message)
{
    printf("[ERROR] %s\r\n", message);
}

void errorf(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    printFormatted("ERROR", format, arguments);

    va_end(arguments);
}
}