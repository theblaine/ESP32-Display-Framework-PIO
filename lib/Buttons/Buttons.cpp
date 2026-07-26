#include "Buttons.h"

namespace
{
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

    ButtonState BootButton;

    bool ReadBootButton()
    {
        return digitalRead(BOOT_BUTTON_PIN) == LOW;
    }

    ButtonState* GetButtonState(Button button)
    {
        switch (button)
        {
            case Button::Boot:
                return &BootButton;
        }

        return nullptr;
    }
}

void Buttons_Begin()
{
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

    const bool pressed = ReadBootButton();
    const uint32_t now = millis();

    BootButton.rawPressed = pressed;
    BootButton.stablePressed = pressed;
    BootButton.rawChangedAt = now;
    BootButton.pressedAt = pressed ? now : 0;
    BootButton.veryLongPressReported = false;
    BootButton.pressedEvent = false;
    BootButton.longPressedEvent = false;
    BootButton.veryLongPressedEvent = false;
}

void Buttons_Update()
{
    const uint32_t now = millis();
    const bool rawPressed = ReadBootButton();

    // Events remain true for one update cycle.
    BootButton.pressedEvent = false;
    BootButton.longPressedEvent = false;
    BootButton.veryLongPressedEvent = false;

    if (rawPressed != BootButton.rawPressed)
    {
        BootButton.rawPressed = rawPressed;
        BootButton.rawChangedAt = now;
    }

    if ((now - BootButton.rawChangedAt) >= BUTTON_DEBOUNCE_MS &&
        BootButton.stablePressed != BootButton.rawPressed)
    {
        BootButton.stablePressed = BootButton.rawPressed;

        if (BootButton.stablePressed)
        {
            BootButton.pressedAt = now;
            BootButton.veryLongPressReported = false;
        }
        else
        {
            const uint32_t heldFor = now - BootButton.pressedAt;

            if (!BootButton.veryLongPressReported)
            {
                if (heldFor >= BUTTON_LONG_PRESS_MS)
                {
                    BootButton.longPressedEvent = true;
                }
                else
                {
                    BootButton.pressedEvent = true;
                }
            }
        }
    }

    if (BootButton.stablePressed &&
        !BootButton.veryLongPressReported &&
        (now - BootButton.pressedAt) >= BUTTON_VERY_LONG_PRESS_MS)
    {
        BootButton.veryLongPressReported = true;
        BootButton.veryLongPressedEvent = true;
    }
}

bool Button_WasPressed(Button button)
{
    ButtonState* state = GetButtonState(button);
    return state != nullptr && state->pressedEvent;
}

bool Button_WasLongPressed(Button button)
{
    ButtonState* state = GetButtonState(button);
    return state != nullptr && state->longPressedEvent;
}

bool Button_WasVeryLongPressed(Button button)
{
    ButtonState* state = GetButtonState(button);
    return state != nullptr && state->veryLongPressedEvent;
}
