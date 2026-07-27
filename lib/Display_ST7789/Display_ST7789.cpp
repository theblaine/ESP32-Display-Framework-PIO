#include "Display_ST7789.h"

#include <SPI.h>

#include "DisplayConfig.h"

namespace
{
    constexpr uint32_t BacklightMaxDuty =
        (1UL << DisplayConfig::BacklightPwmResolution) - 1UL;

    uint8_t CurrentBrightness = 100;

    SPIClass LCDspi(FSPI);

    void SPI_Init()
    {
        LCDspi.begin(
            DisplayConfig::PinClock,
            DisplayConfig::PinMiso,
            DisplayConfig::PinMosi);
    }

    void LCD_WriteCommand(uint8_t command)
    {
        LCDspi.beginTransaction(
            SPISettings(DisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));

        digitalWrite(DisplayConfig::PinChipSelect, LOW);
        digitalWrite(DisplayConfig::PinDataCommand, LOW);

        LCDspi.transfer(command);

        digitalWrite(DisplayConfig::PinChipSelect, HIGH);
        LCDspi.endTransaction();
    }

    void LCD_WriteData(uint8_t data)
    {
        LCDspi.beginTransaction(
            SPISettings(DisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));

        digitalWrite(DisplayConfig::PinChipSelect, LOW);
        digitalWrite(DisplayConfig::PinDataCommand, HIGH);

        LCDspi.transfer(data);

        digitalWrite(DisplayConfig::PinChipSelect, HIGH);
        LCDspi.endTransaction();
    }

    void LCD_WriteDataWord(uint16_t data)
    {
        LCDspi.beginTransaction(
            SPISettings(DisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));

        digitalWrite(DisplayConfig::PinChipSelect, LOW);
        digitalWrite(DisplayConfig::PinDataCommand, HIGH);

        LCDspi.transfer16(data);

        digitalWrite(DisplayConfig::PinChipSelect, HIGH);
        LCDspi.endTransaction();
    }

    void LCD_WriteDataBytes(
        uint8_t* writeData,
        uint8_t* readData,
        uint32_t size)
    {
        LCDspi.beginTransaction(
            SPISettings(DisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));

        digitalWrite(DisplayConfig::PinChipSelect, LOW);
        digitalWrite(DisplayConfig::PinDataCommand, HIGH);

        LCDspi.transferBytes(writeData, readData, size);

        digitalWrite(DisplayConfig::PinChipSelect, HIGH);
        LCDspi.endTransaction();
    }

    void LCD_Reset()
    {
        digitalWrite(DisplayConfig::PinChipSelect, LOW);
        delay(50);

        digitalWrite(DisplayConfig::PinReset, LOW);
        delay(50);

        digitalWrite(DisplayConfig::PinReset, HIGH);
        delay(50);
    }

    void Backlight_Init()
    {
        ledcAttach(
            DisplayConfig::PinBacklight,
            DisplayConfig::BacklightPwmFrequency,
            DisplayConfig::BacklightPwmResolution);

        Display_SetBrightness(CurrentBrightness);
    }
}

void LCD_Init(void)
{
    pinMode(DisplayConfig::PinChipSelect, OUTPUT);
    pinMode(DisplayConfig::PinDataCommand, OUTPUT);
    pinMode(DisplayConfig::PinReset, OUTPUT);

    Backlight_Init();
    SPI_Init();
    LCD_Reset();

    // Start initialization sequence.
    LCD_WriteCommand(0x11);
    delay(120);

    LCD_WriteCommand(0x36);

    if (DisplayConfig::Horizontal)
    {
        LCD_WriteData(0x00);
    }
    else
    {
        LCD_WriteData(0x70);
    }

    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05);

    LCD_WriteCommand(0xB0);
    LCD_WriteData(0x00);
    LCD_WriteData(0xE8);

    LCD_WriteCommand(0xB2);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x33);
    LCD_WriteData(0x33);

    LCD_WriteCommand(0xB7);
    LCD_WriteData(0x35);

    LCD_WriteCommand(0xBB);
    LCD_WriteData(0x35);

    LCD_WriteCommand(0xC0);
    LCD_WriteData(0x2C);

    LCD_WriteCommand(0xC2);
    LCD_WriteData(0x01);

    LCD_WriteCommand(0xC3);
    LCD_WriteData(0x13);

    LCD_WriteCommand(0xC4);
    LCD_WriteData(0x20);

    LCD_WriteCommand(0xC6);
    LCD_WriteData(0x0F);

    LCD_WriteCommand(0xD0);
    LCD_WriteData(0xA4);
    LCD_WriteData(0xA1);

    LCD_WriteCommand(0xD6);
    LCD_WriteData(0xA1);

    LCD_WriteCommand(0xE0);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);
    LCD_WriteData(0x04);
    LCD_WriteData(0x04);
    LCD_WriteData(0x05);
    LCD_WriteData(0x29);
    LCD_WriteData(0x33);
    LCD_WriteData(0x3E);
    LCD_WriteData(0x38);
    LCD_WriteData(0x12);
    LCD_WriteData(0x12);
    LCD_WriteData(0x28);
    LCD_WriteData(0x30);

    LCD_WriteCommand(0xE1);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x07);
    LCD_WriteData(0x28);
    LCD_WriteData(0x33);
    LCD_WriteData(0x3E);
    LCD_WriteData(0x36);
    LCD_WriteData(0x14);
    LCD_WriteData(0x14);
    LCD_WriteData(0x29);
    LCD_WriteData(0x32);

    LCD_WriteCommand(0x21);

    LCD_WriteCommand(0x11);
    delay(120);

    LCD_WriteCommand(0x29);
}

void LCD_SetCursor(
    uint16_t xStart,
    uint16_t yStart,
    uint16_t xEnd,
    uint16_t yEnd)
{
    if (DisplayConfig::Horizontal)
    {
        LCD_WriteCommand(0x2A);
        LCD_WriteData(xStart >> 8);
        LCD_WriteData(xStart + DisplayConfig::OffsetX);
        LCD_WriteData(xEnd >> 8);
        LCD_WriteData(xEnd + DisplayConfig::OffsetX);

        LCD_WriteCommand(0x2B);
        LCD_WriteData(yStart >> 8);
        LCD_WriteData(yStart + DisplayConfig::OffsetY);
        LCD_WriteData(yEnd >> 8);
        LCD_WriteData(yEnd + DisplayConfig::OffsetY);
    }
    else
    {
        LCD_WriteCommand(0x2A);
        LCD_WriteData(yStart >> 8);
        LCD_WriteData(yStart + DisplayConfig::OffsetY);
        LCD_WriteData(yEnd >> 8);
        LCD_WriteData(yEnd + DisplayConfig::OffsetY);

        LCD_WriteCommand(0x2B);
        LCD_WriteData(xStart >> 8);
        LCD_WriteData(xStart + DisplayConfig::OffsetX);
        LCD_WriteData(xEnd >> 8);
        LCD_WriteData(xEnd + DisplayConfig::OffsetX);
    }

    LCD_WriteCommand(0x2C);
}

void LCD_addWindow(
    uint16_t xStart,
    uint16_t yStart,
    uint16_t xEnd,
    uint16_t yEnd,
    uint16_t* color)
{
    const uint16_t width = xEnd - xStart + 1;
    const uint16_t height = yEnd - yStart + 1;
    const uint32_t numberOfBytes =
        static_cast<uint32_t>(width) *
        static_cast<uint32_t>(height) *
        sizeof(uint16_t);

    uint8_t readData[numberOfBytes];

    LCD_SetCursor(xStart, yStart, xEnd, yEnd);

    LCD_WriteDataBytes(
        reinterpret_cast<uint8_t*>(color),
        readData,
        numberOfBytes);
}

void Display_SetBrightness(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    CurrentBrightness = percent;

    const uint32_t duty =
        (static_cast<uint32_t>(percent) * BacklightMaxDuty) / 100UL;

    ledcWrite(DisplayConfig::PinBacklight, duty);
}

uint8_t Display_GetBrightness(void)
{
    return CurrentBrightness;
}