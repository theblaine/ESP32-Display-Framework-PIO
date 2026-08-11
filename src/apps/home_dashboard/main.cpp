#include <Arduino.h>

#include "ProjectVersion.h"

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "RGB_lamp.h"

#include "secrets.h"

#include "NetworkService.h"
#include "MQTTService.h"

#include "pages/TestPage.h"
#include "pages/PiHolePage.h"
#include "pages/FlightRadarPage.h"
#include "pages/HomeAssistantPage.h"
#include "pages/SystemStatusPage.h"
#include "pages/SystemMonitorPage.h"
#include "pages/NetworkPage.h"
#include "pages/DeathStarPage.h"

namespace
{
    constexpr unsigned long PAGE_DURATION_MS =
        10000;

    constexpr const char *MQTT_BROKER =
        "10.0.0.50";

    constexpr uint16_t MQTT_PORT =
        1883;

    constexpr const char *MQTT_CLIENT_ID =
        "waveshare-home-dashboard";

    constexpr const char *FLIGHTRADAR_TOPIC =
        "home/dashboard/flightradar";

    constexpr const char *HOME_ASSISTANT_TOPIC =
        "home/dashboard/homeassistant";

    constexpr const char *PIHOLE_TOPIC =
        "home/dashboard/pihole";

    enum class DashboardPage : uint8_t
    {
        HomeAssistant,
        FlightRadar,
        PiHole,
        Test,
        SystemStatus,
        SystemMonitor,
        Network,
        DeathStar,
    };

    /*
     * Pages included in automatic rotation.
     *
     * Add, remove, comment out, or reorder entries here.
     * Test remains available for locked development mode even
     * though it is not included in normal rotation.
     */
    constexpr DashboardPage ROTATION_PAGES[] =
        {
            // DashboardPage::HomeAssistant,
            // DashboardPage::FlightRadar,
            // DashboardPage::PiHole,
            // DashboardPage::Test,
            // DashboardPage::SystemStatus,
            // DashboardPage::SystemMonitor,
            // DashboardPage::Network,
            DashboardPage::DeathStar,
    };

    constexpr size_t ROTATION_PAGE_COUNT =
        sizeof(ROTATION_PAGES) /
        sizeof(ROTATION_PAGES[0]);

    DashboardPage g_currentPage =
        ROTATION_PAGES[0];

    size_t g_rotationIndex = 0;

    unsigned long g_lastPageChangeTime = 0;

    void drawStatusFooter()
    {
        constexpr int16_t FOOTER_HEIGHT = 28;
        constexpr uint16_t INACTIVE_COLOR = 0x7BEF;

        const int16_t footerY =
            Display::height() - FOOTER_HEIGHT;

        Display_DrawPanel(
            0,
            footerY,
            Display::width(),
            FOOTER_HEIGHT,
            Color::Black,
            Color::White,
            0);

        Display_DrawStatusIndicator(
            4,
            footerY + 2,
            52,
            FOOTER_HEIGHT - 4,
            "HA",
            HomeAssistantPage::hasReceivedData()
                ? Color::Green
                : INACTIVE_COLOR,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        Display_DrawStatusIndicator(
            59,
            footerY + 2,
            52,
            FOOTER_HEIGHT - 4,
            "FR",
            FlightRadarPage::hasReceivedData()
                ? Color::Green
                : INACTIVE_COLOR,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);

        Display_DrawStatusIndicator(
            114,
            footerY + 2,
            54,
            FOOTER_HEIGHT - 4,
            "PH",
            PiHolePage::hasReceivedData()
                ? Color::Green
                : INACTIVE_COLOR,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);
    }

    void handleMqttMessage(
        const char *topic,
        const char *payload)
    {
        if (topic == nullptr ||
            payload == nullptr)
        {
            return;
        }

        if (strcmp(
                topic,
                FLIGHTRADAR_TOPIC) == 0)
        {
            FlightRadarPage::handleMessage(
                payload);

            if (g_currentPage ==
                DashboardPage::FlightRadar)
            {
                FlightRadarPage::draw();
                drawStatusFooter();
            }

            return;
        }

        if (strcmp(
                topic,
                HOME_ASSISTANT_TOPIC) == 0)
        {
            HomeAssistantPage::handleMessage(
                payload);

            if (g_currentPage ==
                DashboardPage::HomeAssistant)
            {
                HomeAssistantPage::draw();
                drawStatusFooter();
            }

            return;
        }

        if (strcmp(
                topic,
                PIHOLE_TOPIC) == 0)
        {
            PiHolePage::handleMessage(
                payload);

            if (g_currentPage ==
                DashboardPage::PiHole)
            {
                PiHolePage::draw();
                drawStatusFooter();
            }

            return;
        }
    }

    void drawCurrentPage()
    {
        switch (g_currentPage)
        {
        case DashboardPage::HomeAssistant:
            HomeAssistantPage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::FlightRadar:
            FlightRadarPage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::PiHole:
            PiHolePage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::Test:
            TestPage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::SystemStatus:
            SystemStatusPage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::SystemMonitor:
            SystemMonitorPage::draw();
            drawStatusFooter();
            break;

        case DashboardPage::Network:
            NetworkPage::draw();
            drawStatusFooter();
            break;
        }
    }

    void advancePage()
    {
        g_rotationIndex++;

        if (g_rotationIndex >=
            ROTATION_PAGE_COUNT)
        {
            g_rotationIndex = 0;
        }

        g_currentPage =
            ROTATION_PAGES[g_rotationIndex];

        drawCurrentPage();
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOGF(
        " %s v%s",
        PROJECT_NAME,
        PROJECT_VERSION);
    LOG(" Home Dashboard");
    LOG("=====================================");

    Display::begin();
    RGBLamp::begin();

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

    NetworkService::begin();

    MQTTService::begin(
        MQTT_BROKER,
        MQTT_PORT,
        MQTT_CLIENT_ID);

    MQTTService::setMessageCallback(
        handleMqttMessage);

    MQTTService::subscribe(
        FLIGHTRADAR_TOPIC);

    MQTTService::subscribe(
        HOME_ASSISTANT_TOPIC);

    MQTTService::subscribe(
        PIHOLE_TOPIC);
    g_rotationIndex = 0;

    g_currentPage =
        ROTATION_PAGES[g_rotationIndex];

    drawCurrentPage();

    g_lastPageChangeTime =
        millis();
}

void loop()
{
    NetworkService::loop();
    MQTTService::loop();

    const unsigned long currentTime =
        millis();

    if (currentTime -
            g_lastPageChangeTime >=
        PAGE_DURATION_MS)
    {
        g_lastPageChangeTime =
            currentTime;

        advancePage();
    }

    delay(10);
}