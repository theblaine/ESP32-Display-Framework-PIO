#pragma once

namespace Logger
{
    void begin();

    void info(const char* message);
    void infof(const char* format, ...);

    void warning(const char* message);
    void warningf(const char* format, ...);

    void error(const char* message);
    void errorf(const char* format, ...);
}

#define LOG(message)            Logger::info(message)
#define LOGF(format, ...)       Logger::infof(format, ##__VA_ARGS__)

#define LOGW(message)           Logger::warning(message)
#define LOGWF(format, ...)      Logger::warningf(format, ##__VA_ARGS__)

#define LOGE(message)           Logger::error(message)
#define LOGEF(format, ...)      Logger::errorf(format, ##__VA_ARGS__)