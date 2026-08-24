#include <Arduino.h>
#include <cstring>

#include "Display.h"
#include "Logger.h"
#include "MQTTService.h"
#include "NetworkService.h"
#include "TimeService.h"
#include "secrets.h"

#include "pages/ClockPage.h"
#include "pages/DevicePage.h"
#include "pages/MqttPage.h"

namespace
{
    constexpr const char *MQTT_BROKER =
        "10.0.0.50";

    constexpr uint16_t MQTT_PORT =
        1883;

    constexpr const char *MQTT_CLIENT_ID =
        "portable-dashboard";

    constexpr const char *MQTT_TOPIC =
        "test/display";

    constexpr const char *TIMEZONE =
        "PST8PDT,M3.2.0,M11.1.0";

    constexpr unsigned long PAGE_DURATION_MS =
        10000;

    enum class PortablePage : uint8_t
    {
        Device,
        Clock,
        Mqtt
    };

    PortablePage g_currentPage =
        PortablePage::Device;

    unsigned long g_lastPageChangeTime = 0;
    bool g_pageDirty = true;

    void drawCurrentPage()
    {
        switch (g_currentPage)
        {
        case PortablePage::Device:
            DevicePage::draw();
            break;

        case PortablePage::Clock:
            ClockPage::draw();
            break;

        case PortablePage::Mqtt:
            MqttPage::draw();
            break;
        }

        g_pageDirty = false;
    }

    bool currentPageNeedsRedraw()
    {
        switch (g_currentPage)
        {
        case PortablePage::Device:
            return DevicePage::needsRedraw();

        case PortablePage::Clock:
            return false;

        case PortablePage::Mqtt:
            return false;
        }

        return false;
    }

    void advancePage()
    {
        switch (g_currentPage)
        {
        case PortablePage::Device:
            g_currentPage = PortablePage::Clock;
            break;

        case PortablePage::Clock:
            g_currentPage = PortablePage::Mqtt;
            break;

        case PortablePage::Mqtt:
            g_currentPage = PortablePage::Device;
            break;
        }

        g_lastPageChangeTime = millis();
        g_pageDirty = true;
    }

    void handleMqttMessage(
        const char *topic,
        const char *payload)
    {
        if (topic == nullptr ||
            strcmp(topic, MQTT_TOPIC) != 0)
        {
            return;
        }

        MqttPage::update(payload);

        if (g_currentPage == PortablePage::Mqtt)
        {
            g_pageDirty = true;
        }
    }

    void configureNetworks()
    {
        NetworkService::addNetwork(
            WIFI_SSID_1,
            WIFI_PASSWORD_1);

#ifdef WIFI_SSID_2
        NetworkService::addNetwork(
            WIFI_SSID_2,
            WIFI_PASSWORD_2);
#endif

#ifdef WIFI_SSID_3
        NetworkService::addNetwork(
            WIFI_SSID_3,
            WIFI_PASSWORD_3);
#endif
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" Portable Dashboard");
    LOG("=====================================");

    Display::begin();

    configureNetworks();
    NetworkService::begin();

    TimeService::begin(
        TIMEZONE);

    MQTTService::begin(
        MQTT_BROKER,
        MQTT_PORT,
        MQTT_CLIENT_ID);

    MQTTService::setMessageCallback(
        handleMqttMessage);

    MQTTService::subscribe(
        MQTT_TOPIC);

    g_lastPageChangeTime = millis();
    drawCurrentPage();
}

void loop()
{
    NetworkService::loop();
    MQTTService::loop();

    const unsigned long currentTime =
        millis();

    if (currentTime - g_lastPageChangeTime >=
        PAGE_DURATION_MS)
    {
        advancePage();
    }

    if (!g_pageDirty &&
        currentPageNeedsRedraw())
    {
        g_pageDirty = true;
    }

    if (g_pageDirty)
    {
        drawCurrentPage();
    }

    delay(1);
}
