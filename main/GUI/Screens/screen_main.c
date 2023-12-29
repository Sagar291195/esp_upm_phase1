
#include "screen_includes.h"
#include "screen_password.h"
#include "screen_fw_error.h"

void Init_Screen(void)
{
    // screen_set_time_date();
    // Screen_Password();
    // vTestScreen1();
    // cScrTransitCheck();
    if( get_device_working_mode() == NORMAL_MODE )
        Screen_Password(SCR_PASSWORD);
    else if( get_device_working_mode() == FIRMWARE_UPDATE_ERROR )     
    {
        screen_firmware_error();
    }
    // pxDashboardScreen();
    // CallScreenInfo();
    // metroCodeScreen();
    // CallMetroFlowCalibrationScreen();
    // callMetroFlowAdjustScreen();
    // callMetroFlowParameterScreen();
    // callMetroFlowSettingScreen();
    // CallMetroMenuScreen();
    // callMetroAdjust();
    // ppxParameterScreen();
    // xsPresetScreenAdvance();
    // callServiceSetScreen();
    // xseSummaryEndScreen();
    // sssSummarySampleScreen();
    // xssSummaryStartScreen();
    // xCallArchvScreen();
    // callMetroHumiditySettingScreen();
    // callMetroPressureSettingScreen();
    // callMetroTempSettingScreen();
}
