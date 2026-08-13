#pragma once

#include <Arduino.h>

void Search_Image(
    const char *directory,
    const char *fileExtension);

void Display_Image(
    const char *directory,
    const char *fileExtension,
    uint16_t id);

void Image_Next_Loop(
    const char *directory,
    const char *fileExtension,
    uint32_t nextTime);