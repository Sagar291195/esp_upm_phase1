
#ifndef ESP_UPM_H
#define ESP_UPM_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "external/ds3231.h"
#include "external/ina3221.h"
#include "external/buzzer.h"
#include "external/ws2812.h"
#include "sensors/bmp280.h"
#include "sensors/sdp32.h"


#include "pid.h"
#include "espupm_tasks.h"
#include "middlewareTest.h"
#include "midFunctions.h"
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
extern int flag_WorkInProg;    // Work In Progress
extern int flag_Ready;         // Ready
extern int flag_WaitInProg;    // Wait In Progress
extern int flag_JobFinished;   // Job Finished
extern int flag_SerAlert;      // Alert Service
extern int flag_MetroNeeded;   // Merology Needed
extern int flag_exportData;    // Export Data
extern int flag_MetroInProg;   // Metrology In Progress
extern bool isMotor; // Flag to Motor On Off Satatus ( flase = OFF, true = ON)

extern double total_liters;

extern int sampleNumber;
extern int HourCount;

extern double StartLTRCountVal;
extern double StopLTRCountVal;

extern double targetLiters;
extern double effectiveLiters;
extern float variationLiters;

extern float targetHr;
extern float effectiveHr;
extern float variationHr;

extern float StartHrEND;
extern float StopHrEND;

extern float totalhourVal;
extern long totalHourInt;
extern int totalHourFloat;

extern float KpVal;
extern float KiVal;
extern float KdVal;

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


