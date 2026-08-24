#pragma once

namespace FlightRadarPage
{
    void draw();
    void handleMessage(const char *payload);
    bool hasReceivedData();
}