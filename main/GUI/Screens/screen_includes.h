#ifndef SCREEN_INCLUDES_H
#define SCREEN_INCLUDES_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 #include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../../lvgl/lvgl.h"

#include "screen_main.h"
#include "screen_common.h"
#include "screen_dashboard.h"
#include "screen_parameter.h"
#include "screen_preset.h"
#include "screen_summary_start.h"
#include "screen_summary_end.h"
#include "screen_summary_sample.h"
#include "screen_metro_menu.h"
#include "screen_metro_adjust.h"
#include "screen_metro_flowadj.h"
#include "screen_metro_flowcal.h"
#include "screen_metro_flowsetting.h"
#include "screen_metro_tempsetting.h"
#include "screen_metro_pressuresetting.h"
#include "screen_metro_humiditysetting.h"
#include "screen_metro_flowparameter.h"
#include "screen_info.h"
#include "screen_service.h"
#include "screen_password.h"
#include "screen_PIDtune.h"
#include "screen_archive.h"

#include "../Widgets/ResumeInfoWidget.h"
#include "../Widgets/sequence_widget.h"

#include "esp_upm.h"
#include "espupm_tasks.h"

 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define NUM_OF_FLOW_CALIBRATION_POINT   3
/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
enum
{
    SCR_PASSWORD,
    SCR_DASHBOARD,
    SCR_METROLOGY_PASSWORD,
    SCR_CHANGE_PASSWORD,
    SCR_METROLOGY_MENU,
    SCR_EXTERNAL_TEMPERATURE_SETTINGS,
    SCR_EXTERNAL_TEMPERATURE_ADJUST,
    SCR_EXTERNAL_PRESSURE_SETTINGS,
    SCR_EXTERNAL_PRESSURE_ADJUST,
    SCR_EXTERNAL_HUMIDITY_SETTINGS,
    SCR_EXTERNAL_HUMIDITY_ADJUST,
    SCR_INTERNAL_TEMPERATURE_SETTINGS,
    SCR_INTERNAL_TEMPERATURE_ADJUST,
    SCR_INTERNAL_PRESSURE_SETTINGS,
    SCR_INTERNAL_PRESSURE_ADJUST,
    SCR_INTERNAL_HUMIDITY_SETTINGS,
    SCR_INTERNAL_HUMIDITY_ADJUST,
    SCR_FLOW_SETTINGS,
    SCR_FLOW_ADJUST,
    SCR_FLOW_CALIBRATION,
    SCR_FLOW_PARAMETER,
    SCR_ARCHIEVE,
    SCR_INFO,
    SCR_PARAMETER,
    SCR_PIDTUNE,
    SCR_PRESET,
    SCR_SERVICE,
    SCR_SUMMARY_START,
    SCR_SUMMARY_END,
    SCR_SUMMARY_SAMPLE,
} metrology_screen_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern uint8_t screenid;
/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_INCLUDES_H*/
