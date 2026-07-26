#pragma once

/**
 * Available pages in the Display_Widgets demo.
 *
 * Add one enum entry and one matching draw-function declaration whenever
 * a new widget demo page is created.
 */
enum class WidgetDemoPage
{
    PanelsAndLabels,
    ProgressBars,
    Values,
    LabelValues,
    StatusIndicators,
    HeaderFooterBars,
    IconLabels,
    Batteries,
    Tables
};

// Demo page functions.
void DrawPanelsAndLabelsDemo();
void DrawProgressBarsDemo();
void DrawValuesDemo();
void DrawLabelValuesDemo();
void DrawStatusIndicatorsDemo();
void DrawHeaderFooterBarsDemo();
void DrawIconLabelsDemo();
void DrawBatteriesDemo();
void DrawTablesDemo();
