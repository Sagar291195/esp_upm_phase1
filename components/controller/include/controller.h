/**
 * @file controller.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This hs the header file for the controller api. 
 * @version 1.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <counters.h>
#include <sampleManagement.h>
#include <sequenceManagement.h>
#include <timeManagement.h>
#include <dataMangement.h>
#include <sensorManagement.h>




/**
 * @brief This function will take the parameters from the view and set to the motor pid
 * 
 * @param fKp kp value
 * @param fKi ki value
 * @param fKd kd value
 * @param fAkp Aggresive kp value
 * @param fAki Aggresive ki value
 * @param fAkd Aggresive kd value
 * @param fNcoff normal cofficient
 * @param fACoff Aggresive cofficient
 */
void vSetPIDParameters(float fKp, float fKi, float fKd,float fAkp,float fAki,float fAkd,float fNcoff,float fACoff);

/**
 * @brief set the sample data to the sample array. This is just the middle function for the sample values
 * 
 * @param uSequenceNumber 
 * @param cStartDate 
 * @param uStartHour 
 * @param uStartMin 
 * @param fFlowSetPoint 
 * @param uDurationHour 
 * @param uDurationMinutes 
 * @param cStartPerson 
 */
void vSaveSampleValues(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson);


/**
 * @brief initiaialize the sample array
 * 
 */
void vControllerInitializeSampleArray();

/**
 * @brief this function is intended to use when the user interrupt (10 sec ) time has been passed out and sample is valid and ready for start
 * 
 */
void vControllerSampleIsValid();

/**
 * @brief showing the sequence wait screen.
 * 
 */
void vShowWaitInProgressScreen();


/**
 * @brief shows the work in progress screen
 * 
 */
void vShowWorkInProgressScreen();

/**
 * @brief update the work in progress screen with the latest value
 * 
 */
void vUpdateWorkInProgressScreen();

/**
 * @brief Stops the ongoing sequence
 * 
 */
void vControllerSampleStop();

/**
 * @brief show show when the job is finished and the user want to see the data
 * 
 */
void vShowJobFinishedScreen();


/**
 * @brief All in one function to stop the current sequence. Call this function from gsm/wifi/mqtt/Gui etc
 * 
 */
void vControllerSampleStop();

/**
 * @brief shows the end summary screen
 * 
 */
void vControllerShowEndSummayScreen();

#ifdef __cplusplus
}
#endif
#endif // __CONTROLLER_H__