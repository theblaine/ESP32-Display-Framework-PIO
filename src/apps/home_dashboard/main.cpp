#include <Arduino.h>
#include <ArduinoJson.h>

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "RGB_lamp.h"

#include "secrets.h"

#include "NetworkService.h"
#include "MQTTService.h"

namespace
{
    constexpr unsigned long PAGE_DURATION_MS = 10000;

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

    struct HomeAssistantData
    {
        String sparePlug13 = "Unknown";
        String alarm = "Disarmed";
        String indoorTemperature = "72 F";
        String humidity = "41%";
    };

    struct FlightRadarData
    {
        String feed = "Online";
        String aircraft = "18";
        String mlat = "Yes";
        String updated = "5 sec";
    };

    struct PiHoleData
    {
        String status = "Online";
        String queries = "12,438";
        String blocked = "8.4%";
        String clients = "27";
    };

    struct TestData
    {
        String text1 = "Test One";
        String text2 = "Test Two";
        String text3 = "Test Three";
        String text4 = "Test Four";
        String text5 = "Test Five";
        String text6 = "Test Six";
    };

    enum class DashboardPage : uint8_t
    {
        HomeAssistant,
        FlightRadar,
        PiHole,
        Test,
        Count
    };

    // DashboardPage::Count = normal rotation
    // DashboardPage::HomeAssistant = lock to Home Assistant
    // DashboardPage::FlightRadar = lock to FlightRadar
    // DashboardPage::Test = lock to Test Page
    // DashboardPage::PiHole = lock to Pi-hole

    constexpr DashboardPage DEV_PAGE =
        DashboardPage::Test; // Change this to lock to a specific page for development

    HomeAssistantData g_homeAssistantData;
    FlightRadarData g_flightRadarData;
    PiHoleData g_piHoleData;
    TestData g_testData;
    DashboardPage g_currentPage =
        DashboardPage::HomeAssistant;

    unsigned long g_lastPageChangeTime = 0;

    unsigned long g_lastHomeAssistantUpdate = 0;
    unsigned long g_lastFlightRadarUpdate = 0;
    unsigned long g_lastPiHoleUpdate = 0;

    uint16_t plugColor(
        const String &state)
    {
        if (state == "ON")
        {
            return Color::Green;
        }

        if (state == "OFF")
        {
            return Color::Red;
        }

        return Color::White;
    }

    uint16_t alarmColor(
        const String &state)
    {
        if (state == "Disarmed")
        {
            return Color::Red;
        }

        if (state == "Armed Home")
        {
            return Color::Green;
        }

        if (state == "Armed Away")
        {
            return Color::Green;
        }

        if (state == "Triggered")
        {
            return Color::Yellow;
        }

        return Color::White;
    }

    uint16_t flightRadarFeedColor(
        const String &state)
    {
        if (state == "Online")
        {
            return Color::Green;
        }

        if (state == "Offline")
        {
            return Color::Red;
        }

        return Color::White;
    }

    uint16_t mlatColor(
        const String &state)
    {
        if (state == "Yes")
        {
            return Color::Green;
        }

        if (state == "No")
        {
            return Color::Red;
        }

        return Color::White;
    }

    uint16_t piHoleStatusColor(
        const String &state)
    {
        if (state == "Online")
        {
            return Color::Green;
        }

        if (state == "Offline")
        {
            return Color::Red;
        }

        return Color::White;
    }

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
            g_lastHomeAssistantUpdate != 0
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
            g_lastFlightRadarUpdate != 0
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
            g_lastPiHoleUpdate != 0
                ? Color::Green
                : INACTIVE_COLOR,
            Color::White,
            Color::Black,
            Display_StatusMarkerShape::Circle,
            1);
    }

    void drawHomeAssistantPage()
    {
        const Display_TableRow rows[] =
            {
                {"Plug 13",
                 g_homeAssistantData.sparePlug13.c_str(),
                 plugColor(
                     g_homeAssistantData.sparePlug13)},
                {"Alarm",
                 g_homeAssistantData.alarm.c_str(),
                 alarmColor(
                     g_homeAssistantData.alarm)},
                {"Indoor",
                 g_homeAssistantData.indoorTemperature.c_str(),
                 Color::Cyan},
                {"Humidity",
                 g_homeAssistantData.humidity.c_str(),
                 Color::Cyan}};

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Home Assistant",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(
            0,
            64,
            0);
        drawStatusFooter();
        LOG("Displayed Home Assistant page.");
    }

    void handleHomeAssistantMessage(
        const char *payload)
    {
        if (payload == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                payload);

        if (error)
        {
            LOGWF(
                "Home Assistant JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *sparePlug13 =
            document["sparePlug13"];

        if (sparePlug13 != nullptr)
        {
            g_homeAssistantData.sparePlug13 =
                sparePlug13;
        }
        g_lastHomeAssistantUpdate = millis();
        LOGF(
            "Home Assistant updated | Spare Plug 13: %s",
            g_homeAssistantData.sparePlug13.c_str());

        if (g_currentPage ==
            DashboardPage::HomeAssistant)
        {
            drawHomeAssistantPage();
        }
    }

    void drawFlightRadarPage()
    {
        const Display_TableRow rows[] =
            {
                {"Feed",
                 g_flightRadarData.feed.c_str(),
                 flightRadarFeedColor(
                     g_flightRadarData.feed)},
                {"Aircraft",
                 g_flightRadarData.aircraft.c_str(),
                 Color::Cyan},
                {"MLAT",
                 g_flightRadarData.mlat.c_str(),
                 mlatColor(
                     g_flightRadarData.mlat)},
                {"Updated",
                 g_flightRadarData.updated.c_str(),
                 Color::White}};

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "FlightRadar24",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(
            0,
            32,
            64);
        drawStatusFooter();
        LOG("Displayed FlightRadar24 page.");
    }

    void handleFlightRadarMessage(
        const char *payload)
    {
        if (payload == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                payload);

        if (error)
        {
            LOGWF(
                "FlightRadar JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *feed =
            document["feed"];

        const char *aircraft =
            document["aircraft"];

        const char *mlat =
            document["mlat"];

        const char *updated =
            document["updated"];

        if (feed != nullptr)
        {
            g_flightRadarData.feed =
                feed;
        }

        if (aircraft != nullptr)
        {
            g_flightRadarData.aircraft =
                aircraft;
        }

        if (mlat != nullptr)
        {
            g_flightRadarData.mlat =
                mlat;
        }

        if (updated != nullptr)
        {
            g_flightRadarData.updated =
                updated;
        }

        g_lastFlightRadarUpdate = millis();

        LOGF(
            "FlightRadar updated | Feed: %s | Aircraft: %s | MLAT: %s | Updated: %s",
            g_flightRadarData.feed.c_str(),
            g_flightRadarData.aircraft.c_str(),
            g_flightRadarData.mlat.c_str(),
            g_flightRadarData.updated.c_str());

        if (g_currentPage ==
            DashboardPage::FlightRadar)
        {
            drawFlightRadarPage();
        }
    }

    void drawPiHolePage()
    {
        const Display_TableRow rows[] =
            {
                {"Status",
                 g_piHoleData.status.c_str(),
                 piHoleStatusColor(
                     g_piHoleData.status)},
                {"Queries",
                 g_piHoleData.queries.c_str(),
                 Color::Cyan},
                {"Blocked",
                 g_piHoleData.blocked.c_str(),
                 Color::Yellow},
                {"Clients",
                 g_piHoleData.clients.c_str(),
                 Color::White}};

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Pi-hole",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(
            64,
            0,
            64);
        drawStatusFooter();
        LOG("Displayed Pi-hole page.");
    }

    void handlePiHoleMessage(
        const char *payload)
    {

        if (payload == nullptr)
        {
            return;
        }

        JsonDocument document;

        const DeserializationError error =
            deserializeJson(
                document,
                payload);

        if (error)
        {
            LOGWF(
                "Pi-hole JSON parse failed: %s",
                error.c_str());

            return;
        }

        const char *status =
            document["status"];

        const char *queries =
            document["queries"];

        const char *blocked =
            document["blocked"];

        const char *clients =
            document["clients"];

        if (status != nullptr)
        {
            g_piHoleData.status =
                status;
        }

        if (queries != nullptr)
        {
            g_piHoleData.queries =
                queries;
        }

        if (blocked != nullptr)
        {
            g_piHoleData.blocked =
                blocked;
        }

        if (clients != nullptr)
        {
            g_piHoleData.clients =
                clients;
        }

        g_lastPiHoleUpdate = millis();

        LOGF(
            "Pi-hole updated | Status: %s | Queries: %s | Blocked: %s | Clients: %s",
            g_piHoleData.status.c_str(),
            g_piHoleData.queries.c_str(),
            g_piHoleData.blocked.c_str(),
            g_piHoleData.clients.c_str());

        if (g_currentPage ==
            DashboardPage::PiHole)
        {
            drawPiHolePage();
        }
    }

    void drawTestPage()
    {
        const Display_TableRow rows[] =
            {
                {"Test 1",
                 g_testData.text1.c_str(),
                 Color::Red},
                {"Test 2",
                 g_testData.text2.c_str(),
                 Color::Cyan},
                {"Test 3",
                 g_testData.text3.c_str(),
                 Color::Yellow},
                {"Test 4",
                 g_testData.text4.c_str(),
                 Color::White},
                {"Test 5",
                 g_testData.text5.c_str(),
                 Color::Green},
                {"Test 6",
                 g_testData.text6.c_str(),
                 Color::Green}
            };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
           "Test Page",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(
            64,
            0,
            64);
        drawStatusFooter();
        LOG("Displayed Test page.");
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
            handleFlightRadarMessage(payload);
            return;
        }

        if (strcmp(
                topic,
                HOME_ASSISTANT_TOPIC) == 0)
        {
            handleHomeAssistantMessage(payload);
            return;
        }

        if (strcmp(
                topic,
                PIHOLE_TOPIC) == 0)
        {
            handlePiHoleMessage(payload);
        }
    }

    void drawCurrentPage()
    {
        switch (g_currentPage)
        {
        case DashboardPage::HomeAssistant:
            drawHomeAssistantPage();
            break;

        case DashboardPage::FlightRadar:
            drawFlightRadarPage();
            break;

        case DashboardPage::PiHole:
            drawPiHolePage();
            break;

        case DashboardPage::Test:
            drawTestPage();
            break;

        case DashboardPage::Count:
            break;
        }
    }

    void advancePage()
    {
        uint8_t nextPage =
            static_cast<uint8_t>(g_currentPage) + 1;

        if (nextPage >=
            static_cast<uint8_t>(
                DashboardPage::Count))
        {
            nextPage = 0;
        }

        g_currentPage =
            static_cast<DashboardPage>(
                nextPage);

        drawCurrentPage();
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
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

    if (DEV_PAGE != DashboardPage::Count)
    {
        g_currentPage =
            DEV_PAGE;
    }

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

    if (DEV_PAGE == DashboardPage::Count)
    {
        if (currentTime -
                g_lastPageChangeTime >=
            PAGE_DURATION_MS)
        {
            g_lastPageChangeTime =
                currentTime;

            advancePage();
        }
    }

    delay(10);
}