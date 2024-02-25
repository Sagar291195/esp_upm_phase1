
#ifndef ESP_UPM_H
#define ESP_UPM_H

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h" 
#include "esp_system.h"

#include "peripherals/ds3231.h"
#include "peripherals/ina3221.h"
#include "peripherals/ws2812.h"
#include "peripherals/bmp280.h"
#include "peripherals/sdp32.h"
#include "peripherals/bme680.h"



/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/


/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
typedef enum{
    STATE_IDLE,
    STATE_START_TEST,
    STATE_BME680_TEST,
    STATE_BME280_TEST,
    STATE_SDP32_TEST,
    STATE_LED_TEST,
    STATE_STOP_TEST
}test_state_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/



/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
void sensorReadTask(void *pvParameter);
void set_device_state( test_state_t state );
test_state_t get_device_state(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESP_UPM_H*/


