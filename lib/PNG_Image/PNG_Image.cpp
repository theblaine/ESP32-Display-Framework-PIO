#include "PNG_Image.h"

#include <PNGdec.h>

#include "SD_Card.h"
#include "Display_ST7789.h"
#include "Logger.h"

namespace
{
    constexpr int16_t MAX_IMAGE_WIDTH = 172;

    PNG g_png;
    File g_imageFile;

    int16_t g_imageX = 0;
    int16_t g_imageY = 0;

    uint16_t g_lineBuffer[MAX_IMAGE_WIDTH];

    void *pngOpen(
        const char *filePath,
        int32_t *size)
    {
        g_imageFile =
            SD_MMC.open(
                filePath,
                FILE_READ);

        if (!g_imageFile)
        {
            return nullptr;
        }

        *size =
            g_imageFile.size();

        return &g_imageFile;
    }

    void pngClose(
        void *handle)
    {
        (void)handle;

        if (g_imageFile)
        {
            g_imageFile.close();
        }
    }

    int32_t pngRead(
        PNGFILE *page,
        uint8_t *buffer,
        int32_t length)
    {
        (void)page;

        if (!g_imageFile)
        {
            return 0;
        }

        return g_imageFile.read(
            buffer,
            length);
    }

    int32_t pngSeek(
        PNGFILE *page,
        int32_t position)
    {
        (void)page;

        if (!g_imageFile)
        {
            return 0;
        }

        return g_imageFile.seek(
            position);
    }

    void pngDraw(
        PNGDRAW *draw)
    {
        if (draw == nullptr ||
            draw->iWidth >
                MAX_IMAGE_WIDTH)
        {
            return;
        }

        g_png.getLineAsRGB565(
            draw,
            g_lineBuffer,
            PNG_RGB565_BIG_ENDIAN,
            0xffffffff);

        for (int16_t index = 0;
             index < draw->iWidth;
             index++)
        {
            g_lineBuffer[index] =
                ((g_lineBuffer[index] >> 8) &
                 0x00FF) |
                ((g_lineBuffer[index] << 8) &
                 0xFF00);
        }

        const int16_t lineY =
            g_imageY +
            draw->y;

        LCD_AddWindow(
            g_imageX,
            lineY,
            g_imageX + draw->iWidth,
            lineY,
            g_lineBuffer);
    }
}

bool PNGImage_Draw(
    const char *filePath,
    int16_t x,
    int16_t y)
{
    if (filePath == nullptr ||
        !SD_IsMounted())
    {
        return false;
    }

    g_imageX = x;
    g_imageY = y;

    const int16_t result =
        g_png.open(
            filePath,
            pngOpen,
            pngClose,
            pngRead,
            pngSeek,
            pngDraw);

    if (result !=
        PNG_SUCCESS)
    {
        LOGEF(
            "PNG open failed: %s",
            filePath);

        return false;
    }

    LOGF(
        "PNG: %s | %dx%d",
        filePath,
        g_png.getWidth(),
        g_png.getHeight());

    if (g_png.getWidth() >
        MAX_IMAGE_WIDTH)
    {
        LOGE(
            "PNG exceeds maximum image width.");

        g_png.close();

        return false;
    }

    const uint32_t startTime =
        millis();

    const int16_t decodeResult =
        g_png.decode(
            nullptr,
            0);

    g_png.close();

    if (decodeResult !=
        PNG_SUCCESS)
    {
        LOGEF(
            "PNG decode failed: %s",
            filePath);

        return false;
    }

    LOGF(
        "PNG decode time: %lu ms",
        millis() - startTime);

    return true;
}