

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Name (mail or git)
 *  @date      Year
 */

/**
 *  @file file
 *  @brief Short description
 *  @details Long description
 */

#ifndef ESPUPM_TASKS_H
#define ESPUPM_TASKS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/

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



    /*********************
     *      DEFINES
     *********************/

    /**********************
     *     TYPEDEFS
     **********************/

    /**********************
     *  GLOBAL MACROS
     **********************/

    /**********************
     *  GLOBAL VARIABLES
     **********************/
    extern bool metroFlowCalStarted;
    extern int _fasDutyCycle;
    extern float flowPoint;
    extern int workProgress;
    extern volatile int Job_Rem_Hr;
    extern volatile int Job_Rem_Min;
    extern float TotalLit;
    extern int literMeasureNum;
    extern int TotalliterInt;
    extern int TotalLiterFloat;
    extern char today_Date_Msg[200];
    extern char guiDate[40];
    extern double total_liters1;
    extern bool navier_set_time;
    extern float totalhourVal1;
    extern float flow_value;
    extern double *totalliterPtr;
    extern float *totalhourPtr;

    extern int HourCount;
    extern bool buzzer_on;
    extern uint8_t cr, cg, cb;

    extern float kp, ki, kd;
    


    typedef struct log_guiDate_t
    {
        char QguiDate[10];
    } log_guiDate_t;

    typedef struct log_guiTime_t
    {
        char QguiTime[60];
    } log_guiTime_t;

    typedef struct log_SDPsensor_t
    {
        float dp_ticks_SDP;
        float out_SDP;
        int16_t temperature_ticks_SDP;
        float flow_value_SDP;
        float polyFlow;
    } log_SDPsensor_t;

    typedef struct log_BME280sensor_t
    {
        float Qpressure_BME;
        float Qtemp_BME;
        float Qhumidity_BME;
    } log_BME280sensor_t;

    typedef struct log_IntBME280sensor_t
    {
        float Qpressure_IntBME;
        float Qtemp_IntBME;
        float Qhumidity_IntBME;
    } log_IntBME280sensor_t;

    typedef struct log_INAsensor_t
    {
        float qC1BusVoltPtr;
        float qC1ShuntVoltPtr;
        float qC1ShuntCurrentPtr;

        float qC2BusVoltPtr;
        float qC2ShuntVoltPtr;
        float qC2ShuntCurrentPtr;

        float qC3BusVoltPtr;
        float qC3ShuntVoltPtr;
        float qC3ShuntCurrentPtr;

    } log_INAsensor_t;

    typedef struct
    {
        float       bme280_temp;
        float       bme280_humid;
        float       bme280_press;
        float       bme680_temp;    
        float       bme680_press;       
        float       bme680_humid;       
        float       bme680_gas_resist; 
        uint16_t    sdp32_diff_press;
        uint16_t    sdp32_temp;
        uint16_t    motor_rpm;
    } sensor_data_t;

    // if sensor connect - true
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

    extern esp_timer_handle_t JTCesp_timer_handle; // JTC = Job Time Counter


    /**********************
     *  GLOBAL PROTOTYPES
     **********************/
    void iLEDActive(void);
    void iLEDDeActive(void);

    void ds3231_task(void *pvParameters);
    void bme280_task(void *pvParamters);
    void InternalBME280_task(void *pvParamters);
    void sdp32_task(void *pvParamters);
    void ina3221_task(void *pvParameters);
    void pump_task(void *pvParamters);
    void ResInfoPerCount_task(void *pvParamters);
    void buzzer_task(void *pvParamters);
    void ws2812_task(void *pvParamters);
    void infoWgtUpdtWaitToProgTask_cb(lv_task_t *infoWgtUpdtWaitToProgTask);
    void vinfoWgtUpdtWaitToProgTask(void); // Task Handled by LV Task

    void setSensorTasksTOzeroPriority(void);





    void modSDP32_task(void *pvParameters);


    /**********************
     *    ERROR ASSERT
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESPUPM_TASKS_H*/
