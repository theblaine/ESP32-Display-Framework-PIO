/**
 ******************************************************************************
 * @file     LVGL_Arduino.ino
 * @author   Yongqin Ou
 * @version  V1.0
 * @date     2024-10-30
 * @brief    Setup experiment for multiple modules
 * @license  MIT
 * @copyright Copyright (c) 2024, Waveshare
 ******************************************************************************
 *
 * Experiment Objective: Learn how to set up and use multiple modules including
 * SD card, display, LVGL, wireless, and RGB lamp.
 *
 * Hardware Resources and Pin Assignment:
 * 1. SD Card Interface --> As configured in SD_Card.h.
 * 2. Display Interface --> As configured in Display_ST7789.h.
 * 3. Wireless Module Interface --> As configured in Wireless.h.
 * 4. RGB Lamp Interface --> As configured in RGB_lamp.h.
 *
 * Experiment Phenomenon:
 * 1. Runs various tests and initializations for different modules.
 * 2. Displays LVGL examples on the display.
 * 3. Continuously runs loops for timer, RGB lamp, and other tasks.
 *
 * Notes:
 * None
 *
 ******************************************************************************
 *
 * Development Platform: ESP32
 * Support Forum: service.waveshare.com
 * Company Website: www.waveshare.com
 *
 ******************************************************************************
 */

#include <Arduino.h>

#include "Logger.h"
#include "SD_Card.h"
#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "factory_demo.h"
#include "Wireless.h"
#include "RGB_lamp.h"

void setup()
{
  Logger::begin();
  delay(1000);

  LOG("=====================================");
  LOG(" ESP32-S3 Factory Demo");
  LOG("=====================================");

  LOG("Testing flash.");
  Flash_test();

  LOG("Initializing SD card.");
  SD_Init();

  LOG("Initializing display.");
  LCD_Init();

  Set_Backlight(90);

  LOG("Initializing LVGL.");
  Lvgl_Init();

  LOG("Loading LVGL example.");
  Lvgl_Example1();

  // lv_demo_widgets();
  // lv_demo_benchmark();
  // lv_demo_keypad_encoder();
  // lv_demo_music();
  // lv_demo_stress();

  LOG("Starting wireless scan.");
  Wireless_Test2();

  LOG("Fatory demo initialization complete.");
}

void loop()
{
  Timer_Loop();
  RGB_Lamp_Loop(2);
  delay(5);
}