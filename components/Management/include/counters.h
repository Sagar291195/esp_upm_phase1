

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

/**
 * @brief maximum variation in tolerance for the hour measurement to mark the sample as sucessful
 * 
 */
#define MAX_TOLERATION_IN_HOUR_VARIATION 5.0

/**
 * @brief maximum variation in tolerance for volume measurement to mark as sucessful
 * 
 */
#define MAX_TOLERATION_IN_VOLUME_VARIATION 5.0

/**
 * @brief maximum tolerance in variation for flow of air to mark the sample as sucessful
 * 
 */
#define MAX_TOLERATION_IN_FLOW_VARIATION 5.0

/***************************************struct and enum *****************************************/

/**
 * @brief this structure stores the volume of air flow values of the sequence summary in the system
 *
 */
typedef struct volumeCounter
{

    /**
     * @brief total start volume of the system
     *
     */
    float fStartVolume;
    /**
     * @brief total end volume of the system
     *
     */
    float fEndVolume;

    /**
     * @brief target volume to be achived
     *
     */
    float fTargetVolume;

    /**
     * @brief Volume has been acheived by running the sequence
     *
     */
    float fEffectiveVolume;

    /**
     * @brief variation in percentage
     *
     */
    float fVariation;

} volumeCounter_t;

/**
 * @brief summary of the hour counter
 *
 */
typedef struct hourCounter
{
    /**
     * @brief total start hour of the sequence
     *
     */
    float fStartHour;
    /**
     * @brief total end hour of the sequence
     *
     */
    float fEndHour;
    /**
     * @brief target hour to be achived
     *
     */
    float fTargetHour;
    /**
     * @brief hour has been acheived by running the sequence
     *
     */
    float fEffectiveHour;
    /**
     * @brief variation in percentage
     *
     */
    float fVariation;

} hourCounter_t;

/**
 * @brief summary of the air flow counter
 *
 */
typedef struct airflowVolumetric
{

    /**
     * @brief air flow set point for the sequecne
     *
     */
    float fAirflowSetPoint;

    /**
     * @brief mean air flow for the sequence during the running
     *
     */
    float fAirflowMean;

    /**
     * @brief max variation in the air flow for the sequence during the running
     *
     */
    float fAirflowMaxValue;

    /**
     * @brief min variation in the air flow for the sequence during the running
     *
     */
    float fAirflowMinValue;

    /**
     * @brief percentage of variation in the air flow for the sequence during the running
     *
     */
    float fAirflowVariation;

    /**
     * @brief check whether the values are in the range or not
     *
     */
    bool bIsInRange;

} airflowVolumetric_t;

typedef struct ambientTemperature
{

    /**
     * @brief measn ambient temperature set point for the sequence
     *
     */
    float fMeanTemperature;

    /**
     * @brief max variation in the ambient temperature for the sequence during the running
     *
     */
    float fMaxTemperature;

    /**
     * @brief min variation in the ambient temperature for the sequence during the running
     *
     */
    float fMinTemperature;

    /**
     * @brief percentage of variation in the ambient temperature for the sequence during the running
     *
     */
    float fTemperatureVariation;

    /**
     * @brief check whether the values are in the range or not
     *
     */
    bool bIsInRange;

} ambientTemperature_t;

/**
 * @brief structure for holding the ambient pressure
 *
 */
typedef struct ambientPressure
{

    /**
     * @brief mean ambient pressure
     *
     */
    float fMeanAmbientPressure;

    /**
     * @brief max Ambient pressure
     *
     */
    float fMaxAmbientPressure;

    /**
     * @brief min ambient pressure
     *
     */
    float fMinAmbientPressure;

    /**
     * @brief variation in pressure
     *
     */
    float fAmbientPressureVariation;

    /**
     * @brief check whether the values are in the range or not
     *
     */
    bool bIsInRange;

} ambientPressure_t;

/**
 * @brief structure to hold the ambient humididy
 *
 */
typedef struct ambientHumidity
{ /**
   * @brief mean ambient humidity
   *
   */
    float fMeanAmbientHumidity;

    /**
     * @brief max ambient Humidity
     *
     */
    float fMaxAmbientHumidity;

    /**
     * @brief min ambient humbidity
     *
     */
    float fMinAmbientHumidity;

    /**
     * @brief ambient humidity variation
     *
     */
    float fAmbientHumidityVariation;

    /**
     * @brief check whether the values are in the range or not
     *
     */
    bool bIsInRange;

} ambientHumidity_t;

/**
 * @brief structure to hold the headloss
 *
 */
typedef struct headLoss
{
    /**
     * @brief mean head loss
     *
     */
    float fMeanHeadLoss;

    /**
     * @brief max head loss
     *
     */
    float fMaxHeadLoss;

    /**
     * @brief minimum head loss
     *
     */
    float fMinHeadLoss;

    /**
     * @brief headloss vairation
     *
     */

    float fHeadLossVariation;

    /**
     * @brief check whether the values are in the range or not
     *
     */
    bool bIsInRange;

} headLoss_t;

/**
 * @brief cotainer applicable for both sequece summary and sample summary
 *
 */

/*********************function prototypes**********************/

/**
 * @brief get the total liter count
 *
 * @return float
 */
float fGetTotalLiterCount();

/**
 * @brief set the total liter count value
 *
 * @param uTotalLiters
 */
void vSetTotalLiterCount(float uTotalLiters);

/**
 * @brief get the total hours count
 *
 * @return float
 */
float fGetTotalHoursCount();

/**
 * @brief set the total hours count value
 *
 * @param uTotalHours
 */
void vSetTotalHoursCount(float fTotalHours);

/**
 * @brief Gett the total liter integral part. Suppose value is 100.22, then it will return 100
 *
 * @return uint32_t
 */
uint32_t uGetTotalLiterIntegerPart();

/**
 * @brief Get the mantissa part of the total liter count. Suppose value is 100.22, then it will return 0.22. Presion is 2 decimal places
 *
 * @return uint8_t
 */
uint8_t uGetTotalLiterFloatPart();

/**
 * @brief Get the total hours integral part. Suppose value is 100.22, then it will return 100
 *
 * @return uint32_t
 */
uint32_t uGetTotalHoursIntegerPart();

/**
 * @brief Get the mantissa part of the total hours count. Suppose value is 100.22, then it will return 0.22. Presion is 2 decimal placess
 *
 * @return uint8_t
 */
uint8_t uGetTotalHoursFloatPart();

/**
 * @brief set the total hours to the nvs flash
 *
 */
void vSetTotalHoursValueToNvs();

/**
 * @brief set the total liter values into the flash
 *
 */
void vSetTotalLitersValueToNvs();

/**
 * @brief get the various counter values from the nvs flash
 *
 */
void vGetTheCounterValuesFromNvsFlash();

/**
 * @brief get the percentage of the job done
 *
 * @return float job in percentage
 */
float fGetPercentageOfJobDone();

/**
 * @brief Set the percentage of the job done
 *
 * @param fPercentage job done in percentage
 */
void fSetPercentageOfJobDone(float fPercentage);

/**
 * @brief Get the total number of seconds passed in the sequence
 *
 * @return uint64_t total number os seconds passed in the sequence
 */
uint64_t uGetTotalSecondPassesInGivenSequence();

/**
 * @brief Set the number of seconds passed in the sequence
 *
 * @param uSecondPasses no of seconds
 */
void vSetTotalSecondPassesInGivenSequence(uint64_t uSecondPasses);

/**
 * @brief Increment the number of seonds passed in the sequence by one
 *
 */
void vIncrementTotalSecondPassesInGivenSequence();

/**
 * @brief Set the total volume in the liters passed in the sequence
 *
 * @param fLiters total volume in liters
 */
void vSetTotalLitersHasBeenPassInGivenSequence(float fLiters);

/**
 * @brief Get the total volume in the liters passed in the sequence
 *
 * @return float volume in liters
 */
float fGetTotalLitersHasBeenPassInGivenSequence();

#endif // __COUNTERS_H__