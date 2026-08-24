#include "image_demo.h"

#include "Logger.h"
#include "PNG_Image.h"
#include "SD_Card.h"

namespace
{
    constexpr uint16_t MAX_IMAGES = 100;

    uint16_t g_imageCount = 0;
    uint16_t g_currentImage = 0;

    char g_sdImageNames[MAX_IMAGES][100];
    char g_fileImageNames[MAX_IMAGES][100];
}

void Search_Image(
    const char *directory,
    const char *fileExtension)
{
    g_imageCount =
        Folder_retrieval(
            directory,
            fileExtension,
            g_sdImageNames,
            MAX_IMAGES);

    if (g_imageCount == 0)
    {
        return;
    }

    for (uint16_t i = 0;
         i < g_imageCount;
         i++)
    {
        strcpy(
            g_fileImageNames[i],
            g_sdImageNames[i]);

        remove_file_extension(
            g_fileImageNames[i]);
    }
}

void Display_Image(
    const char *directory,
    const char *fileExtension,
    uint16_t id)
{
    Search_Image(
        directory,
        fileExtension);

    if (g_imageCount == 0)
    {
        LOGEF(
            "No '%s' files found in '%s'.",
            fileExtension,
            directory);

        return;
    }

    if (id >= g_imageCount)
    {
        id = 0;
    }

    String filePath;

    if (String(directory) == "/")
    {
        filePath =
            String(directory) +
            g_sdImageNames[id];
    }
    else
    {
        filePath =
            String(directory) +
            "/" +
            g_sdImageNames[id];
    }

    LOGF(
        "Displaying PNG: %s",
        filePath.c_str());

    if (!PNGImage_Draw(
            filePath.c_str(),
            0,
            0))
    {
        LOGEF(
            "Failed to display PNG: %s",
            filePath.c_str());
    }
}

void Image_Next_Loop(
    const char *directory,
    const char *fileExtension,
    uint32_t nextTime)
{
    static uint32_t nextTimeNow = 0;

    nextTimeNow++;

    if (nextTimeNow < nextTime)
    {
        return;
    }

    nextTimeNow = 0;

    Search_Image(
        directory,
        fileExtension);

    if (g_imageCount == 0)
    {
        LOGEF(
            "No '%s' files found in '%s'.",
            fileExtension,
            directory);

        return;
    }

    g_currentImage++;

    if (g_currentImage >=
        g_imageCount)
    {
        g_currentImage = 0;
    }

    Display_Image(
        directory,
        fileExtension,
        g_currentImage);
}