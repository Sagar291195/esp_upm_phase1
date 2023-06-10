

/**
 * @file screen_includes.h
 *
 */

//SCREEN_INCLUDES_H
//SCREEN_INCLUDES_H


#ifndef SCREEN_INCLUDES_H
#define SCREEN_INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../../../../lvgl/lvgl.h"

#include "screen_code.h"
#include "screen_main.h"
#include "abhai_test1.h" 
#include "screen_test.h"
#include "screen_common.h"
#include "screen_dashboard.h"
#include "screen_parameter.h"
#include "screen_preset.h"
#include "screen_summary_start.h"
#include "screen_summary_end.h"
#include "screen_summary_sample.h"
#include "screen_metro_code.h"
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
#include "screen_test_esptft.h"
#include "screen_names.h"
#include "screen_metro_pswd.h"
#include "screen_WTPrgTran.h"
#include "screen_PIDtune.h"
#include "screen_archive.h"

#include "../Widgets/ResumeInfoWidget.h"
//#include "../Widgets/InfoWidget.h"
#include "../Widgets/sequence_widget.h"
//#include "../../Language/language.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "esp_upm_sensors.h"
#include "middleware.h"

#include "espupm_tasks.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
enum{
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
}metrology_screen_t;


extern uint8_t screenid ;
/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_INCLUDES_H*/
