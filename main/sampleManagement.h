
/**
 * @file sampleManagement.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the header file for the sample management. The sample management contain the sequence,their data points, their results and their status.
 * @version 1.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SAMPLEMANAGEMENT_H__
#define __SAMPLEMANAGEMENT_H__

#include <stdint.h>
#include <stdbool.h>
#include <sequenceManagement.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

/***********************************************************defines************************************/

/************************************struct enum variables********************************************/

 /* sample header structure */
typedef struct sampleHeader
{   
    float fTotalLitersStart;     //sample start total number of liters 
    float fToatlHoursStart;      //samples hour start 
    char cStartPerson[40];       //sample start person 
} sampleHeader_t;

/* sample footer structure  */
typedef struct sampleFooter
{   
    float ftotalLitersEnd;  //sample end total number of liters
    float ftoatlHoursEnd;   //sample end hour
    char cEndPerson[40];
} sampleFooter_t;

/* structure holds the  end sample summary */
typedef struct xSampleSummary
{   
    xGenericSummary_t xGenericSummary;  //generic summary
    char cStartTime[10];    //sample start time
    char cStopTime[10];     //sample stop time     
    uint16_t uSampleNumber; //sample number to which this summary belongs
    float fFlowSetPoint;    //flow set point 
    uint8_t uSequenceNumber;    //sequence number of the sample
    char cDuration[10];     //duration of sample in 15H 30M
    char cStartPerson[40];  //start person of the sample
    char cEndPerson[40];    // end person of the sample
    char hasProblem[5];     //check whether the sample has problem or not.It has only 2 values yes or no 
}xSampleSummary_t;



/***********************************************function prototypes*********************************/

/**
 * @brief Get the current sample number in the system
 * @return uint32_t current sample number
 */
uint32_t uGetCurrentSampleNumber();

/**
 * @brief Set the sample number in the system
 * @param uSampleNumber sample number to be setted
 */
void vSetCurrentSampleNumber(uint32_t uSampleNumber);

/**
 * @brief Increment the sample number
 */
void vIncrementCurrentSampleNumber();

/**
 * @brief save the sample number to the nvs flash
 */
void vSetSampleNumberToNvsFlash();

/**
 * @brief get the current sequence number which is either running or will be in delay and will run next time
 * @return uint8_t Sequnece number of the sequnce 
 */
uint8_t uGetCurrentRunningSequenceNumber();

/**
 * @brief Get the task handle for the sample management service
 * @return TaskHandle_t 
 */
TaskHandle_t xGetHandleSampleManagementService();

/**
 * @brief this fuction start the sample management service
 */
void vStartSampleManagementService();


/**
 * @brief set the current running sequence number
 * @param uSequenceNumber sequnce no to be setted
 */
void vSetCurrentRunningSequenceNumber(uint8_t uSequenceNumber);

/**
 * @brief save the end summary to the nvs flash 
 */
void vSaveEndSummaryToNvsFlash();

/**
 * @brief Get the end summary from nvs flash
 * 
 */
void vGetEndSummaryFromNvsFlash();


/**
 * @brief checks that the sample runs sucessfully or not. There are some criterias which decides that the sequence runs sucessfully or not. If all the sequnce in the sample runs sucessfully then the sample consider as the sucessful sample or runs without any problem.
 * @return true if no problem detected in the sample
 * @return false if the given sample is not successful
 */
bool bIsSampleRunsWithoutProblem();

/**
 * @brief calculate the target volume in the sample
 * @return float target volume counter
 */
float fGetTargetVolumeCount();

/**
 * @brief Get the total number of targetd hour in the sample
 * @return float target hour counter
 */
float fGetTargetHourCount();

/**
 * @brief Initialize the end summary structure according to the sample gather
 */
void vSetInitialCounterValuesToEndSummary();

/**
 * @brief fetch the end summary variable from the back end.
 * @param xSampleSummary variable which hold the end summary
 */
void vGetEndSummaryVariable(xSampleSummary_t *xSampleSummary);

/**
 * @brief This function will indicate the sample management to move further. Sample management need the indication either to proceed in the new sample or indicated by the sequnce management regarding the completion of the sequence. 
 */
void vNotifySampleMangementToProceed();

/**
 * @brief all in one function to stop the sample management service from any where any time
 * 
 */
void vStopCurrentSample();

/**
 * @brief this function calculates the values of summarry which we get at the end of the sample
 * Various values updated are:
 * 1. Stop date
 * 2. Stop time
 * 3. sample problem
 * 4. stop liters
 * 5. effective stop lites
 * 6. variation in stop liters
 * 7. stop hours
 * 8. effective stop hours
 * 9. variation in stop hours
 */
void vSetCounterValuesEndSummaryDetails();


#endif // __SAMPLEMANAGEMENT_H__