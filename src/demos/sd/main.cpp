#include <Arduino.h>
#include <SD_MMC.h>

#include "Logger.h"
#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "SD_Card.h"

namespace
{
    constexpr const char* MESSAGE_FILE_PATH = "/message.txt";

    String formatCardSize(uint64_t totalBytes)
    {
        const uint64_t totalMegabytes =
            totalBytes / (1024ULL * 1024ULL);

        return String(totalMegabytes) + " MB";
    }

    const char* cardTypeToString(uint8_t cardType)
    {
        switch (cardType)
        {
            case CARD_MMC:
                return "MMC";

            case CARD_SD:
                return "SDSC";

            case CARD_SDHC:
                return "SDHC";

            case CARD_NONE:
                return "None";

            default:
                return "Unknown";
        }
    }

    bool readTextFile(
        const char* path,
        String& contents)
    {
        File file = SD_MMC.open(path, FILE_READ);

        if (!file)
        {
            LOGEF(
                "Unable to open file: %s",
                path);

            return false;
        }

        if (file.isDirectory())
        {
            LOGEF(
                "Path is a directory: %s",
                path);

            file.close();
            return false;
        }

        contents = "";

        while (file.available())
        {
            contents +=
                static_cast<char>(file.read());
        }

        file.close();

        return true;
    }
}

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 SD Card Read Demo");
    LOG("=====================================");

    Display::begin();

    Display_ShowStatusScreen(
        "SD Card",
        "Mounting",
        Color::Yellow);

    SD_Init();

    const uint8_t cardType =
        SD_MMC.cardType();

    if (cardType == CARD_NONE)
    {
        LOGE("No SD card detected.");

        Display_ShowStatusScreen(
            "SD Card",
            "Failed",
            Color::Red,
            "No card detected");

        return;
    }

    const uint64_t totalBytes =
        SD_MMC.totalBytes();

    LOGF(
        "Card mounted | Type: %s | Size: %s",
        cardTypeToString(cardType),
        formatCardSize(totalBytes).c_str());

    Display_ShowStatusScreen(
        "SD Card",
        "Reading",
        Color::Yellow,
        MESSAGE_FILE_PATH);

    String fileContents;

    if (!readTextFile(
            MESSAGE_FILE_PATH,
            fileContents))
    {
        Display_ShowStatusScreen(
            "SD Card",
            "Read Failed",
            Color::Red,
            MESSAGE_FILE_PATH);

        return;
    }

    LOGF(
        "Read %u bytes from %s",
        fileContents.length(),
        MESSAGE_FILE_PATH);

    LOGF(
        "Contents: %s",
        fileContents.c_str());

    Display_ShowStatusScreen(
        "SD Card",
        "File Loaded",
        Color::Green,
        fileContents.c_str());
}

void loop()
{
    delay(1000);
}