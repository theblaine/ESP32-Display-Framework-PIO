#include "ImagePage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Logger.h"
#include "PNG_Image.h"

namespace ImagePage
{
    void draw()
    {
        Display_FillScreen(
            Color::Black);

        PNGImage_Draw(
            "/WaveshareImage2.png");

        LOG("Displayed Image page.");
    }
}