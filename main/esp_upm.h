
#ifndef ESP_UPM_H
#define ESP_UPM_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "external/ds3231.h"
#include "sensors/bmp280.h"
#include "sensors/sdp32.h"
#include "external/ina3221.h"

#include "middleware.h"
#include "PID.h"

#include "espupm_tasks.h"
#include "external/buzzer.h"
#include "external/ws2812.h"
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

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESP_UPM_H*/


