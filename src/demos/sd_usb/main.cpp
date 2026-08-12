#include <Arduino.h>

#include "USB.h"
#include "USBCDC.h"
#include "USBMSC.h"

#include "SD_MMC.h"

USBCDC USBSerial;
USBMSC MSC;

namespace
{
    constexpr int8_t SD_CLK_PIN = 14;
    constexpr int8_t SD_CMD_PIN = 15;
    constexpr int8_t SD_D0_PIN = 16;
    constexpr int8_t SD_D1_PIN = 18;
    constexpr int8_t SD_D2_PIN = 17;
    constexpr int8_t SD_D3_PIN = 21;

    constexpr uint16_t EXPECTED_SECTOR_SIZE = 512;

    uint8_t g_sectorBuffer[EXPECTED_SECTOR_SIZE];

    bool g_ejected = false;

int32_t mscRead(
    uint32_t lba,
    uint32_t offset,
    void *buffer,
    uint32_t bufferSize)
{
    if (buffer == nullptr)
    {
        return -1;
    }

    uint8_t *destination =
        static_cast<uint8_t *>(buffer);

    uint32_t remaining =
        bufferSize;

    uint32_t currentLba =
        lba;

    uint32_t currentOffset =
        offset;

    while (remaining > 0)
    {
        if (currentOffset >=
            EXPECTED_SECTOR_SIZE)
        {
            return -1;
        }

        if (!SD_MMC.readRAW(
                g_sectorBuffer,
                currentLba))
        {
            USBSerial.printf(
                "MSC read failed | LBA: %lu\n",
                static_cast<unsigned long>(
                    currentLba));

            return -1;
        }

        const uint32_t available =
            EXPECTED_SECTOR_SIZE -
            currentOffset;

        const uint32_t bytesToCopy =
            remaining < available
                ? remaining
                : available;

        memcpy(
            destination,
            g_sectorBuffer +
                currentOffset,
            bytesToCopy);

        destination +=
            bytesToCopy;

        remaining -=
            bytesToCopy;

        currentLba++;

        currentOffset = 0;
    }

    return bufferSize;
}

int32_t mscWrite(
    uint32_t lba,
    uint32_t offset,
    uint8_t *buffer,
    uint32_t bufferSize)
{
    if (buffer == nullptr)
    {
        return -1;
    }

    uint8_t *source =
        buffer;

    uint32_t remaining =
        bufferSize;

    uint32_t currentLba =
        lba;

    uint32_t currentOffset =
        offset;

    while (remaining > 0)
    {
        if (currentOffset >=
            EXPECTED_SECTOR_SIZE)
        {
            return -1;
        }

        const uint32_t available =
            EXPECTED_SECTOR_SIZE -
            currentOffset;

        const uint32_t bytesToWrite =
            remaining < available
                ? remaining
                : available;

        /*
         * Preserve untouched bytes for partial-sector writes.
         */
        if (currentOffset != 0 ||
            bytesToWrite !=
                EXPECTED_SECTOR_SIZE)
        {
            if (!SD_MMC.readRAW(
                    g_sectorBuffer,
                    currentLba))
            {
                return -1;
            }
        }

        memcpy(
            g_sectorBuffer +
                currentOffset,
            source,
            bytesToWrite);

        if (!SD_MMC.writeRAW(
                g_sectorBuffer,
                currentLba))
        {
            return -1;
        }

        source +=
            bytesToWrite;

        remaining -=
            bytesToWrite;

        currentLba++;

        currentOffset = 0;
    }

    return bufferSize;
}

    bool mscStartStop(
        uint8_t powerCondition,
        bool start,
        bool loadEject)
    {
        if (loadEject &&
            !start)
        {
            g_ejected = true;

            USBSerial.println(
                "Windows ejected SD card.");
        }

        return true;
    }
}

void setup()
{
    USBSerial.begin();

    delay(500);

    if (!SD_MMC.setPins(
            SD_CLK_PIN,
            SD_CMD_PIN,
            SD_D0_PIN,
            SD_D1_PIN,
            SD_D2_PIN,
            SD_D3_PIN))
    {
        USBSerial.println(
            "SD pin configuration failed.");

        USB.begin();
        return;
    }

    if (!SD_MMC.begin(
            "/sdcard",
            true,
            true))
    {
        USBSerial.println(
            "SD initialization failed.");

        USB.begin();
        return;
    }

    const int sectorSize =
        SD_MMC.sectorSize();

    const int sectorCount =
        SD_MMC.numSectors();

    if (sectorSize !=
            EXPECTED_SECTOR_SIZE ||
        sectorCount <= 0)
    {
        USBSerial.println(
            "Unsupported SD geometry.");

        USB.begin();
        return;
    }

    MSC.vendorID("ESP32");
    MSC.productID("Waveshare SD");
    MSC.productRevision("1.0");

    MSC.onRead(mscRead);
    MSC.onWrite(mscWrite);
    MSC.onStartStop(mscStartStop);

    if (!MSC.begin(
            sectorCount,
            sectorSize))
    {
        USBSerial.println(
            "USB MSC initialization failed.");

        USB.begin();
        return;
    }

    MSC.mediaPresent(true);

    USB.begin();

    delay(1500);

    USBSerial.println();
    USBSerial.println("============================");
    USBSerial.println(" SD USB Mass Storage Test");
    USBSerial.println("============================");

    USBSerial.printf(
        "Sector size : %d\n",
        sectorSize);

    USBSerial.printf(
        "Sectors     : %d\n",
        sectorCount);

    USBSerial.println(
        "SD card exposed to Windows.");
}

void loop()
{
    if (g_ejected)
    {
        USBSerial.println(
            "SAFE TO UNPLUG / REBOOT");

        g_ejected = false;
    }

    delay(1000);
}