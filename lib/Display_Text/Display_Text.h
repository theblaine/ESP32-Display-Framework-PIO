#pragma once

#include <Arduino.h>

void Display_DrawChar(
    int16_t x,
    int16_t y,
    char character,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale = 1);

void Display_DrawText(
    int16_t x,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale = 1);

void Display_DrawTextCentered(
    int16_t centerX,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale = 1);

void Display_DrawTextRight(
    int16_t rightX,
    int16_t y,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale = 1);

void Display_DrawTextWrapped(
    int16_t x,
    int16_t y,
    int16_t maxWidth,
    const char* text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale);

void Display_DrawTextWrapped(
    int16_t x,
    int16_t y,
    int16_t maxWidth,
    const char *text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale = 1);

int16_t Display_GetTextWidth(
    const char* text,
    uint8_t scale = 1);

int16_t Display_GetLineHeight(
    uint8_t scale);

void Display_DrawTextWrapped(
    int16_t x,
    int16_t y,
    int16_t maxWidth,
    const char *text,
    uint16_t textColor,
    uint16_t backgroundColor,
    uint8_t scale);

void Display_SetCursor(
    int16_t x,
    int16_t y);

void Display_SetTextColor(
    uint16_t textColor,
    uint16_t backgroundColor);

void Display_SetTextScale(
    uint8_t scale);

void Display_Print(
    const char* text);

void Display_Println(
    const char* text);

void Display_Println();