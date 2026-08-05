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

    constexpr const char *MQTT_BROKER = "10.0.0.50";
    constexpr uint16_t MQTT_PORT = 1883;
    constexpr const char *MQTT_CLIENT_ID = "waveshare-home-dashboard";

    constexpr const char *FLIGHTRADAR_TOPIC =
        "home/dashboard/flightradar";

    constexpr const char *HOME_ASSISTANT_TOPIC =
        "home/dashboard/homeassistant";

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

    enum class DashboardPage : uint8_t
    {
        HomeAssistant,
        FlightRadar,
        PiHole,
        Count
    };

    HomeAssistantData g_homeAssistantData;
    FlightRadarData g_flightRadarData;
    PiHoleData g_piHoleData;

    DashboardPage g_currentPage =
        DashboardPage::HomeAssistant;

    unsigned long g_lastPageChangeTime = 0;

    void drawHomeAssistantPage()
    {
        const Display_TableRow rows[] =
            {
                {"Plug 13",
                 g_homeAssistantData.sparePlug13.c_str(),
                 g_homeAssistantData.sparePlug13 == "ON"
                     ? Color::Green
                     : Color::Red},
                {"Alarm",
                 g_homeAssistantData.alarm.c_str(),
                 Color::Green},
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
                 Color::Green},
                {"Aircraft",
                 g_flightRadarData.aircraft.c_str(),
                 Color::Cyan},
                {"MLAT",
                 g_flightRadarData.mlat.c_str(),
                 Color::Green},
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
            g_flightRadarData.feed = feed;
        }

        if (aircraft != nullptr)
        {
            g_flightRadarData.aircraft = aircraft;
        }

        if (mlat != nullptr)
        {
            g_flightRadarData.mlat = mlat;
        }

        if (updated != nullptr)
        {
            g_flightRadarData.updated = updated;
        }

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
        }
    }

    void drawPiHolePage()
    {
        const Display_TableRow rows[] =
            {
                {"Status",
                 g_piHoleData.status.c_str(),
                 Color::Green},
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

        LOG("Displayed Pi-hole page.");
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

        case DashboardPage::Count:
            break;
        }
    }

    void advancePage()
    {
        uint8_t nextPage =
            static_cast<uint8_t>(g_currentPage) + 1;

        if (nextPage >=
            static_cast<uint8_t>(DashboardPage::Count))
        {
            nextPage = 0;
        }

        g_currentPage =
            static_cast<DashboardPage>(nextPage);

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

    drawCurrentPage();

    g_lastPageChangeTime = millis();
}

void loop()
{
    NetworkService::loop();
    MQTTService::loop();

    const unsigned long currentTime = millis();

    if (currentTime - g_lastPageChangeTime >=
        PAGE_DURATION_MS)
    {
        g_lastPageChangeTime = currentTime;

        advancePage();
    }

    delay(10);
}