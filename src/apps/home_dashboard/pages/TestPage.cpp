#include "TestPage.h"

#include "Display.h"
#include "Display_GFX.h"
#include "Display_Widgets.h"
#include "Logger.h"
#include "RGB_lamp.h"

namespace
{
    struct TestData
    {
        String text1 = "Test One";
        String text2 = "Test Two";
        String text3 = "Test Three";
        String text4 = "Test Four";
        String text5 = "Test Five";
        String text6 = "Test Six";
    };

    TestData g_testData;
}

namespace TestPage
{
    void draw()
    {
        const Display_TableRow rows[] =
            {
                {"Test 1",
                 g_testData.text1.c_str(),
                 Color::Red},
                {"Test 2",
                 g_testData.text2.c_str(),
                 Color::Cyan},
                {"Test 3",
                 g_testData.text3.c_str(),
                 Color::Yellow},
                {"Test 4",
                 g_testData.text4.c_str(),
                 Color::White},
                {"Test 5",
                 g_testData.text5.c_str(),
                 Color::Green},
                {"Test 6",
                 g_testData.text6.c_str(),
                 Color::Green}
            };

        Display_FillScreen(Color::Black);

        Display_DrawHeaderBar(
            "Test Page",
            Color::Blue,
            Color::White,
            Color::White,
            2,
            34);

        Display_DrawTable(
            8,
            48,
            Display::width() - 16,
            40,
            rows,
            sizeof(rows) / sizeof(rows[0]),
            Color::Black,
            Color::White,
            Color::White,
            Color::Cyan,
            1);

        RGBLamp::setColor(
            64,
            0,
            64);

        LOG("Displayed Test page.");
    }
}