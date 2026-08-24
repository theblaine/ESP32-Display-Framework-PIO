#pragma once

namespace HomeAssistantPage
{
    void draw();
    void handleMessage(const char *payload);
    bool hasReceivedData();
}