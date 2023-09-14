/**
 * @file sequenceManagement.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the header file for the sequence management component.Sequence management component is responsible for managing the sequence of the sampless.
 * Conatins the results of the sequence which has been run
 * @version 1.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __SEQUENCEMANAGEMENT_H__
#define __SEQUENCEMANAGEMENT_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <counters.h>

/***************************************defines****************************************/
/* maximum number of samples in the system */
#define MAXIMUM_NO_OF_SAMPLES 20

/******************************variables***************************/
/* the structue of the particular sequnce  to be tested */
typedef struct sequence
{
    uint8_t uSequenceNo;    //sequence number
    char cStartDate[60];    //start Date
    uint8_t uStartHour;     //start hour
    uint8_t uStartMin;      //start Minutes
    float fFlowSetPoint;    //flow set point from gui
    uint8_t uDurationHour;  //duration Hour
    uint8_t uDurationMinutes;   //duration minutes
    bool bSucessfullyRun;       //whether the given sample has been sucessfully completed or not
    char cStartPerson[40];      //the person who started the sample
} sequence_t;

/*  this structure holds the summary of the sequence after the results */
typedef struct xGenericSummary
{
    char cStartDate[40];
    char cStopDate[40];
    volumeCounter_t xVolumeCounter;
    hourCounter_t xHourCounter;
} xGenericSummary_t;

/* structure to store summary of sequence */
typedef struct sequenceSummary
{   
    xGenericSummary_t summary;
    airflowVolumetric_t airflowVolumetric;
    ambientTemperature_t ambientTemperature;
    ambientPressure_t ambientPressure;
    ambientHumidity_t ambientHumidity;
    headLoss_t headLoss;
} sequenceSummary_t;

/*********************************prototype declaration**************************************/

/**
 * @brief initialize the sample array to zero
 */
void vInitializeSequenceArray();

/**
 * @brief Set the given sample data into the sample array
 * @param uSequenceNumber sequence number
 * @param cStartDate start date of the sample sequence
 * @param uStartHour start hour of sample
 * @param uStartMin start minutes of sample to be taken
 * @param fFlowSetPoint  setted flow rate
 * @param uDurationHour  duration hour of the sample
 * @param uDurationMinutes duration minutes of the sample
 * @param cStartPerson person who started the sample
 */
void vSetSequenceValues(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson);

/**
 * @brief return the address of the sample array
 * @return sequence_t* address of the sample array
 */
sequence_t *pGetAddressOfSequenceArray();

/**
 * @brief save the sequence array to nvs
 */
void vSetSequenceArrayToNVS();

/**
 * @brief retrive the samples from the nvs and stores into the array
 */
void vGetSequenceFromNvsToArray();

/**
 * @brief return the number of Sequence in the array or in the given sample
 * @return uint8_t no of samples in the array
 */
uint8_t uGetNoOfSequenceInArray();

/**
 * @brief Get the values of the particular smaple from the smaple array. It will return the address of the particular sample
 * @param uSequenceNumber sample numberto get
 * @return sequence_t* whole sample data
 */
sequence_t *pGetSequenceFromArray(uint8_t uSequenceNumber);

/**
 * @brief Get the waiting  time in sec to in starting the sequence
 * @param uSequenceNumber delay of the sequnce number to be caluclated
 * @return unsighed long dealy in sec
 */
int32_t uGetNumberOfSecondRemainingToStartSequence(uint8_t uSequenceNumber);

/**
 * @brief set the sequence to be started in a task.
 * @param uSequenceNumber Sequence to be started
 */
void vSetSequenceToRun(uint8_t *uSequenceNumber);


/**
 * @brief Get the sequence number to be saved. This fucntion gives the sequcne number to be show on the gui which is free or not written now 
 * @return uint8_t free sequence number
 */
uint8_t uGetSequenceNumberToBeSaved();


/**
 * @brief All in one function to get the variable from the nvs flash into the memory 
 */
void vGetSequceManagementFromNVS();

/**
 * @brief set the total sequence count to the nvs flash
 */
void vSetTotalSequenceCountFromNvs();

/**
 * @brief Get the Nth sequence from the array 
 * @param xNthSequece nth sequence to be returned
 * @param uSequenceNumber Sequence number  to be returned
 */
void vGetNthSaequenceFromArray(sequence_t* xNthSequece, uint8_t uSequenceNumber);

/**
 * @brief Stops the onging sequence. Current sequnce will be interrupted either by the person or  when the sequnce is over
 */
void vStopCurrentSequence();

/**
 * @brief will stop the running sequence. Use this funtion from the front end to stop the sequence.
 */
void vStopTheRunningSequnence();


/**
 * @brief check if sequence management in progress
 * @return true 
 * @return false 
 */
bool bIsSequenceRunning();

#endif // __SEQUENCEMANAGEMENT_H__