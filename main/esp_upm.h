
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
#include "peripherals/buzzer.h"
#include "peripherals/ws2812.h"
#include "peripherals/motor.h"
#include "peripherals/bmp280.h"
#include "peripherals/sdp32.h"
#include "peripherals/bme680.h"

#include "pid.h"
#include "espupm_tasks.h"
#include "storage/flash.h"
#include "storage/parameters.h"
#include "storage/calibration.h"

#include "sampleManagement.h"
#include "sequenceManagement.h"
#include "timeManagement.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define FIRMWARE_VERSION    "1.4.0.0"

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
 void set_fw_update_errorcode( uint8_t errorcode );
 void set_device_normal_operating_mode( void );
 uint8_t get_firmware_update_error( void );
 uint8_t get_device_working_mode( void );
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESP_UPM_H*/


