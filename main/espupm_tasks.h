#ifndef ESPUPM_TASKS_H
#define ESPUPM_TASKS_H

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
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "../lvgl/lvgl.h"
#include "../lvgl_esp32_drivers/lvgl_helpers.h"

#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_timer.h" 
#include "esp_upm.h"
#include "GUI/Screens/screen_includes.h"


/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
typedef struct
{
    bool bme280;
    bool bme680;
    bool ds3231;
    bool ina3231;
    bool mpu6050;
    bool sdp32;
    bool buzzer;
} sensor_present_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern char today_Date_Msg[200];
extern char guiDate[40];
extern bool buzzer_on;

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
void ds3231_task(void *pvParameters);
void buzzer_task(void *pvParamters);
void ws2812_task(void *pvParamters);
void set_navier_time_flag(bool value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESPUPM_TASKS_H*/
