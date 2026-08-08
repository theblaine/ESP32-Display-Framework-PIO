#include "Buttons.h"

namespace
{
    /*
     * The BOOT button is currently GPIO 0 on the supported boards.
     *
     * If future boards use different button mappings, this should move
     * into board-specific configuration rather than adding board checks
     * inside this library.
     */
    constexpr uint8_t BOOT_BUTTON_PIN = 0;

    constexpr uint32_t BUTTON_DEBOUNCE_MS = 25;
    constexpr uint32_t BUTTON_LONG_PRESS_MS = 750;
    constexpr uint32_t BUTTON_VERY_LONG_PRESS_MS = 3000;

    struct ButtonState
    {
        bool rawPressed = false;
        bool stablePressed = false;

        bool pressedEvent = false;
        bool longPressedEvent = false;
        bool veryLongPressedEvent = false;

        bool veryLongPressReported = false;

        uint32_t rawChangedAt = 0;
        uint32_t pressedAt = 0;
    };

    ButtonState g_bootButton;

    bool readBootButton()
    {
        return digitalRead(
                   BOOT_BUTTON_PIN) == LOW;
    }

    ButtonState *getButtonState(
        Button button)
    {
        switch (button)
        {
        case Button::Boot:
            return &g_bootButton;
        }

        return nullptr;
    }
}

void Buttons_Begin()
{
    pinMode(
        BOOT_BUTTON_PIN,
        INPUT_PULLUP);

    const bool pressed =
        readBootButton();

    const uint32_t now =
        millis();

    g_bootButton.rawPressed =
        pressed;

    g_bootButton.stablePressed =
        pressed;

    g_bootButton.rawChangedAt =
        now;

    g_bootButton.pressedAt =
        pressed
            ? now
            : 0;

    g_bootButton.veryLongPressReported =
        false;

    g_bootButton.pressedEvent =
        false;

    g_bootButton.longPressedEvent =
        false;

    g_bootButton.veryLongPressedEvent =
        false;
}

void Buttons_Update()
{
    const uint32_t now =
        millis();

    const bool rawPressed =
        readBootButton();

    // Button events are valid for one update cycle only.
    g_bootButton.pressedEvent =
        false;

    g_bootButton.longPressedEvent =
        false;

    g_bootButton.veryLongPressedEvent =
        false;

    // Track changes in the raw GPIO state.
    if (rawPressed !=
        g_bootButton.rawPressed)
    {
        g_bootButton.rawPressed =
            rawPressed;

        g_bootButton.rawChangedAt =
            now;
    }

    // Accept a raw state change only after the debounce interval.
    if ((now -
         g_bootButton.rawChangedAt) >=
            BUTTON_DEBOUNCE_MS &&
        g_bootButton.stablePressed !=
            g_bootButton.rawPressed)
    {
        g_bootButton.stablePressed =
            g_bootButton.rawPressed;

        if (g_bootButton.stablePressed)
        {
            // Beginning of a new button press.
            g_bootButton.pressedAt =
                now;

            g_bootButton.veryLongPressReported =
                false;
        }
        else
        {
            // Button released. Classify the completed press.
            const uint32_t heldFor =
                now -
                g_bootButton.pressedAt;

            if (!g_bootButton.veryLongPressReported)
            {
                if (heldFor >=
                    BUTTON_LONG_PRESS_MS)
                {
                    g_bootButton.longPressedEvent =
                        true;
                }
                else
                {
                    g_bootButton.pressedEvent =
                        true;
                }
            }
        }
    }

    // A very-long press fires immediately when the threshold is reached.
    if (g_bootButton.stablePressed &&
        !g_bootButton.veryLongPressReported &&
        (now -
         g_bootButton.pressedAt) >=
            BUTTON_VERY_LONG_PRESS_MS)
    {
        g_bootButton.veryLongPressReported =
            true;

        g_bootButton.veryLongPressedEvent =
            true;
    }
}

bool Button_WasPressed(
    Button button)
{
    ButtonState *state =
        getButtonState(button);

    return state != nullptr &&
           state->pressedEvent;
}

bool Button_WasLongPressed(
    Button button)
{
    ButtonState *state =
        getButtonState(button);

    return state != nullptr &&
           state->longPressedEvent;
}

bool Button_WasVeryLongPressed(
    Button button)
{
    ButtonState *state =
        getButtonState(button);

    return state != nullptr &&
           state->veryLongPressedEvent;
}