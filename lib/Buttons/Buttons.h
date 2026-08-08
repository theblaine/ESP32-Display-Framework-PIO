#pragma once

#include <Arduino.h>

/*
 * Buttons
 *
 * Provides debounced button input with support for:
 *
 *   - Short press
 *   - Long press
 *   - Very-long press
 *
 * Short and long press events are reported when the button is released.
 * A very-long press is reported as soon as the configured hold time is
 * reached while the button is still pressed.
 *
 * Event flags remain active for one Buttons_Update() cycle.
 */

/**
 * Physical buttons supported by the Buttons library.
 */
enum class Button
{
    Boot
};

/**
 * Initializes button GPIO and internal state.
 *
 * Call once from setup() before calling Buttons_Update().
 */
void Buttons_Begin();

/**
 * Updates button debouncing, timing, and event state.
 *
 * Call once during every pass through loop().
 */
void Buttons_Update();

/**
 * Returns true during the update cycle in which a short press
 * is released.
 */
bool Button_WasPressed(Button button);

/**
 * Returns true during the update cycle in which a long press
 * is released.
 *
 * A press that reaches the very-long threshold does not also
 * generate a long-press event.
 */
bool Button_WasLongPressed(Button button);

/**
 * Returns true during the update cycle in which the button reaches
 * the very-long-press threshold.
 *
 * Unlike short and long presses, this event fires while the button
 * is still being held.
 */
bool Button_WasVeryLongPressed(Button button);