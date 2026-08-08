/*
 * Wireless
 *
 * Legacy hardware test helpers used by the original
 * Waveshare demonstration programs.
 *
 * Provides simple Wi-Fi and BLE scanning routines.
 *
 * Not used by the Display Framework applications.
 */

#pragma once
#include "WiFi.h"
#include <BLEDevice.h>
#include <BLEScan.h>

extern bool WIFI_Connection;
extern uint8_t WIFI_NUM;
extern uint8_t BLE_NUM;
extern bool Scan_finish;

void Wireless_Test2();