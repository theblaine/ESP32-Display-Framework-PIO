#pragma once

#include <Arduino.h>

/*
 * RGB_Lamp
 *
 * Controls the onboard addressable RGB LED.
 *
 * Framework code should use the RGBLamp namespace.
 *
 * RGB_Lamp_Loop() is retained for compatibility with the original
 * demonstration applications that display a rainbow animation.
 */

#define PIN_NEOPIXEL 38

// Sets the RGB LED using standard RGB color values.
void Set_Color(
    uint8_t red,
    uint8_t green,
    uint8_t blue);

// Advances the legacy rainbow animation.
void RGB_Lamp_Loop(
    uint16_t waiting);




namespace RGBLamp
{
    // Initializes the RGB LED.
    void begin();

    // Sets the LED using standard RGB values.
    void setColor(
        uint8_t red,
        uint8_t green,
        uint8_t blue);

    // Turns the LED off.
    void off();
}