#include "Display_ST7789.h"

namespace
{
  constexpr uint32_t BACKLIGHT_MAX_DUTY = (1UL << WaveshareDisplayConfig::BacklightPwmResolution) - 1UL;
  uint8_t CurrentBrightness = 100;
}

   
SPIClass LCDspi(FSPI);
#define SPI_WRITE(_dat)         LCDspi.transfer(_dat)
#define SPI_WRITE_Word(_dat)    LCDspi.transfer16(_dat)
void SPI_Init()
{
  LCDspi.begin(WaveshareDisplayConfig::PinClock,WaveshareDisplayConfig::PinMiso,WaveshareDisplayConfig::PinMosi); 
}

void LCD_WriteCommand(uint8_t Cmd)  
{ 
  LCDspi.beginTransaction(SPISettings(WaveshareDisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, LOW);  
  digitalWrite(WaveshareDisplayConfig::PinDataCommand, LOW); 
  SPI_WRITE(Cmd);
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, HIGH);  
  LCDspi.endTransaction();
}
void LCD_WriteData(uint8_t Data) 
{ 
  LCDspi.beginTransaction(SPISettings(WaveshareDisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, LOW);  
  digitalWrite(WaveshareDisplayConfig::PinDataCommand, HIGH);  
  SPI_WRITE(Data);  
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, HIGH);  
  LCDspi.endTransaction();
}    
void LCD_WriteData_Word(uint16_t Data)
{
  LCDspi.beginTransaction(SPISettings(WaveshareDisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, LOW);  
  digitalWrite(WaveshareDisplayConfig::PinDataCommand, HIGH); 
  SPI_WRITE_Word(Data);
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, HIGH);  
  LCDspi.endTransaction();
}   
void LCD_WriteData_nbyte(uint8_t* SetData,uint8_t* ReadData,uint32_t Size) 
{ 
  LCDspi.beginTransaction(SPISettings(WaveshareDisplayConfig::SpiFrequency, MSBFIRST, SPI_MODE0));
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, LOW);  
  digitalWrite(WaveshareDisplayConfig::PinDataCommand, HIGH);  
  LCDspi.transferBytes(SetData, ReadData, Size);
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, HIGH);  
  LCDspi.endTransaction();
} 

void LCD_Reset(void)
{
  digitalWrite(WaveshareDisplayConfig::PinChipSelect, LOW);       
  delay(50);
  digitalWrite(WaveshareDisplayConfig::PinReset, LOW); 
  delay(50);
  digitalWrite(WaveshareDisplayConfig::PinReset, HIGH); 
  delay(50);
}
void LCD_Init(void)
{
  pinMode(WaveshareDisplayConfig::PinChipSelect, OUTPUT);
  pinMode(WaveshareDisplayConfig::PinDataCommand, OUTPUT);
  pinMode(WaveshareDisplayConfig::PinReset, OUTPUT); 
  Backlight_Init();
  SPI_Init();

  LCD_Reset();
  //************* Start Initial Sequence **********// 
  LCD_WriteCommand(0x11);
  delay(120);
  LCD_WriteCommand(0x36);
  if (WaveshareDisplayConfig::Horizontal)
      LCD_WriteData(0x00);
  else
      LCD_WriteData(0x70);

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
/******************************************************************************
function: Set the cursor position
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinatesen
******************************************************************************/
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend)
{ 
  if (WaveshareDisplayConfig::Horizontal) {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + WaveshareDisplayConfig::OffsetX);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + WaveshareDisplayConfig::OffsetX);
    
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + WaveshareDisplayConfig::OffsetY);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + WaveshareDisplayConfig::OffsetY);
  }
  else {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + WaveshareDisplayConfig::OffsetY);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + WaveshareDisplayConfig::OffsetY);
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + WaveshareDisplayConfig::OffsetX);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + WaveshareDisplayConfig::OffsetX);
  }
  LCD_WriteCommand(0x2C);
}
/******************************************************************************
function: Refresh the image in an area
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinates
    color :   Set the color
******************************************************************************/
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color)
{             
  uint16_t Show_Width = Xend - Xstart + 1;
  uint16_t Show_Height = Yend - Ystart + 1;
  uint32_t numBytes = Show_Width * Show_Height * sizeof(uint16_t);
  uint8_t Read_D[numBytes];
  LCD_SetCursor(Xstart, Ystart, Xend, Yend);
  LCD_WriteData_nbyte((uint8_t*)color, Read_D, numBytes);        
}
// backlight
void Backlight_Init(void)
{
  ledcAttach(WaveshareDisplayConfig::PinBacklight, WaveshareDisplayConfig::BacklightPwmFrequency, WaveshareDisplayConfig::BacklightPwmResolution);
  Display_SetBrightness(CurrentBrightness);
}

void Display_SetBrightness(uint8_t percent)
{
  if (percent > 100)
  {
    percent = 100;
  }

  CurrentBrightness = percent;

  const uint32_t duty =
      (static_cast<uint32_t>(percent) * BACKLIGHT_MAX_DUTY) / 100UL;

  ledcWrite(WaveshareDisplayConfig::PinBacklight, duty);
}

uint8_t Display_GetBrightness()
{
  return CurrentBrightness;
}

void Set_Backlight(uint8_t light)
{
  Display_SetBrightness(light);
}
