#include "Display_Text.h"
#include "Display_GFX.h"

#include <ctype.h>
#include <string.h>

namespace
{
    constexpr uint8_t FontWidth = 5;
    constexpr uint8_t FontHeight = 7;
    constexpr uint8_t CharacterSpacing = 1;

    int16_t CursorX = 0;
    int16_t CursorY = 0;
    int16_t LineStartX = 0;

    uint16_t CurrentTextColor = 0xFFFF;
    uint16_t CurrentBackgroundColor = 0x0000;

    uint8_t CurrentTextScale = 1;

    bool GetCharacterBitmap(
        char character,
        uint8_t bitmap[FontWidth])
    {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character)));

        switch (character)
        {
            case ' ':
                bitmap[0] = 0x00;
                bitmap[1] = 0x00;
                bitmap[2] = 0x00;
                bitmap[3] = 0x00;
                bitmap[4] = 0x00;
                return true;

            case 'A':
                bitmap[0] = 0x7E;
                bitmap[1] = 0x11;
                bitmap[2] = 0x11;
                bitmap[3] = 0x11;
                bitmap[4] = 0x7E;
                return true;

            case 'B':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x49;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x36;
                return true;

            case 'C':
                bitmap[0] = 0x3E;
                bitmap[1] = 0x41;
                bitmap[2] = 0x41;
                bitmap[3] = 0x41;
                bitmap[4] = 0x22;
                return true;

            case 'D':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x41;
                bitmap[2] = 0x41;
                bitmap[3] = 0x22;
                bitmap[4] = 0x1C;
                return true;

            case 'E':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x49;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x41;
                return true;

            case 'F':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x09;
                bitmap[2] = 0x09;
                bitmap[3] = 0x09;
                bitmap[4] = 0x01;
                return true;

            case 'G':
                bitmap[0] = 0x3E;
                bitmap[1] = 0x41;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x7A;
                return true;

            case 'H':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x08;
                bitmap[2] = 0x08;
                bitmap[3] = 0x08;
                bitmap[4] = 0x7F;
                return true;

            case 'I':
                bitmap[0] = 0x00;
                bitmap[1] = 0x41;
                bitmap[2] = 0x7F;
                bitmap[3] = 0x41;
                bitmap[4] = 0x00;
                return true;

            case 'J':
                bitmap[0] = 0x20;
                bitmap[1] = 0x40;
                bitmap[2] = 0x41;
                bitmap[3] = 0x3F;
                bitmap[4] = 0x01;
                return true;

            case 'K':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x08;
                bitmap[2] = 0x14;
                bitmap[3] = 0x22;
                bitmap[4] = 0x41;
                return true;

            case 'L':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x40;
                bitmap[2] = 0x40;
                bitmap[3] = 0x40;
                bitmap[4] = 0x40;
                return true;

            case 'M':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x02;
                bitmap[2] = 0x0C;
                bitmap[3] = 0x02;
                bitmap[4] = 0x7F;
                return true;

            case 'N':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x04;
                bitmap[2] = 0x08;
                bitmap[3] = 0x10;
                bitmap[4] = 0x7F;
                return true;

            case 'O':
                bitmap[0] = 0x3E;
                bitmap[1] = 0x41;
                bitmap[2] = 0x41;
                bitmap[3] = 0x41;
                bitmap[4] = 0x3E;
                return true;

            case 'P':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x09;
                bitmap[2] = 0x09;
                bitmap[3] = 0x09;
                bitmap[4] = 0x06;
                return true;

            case 'Q':
                bitmap[0] = 0x3E;
                bitmap[1] = 0x41;
                bitmap[2] = 0x51;
                bitmap[3] = 0x21;
                bitmap[4] = 0x5E;
                return true;

            case 'R':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x09;
                bitmap[2] = 0x19;
                bitmap[3] = 0x29;
                bitmap[4] = 0x46;
                return true;

            case 'S':
                bitmap[0] = 0x46;
                bitmap[1] = 0x49;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x31;
                return true;

            case 'T':
                bitmap[0] = 0x01;
                bitmap[1] = 0x01;
                bitmap[2] = 0x7F;
                bitmap[3] = 0x01;
                bitmap[4] = 0x01;
                return true;

            case 'U':
                bitmap[0] = 0x3F;
                bitmap[1] = 0x40;
                bitmap[2] = 0x40;
                bitmap[3] = 0x40;
                bitmap[4] = 0x3F;
                return true;

            case 'V':
                bitmap[0] = 0x1F;
                bitmap[1] = 0x20;
                bitmap[2] = 0x40;
                bitmap[3] = 0x20;
                bitmap[4] = 0x1F;
                return true;

            case 'W':
                bitmap[0] = 0x7F;
                bitmap[1] = 0x20;
                bitmap[2] = 0x18;
                bitmap[3] = 0x20;
                bitmap[4] = 0x7F;
                return true;

            case 'X':
                bitmap[0] = 0x63;
                bitmap[1] = 0x14;
                bitmap[2] = 0x08;
                bitmap[3] = 0x14;
                bitmap[4] = 0x63;
                return true;

            case 'Y':
                bitmap[0] = 0x03;
                bitmap[1] = 0x04;
                bitmap[2] = 0x78;
                bitmap[3] = 0x04;
                bitmap[4] = 0x03;
                return true;

            case 'Z':
                bitmap[0] = 0x61;
                bitmap[1] = 0x51;
                bitmap[2] = 0x49;
                bitmap[3] = 0x45;
                bitmap[4] = 0x43;
                return true;

            case '0':
                bitmap[0] = 0x3E;
                bitmap[1] = 0x51;
                bitmap[2] = 0x49;
                bitmap[3] = 0x45;
                bitmap[4] = 0x3E;
                return true;

            case '1':
                bitmap[0] = 0x00;
                bitmap[1] = 0x42;
                bitmap[2] = 0x7F;
                bitmap[3] = 0x40;
                bitmap[4] = 0x00;
                return true;

            case '2':
                bitmap[0] = 0x42;
                bitmap[1] = 0x61;
                bitmap[2] = 0x51;
                bitmap[3] = 0x49;
                bitmap[4] = 0x46;
                return true;

            case '3':
                bitmap[0] = 0x21;
                bitmap[1] = 0x41;
                bitmap[2] = 0x45;
                bitmap[3] = 0x4B;
                bitmap[4] = 0x31;
                return true;

            case '4':
                bitmap[0] = 0x18;
                bitmap[1] = 0x14;
                bitmap[2] = 0x12;
                bitmap[3] = 0x7F;
                bitmap[4] = 0x10;
                return true;

            case '5':
                bitmap[0] = 0x27;
                bitmap[1] = 0x45;
                bitmap[2] = 0x45;
                bitmap[3] = 0x45;
                bitmap[4] = 0x39;
                return true;

            case '6':
                bitmap[0] = 0x3C;
                bitmap[1] = 0x4A;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x30;
                return true;

            case '7':
                bitmap[0] = 0x01;
                bitmap[1] = 0x71;
                bitmap[2] = 0x09;
                bitmap[3] = 0x05;
                bitmap[4] = 0x03;
                return true;

            case '8':
                bitmap[0] = 0x36;
                bitmap[1] = 0x49;
                bitmap[2] = 0x49;
                bitmap[3] = 0x49;
                bitmap[4] = 0x36;
                return true;

            case '9':
                bitmap[0] = 0x06;
                bitmap[1] = 0x49;
                bitmap[2] = 0x49;
                bitmap[3] = 0x29;
                bitmap[4] = 0x1E;
                return true;

            case ':':
                bitmap[0] = 0x00;
                bitmap[1] = 0x36;
                bitmap[2] = 0x36;
                bitmap[3] = 0x00;
                bitmap[4] = 0x00;
                return true;

            case '.':
                bitmap[0] = 0x00;
                bitmap[1] = 0x60;
                bitmap[2] = 0x60;
                bitmap[3] = 0x00;
                bitmap[4] = 0x00;
                return true;

            case ',':
                bitmap[0] = 0x00;
                bitmap[1] = 0x40;
                bitmap[2] = 0x20;
                bitmap[3] = 0x00;
                bitmap[4] = 0x00;
                return true;

            case '-':
                bitmap[0] = 0x08;
                bitmap[1] = 0x08;
                bitmap[2] = 0x08;
                bitmap[3] = 0x08;
                bitmap[4] = 0x08;
                return true;

            case '/':
                bitmap[0] = 0x20;
                bitmap[1] = 0x10;
                bitmap[2] = 0x08;
                bitmap[3] = 0x04;
                bitmap[4] = 0x02;
                return true;

            case '%':
                bitmap[0] = 0x23;
                bitmap[1] = 0x13;
                bitmap[2] = 0x08;
                bitmap[3] = 0x64;
                bitmap[4] = 0x62;
                return true;

            case '+':
                bitmap[0] = 0x08;
                bitmap[1] = 0x08;
                bitmap[2] = 0x3E;
                bitmap[3] = 0x08;
                bitmap[4] = 0x08;
                return true;

            case '=':
                bitmap[0] = 0x14;
                bitmap[1] = 0x14;
                bitmap[2] = 0x14;
                bitmap[3] = 0x14;
                bitmap[4] = 0x14;
                return true;

            case '?':
            default:
                bitmap[0] = 0x02;
                bitmap[1] = 0x01;
                bitmap[2] = 0x51;
                bitmap[3] = 0x09;
                bitmap[4] = 0x06;
                return false;
        }
    }
}

void Display_DrawChar(
    int16_t x,
    int16_t y,
    char character,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale)
{
    if (scale == 0)
    {
        return;
    }

    uint8_t bitmap[FontWidth];

    GetCharacterBitmap(
        character,
        bitmap);

    for (uint8_t column = 0;
         column < FontWidth;
         column++)
    {
        for (uint8_t row = 0;
             row < FontHeight;
             row++)
        {
            const bool pixelIsSet =
                bitmap[column] & (1U << row);

            const uint16_t color =
                pixelIsSet
                    ? textColor
                    : backgroundColor;

            Display_FillRect(
                x + column * scale,
                y + row * scale,
                scale,
                scale,
                color);
        }
    }

    Display_FillRect(
        x + FontWidth * scale,
        y,
        CharacterSpacing * scale,
        FontHeight * scale,
        backgroundColor);
}

void Display_DrawText(
    int16_t x,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale)
{
    if (text == nullptr || scale == 0)
    {
        return;
    }

    int16_t cursorX = x;

    while (*text != '\0')
    {
        Display_DrawChar(
            cursorX,
            y,
            *text,
            textColor,
            backgroundColor,
            scale);

        cursorX +=
            (FontWidth + CharacterSpacing) * scale;

        text++;
    }
}


void Display_DrawTextCentered(
    int16_t centerX,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale)
{
    const int16_t width =
        Display_GetTextWidth(text, scale);

    Display_DrawText(
        centerX - (width / 2),
        y,
        text,
        textColor,
        backgroundColor,
        scale);
}

void Display_DrawTextRight(
    int16_t rightX,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale)
{
    const int16_t width =
        Display_GetTextWidth(text, scale);

    Display_DrawText(
        rightX - width,
        y,
        text,
        textColor,
        backgroundColor,
        scale);
}


int16_t Display_GetTextWidth(const char* text, uint8_t scale)
{
    if (text == nullptr || scale == 0)
    {
        return 0;
    }

    const size_t characterCount = strlen(text);

    if (characterCount == 0)
    {
        return 0;
    }

    return static_cast<int16_t>(
        characterCount *
        (FontWidth + CharacterSpacing) *
        scale);
}

void Display_SetCursor(
    int16_t x,
    int16_t y)
{
    CursorX = x;
    CursorY = y;
    LineStartX = x;
}

void Display_SetTextColor(
    uint16_t textColor,
    uint16_t backgroundColor)
{
    CurrentTextColor = textColor;
    CurrentBackgroundColor = backgroundColor;
}

void Display_SetTextScale(
    uint8_t scale)
{
    if (scale == 0)
    {
        return;
    }

    CurrentTextScale = scale;
}

void Display_Print(
    const char* text)
{
    if (text == nullptr)
    {
        return;
    }

    Display_DrawText(
        CursorX,
        CursorY,
        text,
        CurrentTextColor,
        CurrentBackgroundColor,
        CurrentTextScale);

    CursorX += Display_GetTextWidth(
        text,
        CurrentTextScale);
}

void Display_Println(
    const char* text)
{
    Display_Print(text);
    Display_Println();
}

void Display_Println()
{
    CursorX = LineStartX;

    CursorY +=
        (FontHeight + CharacterSpacing) *
        CurrentTextScale;
}