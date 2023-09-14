

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      Year
 */

/**
*  @file file
*  @brief Short description
*  @details Long description
*/

#ifndef MIDVARIABLE_H
#define MIDVARIABLE_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

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
 *  EXTERN VARIABLES
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
//extern double total_liters;

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

#endif /*MIDVARIABLE_H*/


