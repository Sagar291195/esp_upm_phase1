/**
 * @file lv_demo_widgets.h
 *
 */

#ifndef LV_DEMO_WIDGETS_H
#define LV_DEMO_WIDGETS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
//#include "C:\esp\esp-idf\myPrograms\lvgl_esp_codes\lv_port_esp32_old\components\lv_examples\lv_examples\src\lv_demo_widgets\screen_code.h"

#include "../ESPUPM_SIM_01/GUI/Screens/screen_includes.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_demo_widgets(void);


//=================================
void testTouch(void);
void vClearBTN(void);
//oid vTickMark(void);
void vPasswordLine(void);
void vPassword_scr1(void);
void vCallCodeScreen(void);



void vCallTask(void); 
void task_cb(lv_task_t *t);


//=====================================

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
