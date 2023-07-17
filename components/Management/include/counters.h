

/**
 * @file counters.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This files contain the definition of the counters. Counters are the values which are unique in nature globally
 * @version 1.1
 * @date 2022-05-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __COUNTERS_H__
#define __COUNTERS_H__

#include <stdint.h>
#include <stdbool.h>


/**********************************************defines*******************************************/

/* maximum variation in tolerance for the hour measurement to mark the sample as sucessful */
#define MAX_TOLERATION_IN_HOUR_VARIATION 5.0
/* maximum variation in tolerance for volume measurement to mark as sucessful */
#define MAX_TOLERATION_IN_VOLUME_VARIATION 5.0
/* maximum tolerance in variation for flow of air to mark the sample as sucessful */
#define MAX_TOLERATION_IN_FLOW_VARIATION 5.0

/***************************************struct and enum *****************************************/

/* structure to store the volume of air flow values of the sequence summary in the system */
typedef struct volumeCounter
{
    float fStartVolume;     //total start volume of the system
    float fEndVolume;       //total end volume of the system
    float fTargetVolume;    //target volume to be achived
    float fEffectiveVolume; // Volume has been acheived by running the sequence
    float fVariation;   //variation in percentage
} volumeCounter_t;


 /* summary of the hour counter */
typedef struct hourCounter
{
    float fStartHour;   //total start hour of the sequence
    float fEndHour;     //total end hour of the sequence
    float fTargetHour;     // target hour to be achived
    float fEffectiveHour;   //hour has been acheived by running the sequence
    float fVariation;   //variation in percentage

} hourCounter_t;

/* summary of the air flow counter */
typedef struct airflowVolumetric
{
    float fAirflowSetPoint;     //air flow set point for the sequecne 
    float fAirflowMean;         //mean air flow for the sequence during the running 
    float fAirflowMaxValue;     //max variation in the air flow for the sequence during the running
    float fAirflowMinValue;     //min variation in the air flow for the sequence during the running
    float fAirflowVariation;    //percentage of variation in the air flow for the sequence during the running
    bool bIsInRange;        //check whether the values are in the range or not
} airflowVolumetric_t;

typedef struct ambientTemperature
{
    float fMeanTemperature; //measn ambient temperature set point for the sequence
    float fMaxTemperature;  //max variation in the ambient temperature for the sequence during the running
    float fMinTemperature;  //min variation in the ambient temperature for the sequence during the running
    float fTemperatureVariation;    //percentage of variation in the ambient temperature for the sequence during the running
bool bIsInRange;            //check whether the values are in the range or not
} ambientTemperature_t;

/* structure for holding the ambient pressure */
typedef struct ambientPressure
{
    float fMeanAmbientPressure; //mean ambient pressure
    float fMaxAmbientPressure;  //max Ambient pressure
    float fMinAmbientPressure;  //min ambient pressure
    float fAmbientPressureVariation;    //variation in pressure
    bool bIsInRange;    //check whether the values are in the range or not

} ambientPressure_t;

/* structure to hold the ambient humididy */
typedef struct ambientHumidity
{
    float fMeanAmbientHumidity;     //mean ambient humidity
    float fMaxAmbientHumidity;      //max ambient Humidity
    float fMinAmbientHumidity;  // min ambient humbidity
    float fAmbientHumidityVariation;    //ambient humidity variation
    bool bIsInRange;     check whether the values are in the range or not
} ambientHumidity_t;

/**
 * @brief structure to hold the headloss
 *
 */
typedef struct headLoss
{
    float fMeanHeadLoss;    // mean head loss
    float fMaxHeadLoss; // max head loss
    float fMinHeadLoss; //minimum head loss
    float fHeadLossVariation;   //headloss vairation
    bool bIsInRange;    //check whether the values are in the range or not
} headLoss_t;

/**
 * @brief cotainer applicable for both sequece summary and sample summary
 *
 */

/*********************function prototypes**********************/
/**
 * @brief get the total liter count
 * @return float
 */
float fGetTotalLiterCount();

/**
 * @brief set the total liter count value
 * @param uTotalLiters
 */
void vSetTotalLiterCount(float uTotalLiters);

/**
 * @brief get the total hours count
 * @return float
 */
float fGetTotalHoursCount();

/**
 * @brief set the total hours count value
 * @param uTotalHours
 */
void vSetTotalHoursCount(float fTotalHours);

/**
 * @brief Gett the total liter integral part. Suppose value is 100.22, then it will return 100
 * @return uint32_t
 */
uint32_t uGetTotalLiterIntegerPart();

/**
 * @brief Get the mantissa part of the total liter count. Suppose value is 100.22, then it will return 0.22. Presion is 2 decimal places
 * @return uint8_t
 */
uint8_t uGetTotalLiterFloatPart();

/**
 * @brief Get the total hours integral part. Suppose value is 100.22, then it will return 100
 * @return uint32_t
 */
uint32_t uGetTotalHoursIntegerPart();

/**
 * @brief Get the mantissa part of the total hours count. Suppose value is 100.22, then it will return 0.22. Presion is 2 decimal placess
 * @return uint8_t
 */
uint8_t uGetTotalHoursFloatPart();

/**
 * @brief set the total hours to the nvs flash
 */
void vSetTotalHoursValueToNvs();

/**
 * @brief set the total liter values into the flash
 */
void vSetTotalLitersValueToNvs();

/**
 * @brief get the various counter values from the nvs flash
 */
void vGetTheCounterValuesFromNvsFlash();

/**
 * @brief get the percentage of the job done
 * @return float job in percentage
 */
float fGetPercentageOfJobDone();

/**
 * @brief Set the percentage of the job done
 * @param fPercentage job done in percentage
 */
void fSetPercentageOfJobDone(float fPercentage);

/**
 * @brief Get the total number of seconds passed in the sequence
 * @return uint64_t total number os seconds passed in the sequence
 */
uint64_t uGetTotalSecondPassesInGivenSequence();

/**
 * @brief Set the number of seconds passed in the sequence
 * @param uSecondPasses no of seconds
 */
void vSetTotalSecondPassesInGivenSequence(uint64_t uSecondPasses);

/**
 * @brief Increment the number of seonds passed in the sequence by one
 */
void vIncrementTotalSecondPassesInGivenSequence();

/**
 * @brief Set the total volume in the liters passed in the sequence
 * @param fLiters total volume in liters
 */
void vSetTotalLitersHasBeenPassInGivenSequence(float fLiters);

/**
 * @brief Get the total volume in the liters passed in the sequence
 * @return float volume in liters
 */
float fGetTotalLitersHasBeenPassInGivenSequence();

#endif // __COUNTERS_H__