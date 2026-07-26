#include <Arduino.h>

#include "Logger.h"
#include "RGB_lamp.h"

namespace
{
constexpr unsigned long COLOR_DURATION_MS = 1500;
constexpr uint8_t BRIGHTNESS = 64;

struct Color
{
    const char* name;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

constexpr Color COLORS[] = {
    {"Red",     BRIGHTNESS, 0,          0},
    {"Green",   0,          BRIGHTNESS, 0},
    {"Blue",    0,          0,          BRIGHTNESS},
    {"Yellow",  BRIGHTNESS, BRIGHTNESS, 0},
    {"Cyan",    0,          BRIGHTNESS, BRIGHTNESS},
    {"Magenta", BRIGHTNESS, 0,          BRIGHTNESS},
    {"White",   BRIGHTNESS, BRIGHTNESS, BRIGHTNESS},
    {"Off",     0,          0,          0},
};

constexpr size_t COLOR_COUNT =
    sizeof(COLORS) / sizeof(COLORS[0]);

size_t currentColorIndex = 0;
unsigned long lastColorChange = 0;

void showColor(const Color& color)
{
    Set_Color(color.red, color.green, color.blue);

    LOGF(
        "RGB LED: %-8s | R=%3u G=%3u B=%3u",
        color.name,
        color.red,
        color.green,
        color.blue
    );
}
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 RGB LED Demo");
    LOG("=====================================");
    LOGF("RGB LED GPIO: %d", PIN_NEOPIXEL);
    LOGF("Color duration: %lu ms", COLOR_DURATION_MS);

    showColor(COLORS[currentColorIndex]);
    lastColorChange = millis();
}

void loop()
{
    if (millis() - lastColorChange < COLOR_DURATION_MS)
    {
        return;
    }

    lastColorChange = millis();

    currentColorIndex++;

    if (currentColorIndex >= COLOR_COUNT)
    {
        currentColorIndex = 0;
    }

    showColor(COLORS[currentColorIndex]);
}