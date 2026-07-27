#include <Arduino.h>

#include "Logger.h"
#include "Display.h"
#include "Buttons.h"

#include "WidgetDemoPages.h"

// =====================================================================
// STARTUP DEMO PAGE
// =====================================================================
// Choose the demo shown after power-up or reset.

// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::PanelsAndLabels;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::ProgressBars;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::Values;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::LabelValues;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::StatusIndicators;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::HeaderFooterBars;
constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::IconLabels;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::Batteries;
// constexpr WidgetDemoPage StartupDemoPage = WidgetDemoPage::Tables;

constexpr uint8_t StartupBrightness = 100;
constexpr uint8_t BrightnessLevels[] = {100, 75, 50, 25};
constexpr size_t BrightnessLevelCount =
    sizeof(BrightnessLevels) / sizeof(BrightnessLevels[0]);

WidgetDemoPage CurrentDemoPage = StartupDemoPage;
size_t CurrentBrightnessLevel = 0;

// =====================================================================
// BRIGHTNESS HELPERS
// =====================================================================

void ApplyNextBrightnessLevel()
{
    CurrentBrightnessLevel =
        (CurrentBrightnessLevel + 1) % BrightnessLevelCount;

    const uint8_t brightness = BrightnessLevels[CurrentBrightnessLevel];
    Display::setBrightness(brightness);

    switch (brightness)
    {
        case 100:
            LOG("Brightness: 100%.");
            break;
        case 75:
            LOG("Brightness: 75%.");
            break;
        case 50:
            LOG("Brightness: 50%.");
            break;
        case 25:
            LOG("Brightness: 25%.");
            break;
    }
}

// =====================================================================
// DEMO PAGE HELPERS
// =====================================================================

void DrawCurrentDemo()
{
    switch (CurrentDemoPage)
    {
        case WidgetDemoPage::PanelsAndLabels:
            LOG("Drawing panels and labels demo.");
            DrawPanelsAndLabelsDemo();
            break;

        case WidgetDemoPage::ProgressBars:
            LOG("Drawing progress bars demo.");
            DrawProgressBarsDemo();
            break;

        case WidgetDemoPage::Values:
            LOG("Drawing values demo.");
            DrawValuesDemo();
            break;

        case WidgetDemoPage::LabelValues:
            LOG("Drawing label and value demo.");
            DrawLabelValuesDemo();
            break;

        case WidgetDemoPage::StatusIndicators:
            LOG("Drawing status indicators demo.");
            DrawStatusIndicatorsDemo();
            break;

        case WidgetDemoPage::HeaderFooterBars:
            LOG("Drawing header and footer bars demo.");
            DrawHeaderFooterBarsDemo();
            break;

        case WidgetDemoPage::IconLabels:
            LOG("Drawing icon and label demo.");
            DrawIconLabelsDemo();
            break;

        case WidgetDemoPage::Batteries:
            LOG("Drawing battery widgets demo.");
            DrawBatteriesDemo();
            break;

        case WidgetDemoPage::Tables:
            LOG("Drawing table widget demo.");
            DrawTablesDemo();
            break;
    }
}

WidgetDemoPage NextDemoPage(WidgetDemoPage page)
{
    switch (page)
    {
        case WidgetDemoPage::PanelsAndLabels:
            return WidgetDemoPage::ProgressBars;

        case WidgetDemoPage::ProgressBars:
            return WidgetDemoPage::Values;

        case WidgetDemoPage::Values:
            return WidgetDemoPage::LabelValues;

        case WidgetDemoPage::LabelValues:
            return WidgetDemoPage::StatusIndicators;

        case WidgetDemoPage::StatusIndicators:
            return WidgetDemoPage::HeaderFooterBars;

        case WidgetDemoPage::HeaderFooterBars:
            return WidgetDemoPage::IconLabels;

        case WidgetDemoPage::IconLabels:
            return WidgetDemoPage::Batteries;

        case WidgetDemoPage::Batteries:
            return WidgetDemoPage::Tables;

        case WidgetDemoPage::Tables:
            return WidgetDemoPage::PanelsAndLabels;
    }

    return StartupDemoPage;
}

// =====================================================================
// ARDUINO SETUP AND LOOP
// =====================================================================

void setup()
{
    delay(1000);

    Logger::begin();

    LOG("=====================================");
    LOG(" ESP32-S3 Display Widgets Demo");
    LOG("=====================================");

    LOG("Initializing display.");
    Display::begin();
    Display::setBrightness(StartupBrightness);
    LOG("Brightness: 100%.");

    LOG("Initializing BOOT button.");
    Buttons_Begin();

    CurrentDemoPage = StartupDemoPage;
    DrawCurrentDemo();

    LOG("Short press: next demo.");
    LOG("Long press: startup demo.");
    LOG("Hold 3 seconds: cycle brightness.");
}

void loop()
{
    Buttons_Update();

    if (Button_WasVeryLongPressed(Button::Boot))
    {
        LOG("BOOT very long press: cycling brightness.");
        ApplyNextBrightnessLevel();
    }
    else if (Button_WasLongPressed(Button::Boot))
    {
        LOG("BOOT long press: returning to startup demo.");
        CurrentDemoPage = StartupDemoPage;
        DrawCurrentDemo();
    }
    else if (Button_WasPressed(Button::Boot))
    {
        LOG("BOOT short press: advancing to next demo.");
        CurrentDemoPage = NextDemoPage(CurrentDemoPage);
        DrawCurrentDemo();
    }

    delay(1);
}
