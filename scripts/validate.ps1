# ESP32 Display Framework Validation Script
#
# Builds every PlatformIO environment to verify the framework.
#
# Usage:
#     .\scripts\validate.ps1

$ErrorActionPreference = "Stop"

$environments = @(
    "app_home_dashboard",

    "demo_display_graphics",
    "demo_display_text",
    "demo_display_console",
    "demo_display_widgets",
    "demo_factory",
    "demo_png",
    "demo_sd",
    "demo_sd_usb",
    "demo_rgb",
    "demo_wifi",
    "demo_mqtt",
    "demo_ntp",

    "ttgo_display_graphics",
    "ttgo_display_text",
    "ttgo_display_console",
    "ttgo_display_calibration"
)

Write-Host ""
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host " ESP32 Display Framework Validation"
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    Write-Host "PlatformIO CLI (pio) was not found in PATH." -ForegroundColor Red
    Write-Host ""
    Write-Host "Run this from the PlatformIO terminal, or install the PlatformIO CLI."
    exit 1
}

$start = Get-Date

foreach ($env in $environments)
{
    Write-Host ""
    Write-Host "--------------------------------------------------"
    Write-Host "Building: $env" -ForegroundColor Yellow

    & pio run -e $env

    if ($LASTEXITCODE -ne 0)
    {
        Write-Host ""
        Write-Host "FAILED: $env" -ForegroundColor Red
        exit $LASTEXITCODE
    }

    Write-Host "PASS: $env" -ForegroundColor Green
}

$elapsed = (Get-Date) - $start

Write-Host ""
Write-Host "==========================================" -ForegroundColor Green
Write-Host " ALL BUILDS PASSED"
Write-Host " Elapsed: $($elapsed.ToString())"
Write-Host "==========================================" -ForegroundColor Green
