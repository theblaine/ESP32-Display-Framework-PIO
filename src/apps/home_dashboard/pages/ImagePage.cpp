#include "ImagePage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "PNG_Image.h"

namespace ImagePage
{
    void draw()
    {
        Display_FillScreen(
            Color::Black);

        Display_DrawHeaderBar(
            "Image",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        const bool imageLoaded =
            PNGImage_Draw(
                "/Sunset_140x140.png",
                16,
                48);

        if (imageLoaded)
        {
            LOG("Image page PNG loaded.");
        }
        else
        {
            LOGE("Image page PNG failed to load.");
        }
    }
}