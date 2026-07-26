#pragma once

#include <Arduino.h>

/**
 * Physical buttons supported by the Buttons library.
 */
enum class Button
{
    Boot
};

/**
 * Initializes the physical buttons and their internal state.
 *
 * Call once from setup() before calling Buttons_Update().
 */
void Buttons_Begin();

/**
 * Updates button debouncing and press timing.
 *
 * Call once during every pass through loop().
 */
void Buttons_Update();

/**
 * Returns true once after a brief press is released.
 */
bool Button_WasPressed(Button button);

/**
 * Returns true once after a long press is released.
 *
 * A press that reaches the very-long threshold does not also generate
 * a long-press event.
 */
bool Button_WasLongPressed(Button button);

/**
 * Returns true once when a button reaches the very-long-press interval.
 *
 * The event fires while the button is still held.
 */
bool Button_WasVeryLongPressed(Button button);
