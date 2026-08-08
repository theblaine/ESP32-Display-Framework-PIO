#include "Display_ST7789.h"

#include <SPI.h>

#include "DisplayConfig.h"

namespace
{
    // Maximum PWM duty cycle for the configured resolution.
    constexpr uint32_t BacklightMaxDuty =
        (1UL << DisplayConfig::BacklightPwmResolution) - 1UL;

    // Cached brightness percentage.
    uint8_t CurrentBrightness = 100;

#if defined(CONFIG_IDF_TARGET_ESP32S3)
    SPIClass LCDspi(FSPI);
#else
    SPIClass LCDspi(VSPI);
#endif

    // Initialize the SPI bus used by the LCD.
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
        uint8_t *writeData,
        uint8_t *readData,
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

    // Performs the LCD hardware reset sequence.
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

    /*
     * ST7789 controller initialization sequence.
     *
     * These register values are based on the reference implementation
     * and have been verified on the supported display boards.
     */
    LCD_WriteCommand(0x11);
    delay(120);

    LCD_WriteCommand(0x36);
    LCD_WriteData(DisplayConfig::MemoryAccessControl);

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
        const uint16_t columnStart = xStart + DisplayConfig::OffsetX;
        const uint16_t columnEnd = xEnd + DisplayConfig::OffsetX;
        const uint16_t rowStart = yStart + DisplayConfig::OffsetY;
        const uint16_t rowEnd = yEnd + DisplayConfig::OffsetY;

        LCD_WriteCommand(0x2A);
        LCD_WriteData(columnStart >> 8);
        LCD_WriteData(columnStart & 0xFF);
        LCD_WriteData(columnEnd >> 8);
        LCD_WriteData(columnEnd & 0xFF);

        LCD_WriteCommand(0x2B);
        LCD_WriteData(rowStart >> 8);
        LCD_WriteData(rowStart & 0xFF);
        LCD_WriteData(rowEnd >> 8);
        LCD_WriteData(rowEnd & 0xFF);
    }
    else
    {
        const uint16_t columnStart = yStart + DisplayConfig::OffsetY;
        const uint16_t columnEnd = yEnd + DisplayConfig::OffsetY;
        const uint16_t rowStart = xStart + DisplayConfig::OffsetX;
        const uint16_t rowEnd = xEnd + DisplayConfig::OffsetX;

        LCD_WriteCommand(0x2A);
        LCD_WriteData(columnStart >> 8);
        LCD_WriteData(columnStart & 0xFF);
        LCD_WriteData(columnEnd >> 8);
        LCD_WriteData(columnEnd & 0xFF);

        LCD_WriteCommand(0x2B);
        LCD_WriteData(rowStart >> 8);
        LCD_WriteData(rowStart & 0xFF);
        LCD_WriteData(rowEnd >> 8);
        LCD_WriteData(rowEnd & 0xFF);
    }

    LCD_WriteCommand(0x2C);
}

void LCD_AddWindow(
    uint16_t xStart,
    uint16_t yStart,
    uint16_t xEnd,
    uint16_t yEnd,
    uint16_t *color)
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
        reinterpret_cast<uint8_t *>(color),
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