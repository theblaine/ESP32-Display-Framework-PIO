#include "NetworkService.h"

#include <WiFi.h>
#include <WiFiMulti.h>

#include "Logger.h"

namespace
{
    constexpr unsigned long WIFI_SERVICE_INTERVAL_MS = 2000;
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 10000;

    WiFiMulti g_wifiMulti;

    unsigned long g_lastWiFiServiceTime = 0;
    bool g_wasConnected = false;

    void logConnectionDetails()
    {
        LOG("Wi-Fi connection established.");

        LOGF(
            "SSID       : %s",
            WiFi.SSID().c_str());

        LOGF(
            "IP Address : %s",
            WiFi.localIP().toString().c_str());

        LOGF(
            "Gateway    : %s",
            WiFi.gatewayIP().toString().c_str());

        LOGF(
            "Subnet     : %s",
            WiFi.subnetMask().toString().c_str());

        LOGF(
            "DNS        : %s",
            WiFi.dnsIP().toString().c_str());

        LOGF(
            "RSSI       : %d dBm",
            WiFi.RSSI());
    }
}

namespace NetworkService
{
    void addNetwork(
        const char* ssid,
        const char* password)
    {
        if (ssid == nullptr || password == nullptr)
        {
            return;
        }

        g_wifiMulti.addAP(
            ssid,
            password);

        LOGF(
            "Configured Wi-Fi network: %s",
            ssid);
    }

    void begin()
    {
        WiFi.mode(WIFI_STA);
        WiFi.persistent(false);
        WiFi.setAutoReconnect(true);

        LOG("Starting Wi-Fi connection management.");

        g_lastWiFiServiceTime = 0;
        g_wasConnected = false;
    }

    void loop()
    {
        const unsigned long currentTime = millis();

        if (currentTime - g_lastWiFiServiceTime <
            WIFI_SERVICE_INTERVAL_MS)
        {
            return;
        }

        g_lastWiFiServiceTime = currentTime;

        const wl_status_t status =
            static_cast<wl_status_t>(
                g_wifiMulti.run(
                    WIFI_CONNECT_TIMEOUT_MS));

        const bool connected =
            status == WL_CONNECTED;

        if (connected && !g_wasConnected)
        {
            logConnectionDetails();
        }
        else if (!connected && g_wasConnected)
        {
            LOGW(
                "Wi-Fi connection lost. Reconnecting.");
        }

        g_wasConnected = connected;
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    String ssid()
    {
        if (!isConnected())
        {
            return "";
        }

        return WiFi.SSID();
    }

    String ipAddress()
    {
        if (!isConnected())
        {
            return "";
        }

        return WiFi.localIP().toString();
    }

    int32_t rssi()
    {
        if (!isConnected())
        {
            return 0;
        }

        return WiFi.RSSI();
    }
}