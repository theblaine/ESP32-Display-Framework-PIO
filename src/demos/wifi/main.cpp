#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include "Logger.h"
#include "secrets.h"

namespace
{
constexpr unsigned long WIFI_SERVICE_INTERVAL_MS = 2000;
constexpr unsigned long STATUS_INTERVAL_MS       = 10000;
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS       = 10000;

WiFiMulti wifiMulti;

unsigned long lastWiFiServiceTime = 0;
unsigned long lastStatusTime      = 0;

wl_status_t previousStatus = WL_DISCONNECTED;
bool statusInitialized     = false;

const char* wifiStatusToString(wl_status_t status)
{
    switch (status)
    {
        case WL_IDLE_STATUS:
            return "Idle";

        case WL_NO_SSID_AVAIL:
            return "SSID unavailable";

        case WL_SCAN_COMPLETED:
            return "Scan completed";

        case WL_CONNECTED:
            return "Connected";

        case WL_CONNECT_FAILED:
            return "Connection failed";

        case WL_CONNECTION_LOST:
            return "Connection lost";

        case WL_DISCONNECTED:
            return "Disconnected";

        case WL_NO_SHIELD:
            return "Wi-Fi hardware unavailable";

        default:
            return "Unknown";
    }
}

void printConnectionDetails()
{
    LOG("Wi-Fi connection established.");
    LOGF("SSID       : %s", WiFi.SSID().c_str());
    LOGF("IP Address : %s", WiFi.localIP().toString().c_str());
    LOGF("Gateway    : %s", WiFi.gatewayIP().toString().c_str());
    LOGF("Subnet     : %s", WiFi.subnetMask().toString().c_str());
    LOGF("DNS        : %s", WiFi.dnsIP().toString().c_str());
    LOGF("RSSI       : %d dBm", WiFi.RSSI());
}

void handleWiFiStatus(wl_status_t currentStatus)
{
    /*
     * Record the first real status without inventing a previous state such
     * as WL_NO_SHIELD. This avoids the misleading message:
     *
     * "Wi-Fi hardware unavailable -> Disconnected"
     */
    if (!statusInitialized)
    {
        previousStatus   = currentStatus;
        statusInitialized = true;

        if (currentStatus == WL_CONNECTED)
        {
            printConnectionDetails();
        }
        else
        {
            LOGF(
                "Initial Wi-Fi status: %s (%d)",
                wifiStatusToString(currentStatus),
                static_cast<int>(currentStatus)
            );
        }

        return;
    }

    if (currentStatus == previousStatus)
    {
        return;
    }

    LOGF(
        "Wi-Fi status changed: %s -> %s",
        wifiStatusToString(previousStatus),
        wifiStatusToString(currentStatus)
    );

    if (currentStatus == WL_CONNECTED)
    {
        printConnectionDetails();
    }
    else if (previousStatus == WL_CONNECTED)
    {
        LOGW("Wi-Fi connection lost. WiFiMulti will attempt to reconnect.");
    }

    previousStatus = currentStatus;
}

void serviceWiFi(bool forceRun = false)
{
    const unsigned long currentTime = millis();

    if (!forceRun &&
        currentTime - lastWiFiServiceTime < WIFI_SERVICE_INTERVAL_MS)
    {
        return;
    }

    lastWiFiServiceTime = currentTime;

    /*
     * WiFiMulti::run() returns uint8_t in the installed ESP32 Arduino core,
     * so explicitly convert it to wl_status_t.
     *
     * WiFiMulti must be called regularly so it can:
     *   - scan for configured networks
     *   - select an available network
     *   - connect
     *   - reconnect after a connection is lost
     */
    const wl_status_t currentStatus =
        static_cast<wl_status_t>(
            wifiMulti.run(WIFI_CONNECT_TIMEOUT_MS)
        );

    handleWiFiStatus(currentStatus);
}

void printPeriodicStatus()
{
    const unsigned long currentTime = millis();

    if (currentTime - lastStatusTime < STATUS_INTERVAL_MS)
    {
        return;
    }

    lastStatusTime = currentTime;

    const wl_status_t currentStatus = WiFi.status();

    if (currentStatus == WL_CONNECTED)
    {
        LOGF(
            "Connected | SSID: %s | RSSI: %d dBm | IP: %s",
            WiFi.SSID().c_str(),
            WiFi.RSSI(),
            WiFi.localIP().toString().c_str()
        );
    }
    else
    {
        LOGWF(
            "Waiting for Wi-Fi | Status: %s (%d)",
            wifiStatusToString(currentStatus),
            static_cast<int>(currentStatus)
        );
    }
}

void addConfiguredNetworks()
{
    LOG("Configured Wi-Fi networks:");

    wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWORD_1);
    LOGF("  1. %s", WIFI_SSID_1);

#ifdef WIFI_SSID_2
    wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWORD_2);
    LOGF("  2. %s", WIFI_SSID_2);
#endif

#ifdef WIFI_SSID_3
    wifiMulti.addAP(WIFI_SSID_3, WIFI_PASSWORD_3);
    LOGF("  3. %s", WIFI_SSID_3);
#endif
}
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 WiFiMulti Demo");
    LOG("=====================================");

    /*
     * Station mode connects the ESP32 to an existing wireless network.
     */
    WiFi.mode(WIFI_STA);

    /*
     * Do not save credentials to flash each time the demo starts.
     */
    WiFi.persistent(false);

    /*
     * Enable the ESP32 Wi-Fi driver's reconnect behavior in addition to
     * regularly servicing WiFiMulti from loop().
     */
    WiFi.setAutoReconnect(true);

    addConfiguredNetworks();

    LOG("Starting WiFiMulti connection management.");

    /*
     * Perform the first connection attempt immediately instead of waiting
     * for the normal service interval.
     */
    serviceWiFi(true);
}

void loop()
{
    serviceWiFi();
    printPeriodicStatus();

    delay(10);
}