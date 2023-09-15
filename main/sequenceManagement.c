/**
 * @file sequenceManagement.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file consist the sequence management implementation
 * @version 1.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sequenceManagement.h>
#include <string.h>
#include <nvs.h>
#include <esp_log.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <timeManagement.h>


#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <math.h>
#include <sensorManagement.h>
#include <userCompensationLayer.h>
#include <esp_timer.h>

#include "esp_upm.h"
/***********************************************************defines************************************/

#define TAG "Sequnce_Management"
#define NVS_STORGE_NAME "sequence"

#define STORAGE_KEY "arrStrg"
#define TOTAL_SEQUENCE_COUNT_KEY "ttlSeqCnt"

/**
 * @brief value of seconds in an hour (1/3600)
 *
 */
#define EACH_SEC_INTO_HOURS 0.000277778

/*********************************************fucntion prototypes************************************/

/**
 * @brief This task have the following tasks to perform
 * 1. calulate the total hours passed
 * 2. saving the total hours and total liters to nvs storage
 * 3. calculate the percentage of the sequce completed.
 * 4. update the total liters / total hours / and percentage to the screen
 *
 * @param pvParameters
 */
static void vUpdateScreenAndSaveValuesEverySecond(void *pvParameters);

/**
 * @brief This funtion perform
 * 1. set the total liter countes at starting
 * 2. set the target liter count
 * 3. set the total hours countes at starting
 * 4. set the target hours count
 *
 */
static void vInitiateSequenceSummaryStart();

/**
 * @brief calulates and put the end summary of the sequence to the sequnece summary structure
 *
 */
static void vCalculateSequneceEndSummary();

/**
 * @brief This task will monitor analyse the data coming from the sensor and check that is it within the range of the target value
 * @param pvParameters
 */
static void vMonitorSensorDataTask(void *pvParameters);

/**
 * @brief Calulate variation in the when we need to calculate variation when max and min value are given, with respect to the target value
 * @param fMaxValue  Maximum deviation from the target value in the upper range
 * @param fMinValue  minimum deviation from the target value in the lower range
 * @param fTargetValue target value to be achived
 * @return float percent of the maximum deviation
 */
static float fCalculateVariationInPercentage(float fMaxValue, float fMinValue, float fTargetValue);

/*********************************variables********************************************/
extern char guiTime[25];
/* variable to store the sequence summary data */
sequenceSummary_t xCurrentSequenceSummary;
/* This semaphore will stop the ongoing sequence. This will be used for the force stop the running sequnence  */
static SemaphoreHandle_t xStopTheRunningSequenceSemaphore = NULL;
/* semaphore for the gui handle  */
extern SemaphoreHandle_t xGuiSemaphore1;
/* this is the dash board flag to set the mode of the dashboard in the gui, need to set before calling the DashboardInfoWidget() to set the mode. */
extern int dashboardflg;
/* this will store all the samples in the system */
static sequence_t totalSequence[MAXIMUM_NO_OF_SAMPLES];
/*  total number of samples in the system */
uint8_t uTotalSequenceCount = 0;
static TaskHandle_t xTaskHandleUpdateScreenAndSaveValuesEverySecond = NULL;
/* this counts the number of seconds passed since the start of the sequence */
uint16_t uTotalTimePassInSeconds = 0;
/* task handle which contols the sequence task */
static TaskHandle_t xTaskHandleRunSequnceTask = NULL;
/* it stores is it safe to delete the task or not */
bool bDeleteUpdateScreenAndVaulesTask = false;
static TaskHandle_t xTaskHandleMonitorSensorData = NULL;

/*********************************functions***********************************************/

void vTimerUpdateTimePassInSec(TimerHandle_t xTimer)
{
    uTotalTimePassInSeconds++;
}

static void print_on_terminal()
{
    ESP_LOGD(TAG, "----------------------------------------------------------------------------");
    ESP_LOGD(TAG, "Sequence number %d", totalSequence[uTotalSequenceCount].uSequenceNo);
    ESP_LOGD(TAG, "Start date %s", totalSequence[uTotalSequenceCount].cStartDate);
    ESP_LOGD(TAG, "start hour %d", totalSequence[uTotalSequenceCount].uStartHour);
    ESP_LOGD(TAG, "start minute %d", totalSequence[uTotalSequenceCount].uStartMin);
    ESP_LOGD(TAG, "flowpoint %f", totalSequence[uTotalSequenceCount].fFlowSetPoint);
    ESP_LOGD(TAG, "Duration hour %d", totalSequence[uTotalSequenceCount].uDurationHour);
    ESP_LOGD(TAG, "Duration minute %d", totalSequence[uTotalSequenceCount].uDurationMinutes);
    ESP_LOGD(TAG, "sucess %d", totalSequence[uTotalSequenceCount].bSucessfullyRun);
    ESP_LOGD(TAG, "Start Person %s", totalSequence[uTotalSequenceCount].cStartPerson);
}

void vSetSequenceValues(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson)
{

    totalSequence[uSequenceNumber - 1].uSequenceNo = uSequenceNumber;
    strcpy(&totalSequence[uSequenceNumber - 1].cStartDate, pStartDate);
    totalSequence[uSequenceNumber - 1].uStartHour = uStartHour;
    totalSequence[uSequenceNumber - 1].uStartMin = uStartMin;
    totalSequence[uSequenceNumber - 1].fFlowSetPoint = fFlowSetPoint;
    totalSequence[uSequenceNumber - 1].uDurationHour = uDurationHour;
    totalSequence[uSequenceNumber - 1].uDurationMinutes = uDurationMinutes;
    totalSequence[uSequenceNumber - 1].bSucessfullyRun = false; // setting the sucessfully run state to false
    // memcpy(totalSequence[uSequenceNumber - 1].cStartPerson, cStartPerson, 40);
    strcpy(&totalSequence[uSequenceNumber - 1].cStartPerson, pStartPerson);

    print_on_terminal();
    /* incrementing the total number of samples */
    uTotalSequenceCount++;
}

void vInitializeSequenceArray()
{
    /* initializind the sample arrey to 0 */
    memset(totalSequence, 0, MAXIMUM_NO_OF_SAMPLES * sizeof(sequence_t));
    /* setting the number of samples to 0 */
    uTotalSequenceCount = 0;
}

sequence_t *pGetAddressOfSequenceArray()
{
    return totalSequence;
}

void vSetSequenceArrayToNVS()
{
    (void) nvswrite_value_parameters(NVS_STORGE_NAME, STORAGE_KEY, (void *)totalSequence, sizeof(totalSequence));
}

void vGetSequenceFromNvsToArray()
{
    (void) nvsread_value_parameter(NVS_STORGE_NAME, STORAGE_KEY, (void *)totalSequence);
}

uint8_t uGetNoOfSequenceInArray()
{
    return uTotalSequenceCount;
}

sequence_t *pGetSequenceFromArray(uint8_t uSequenceNumber)
{
    return &totalSequence[uSequenceNumber - 1];
}

void taskRunSample(void *pvParameters)
{
    /* creating semaphore for stopping the task if, the semaphore handle is NULL */
    if (xStopTheRunningSequenceSemaphore == NULL)
    {
        xStopTheRunningSequenceSemaphore = xSemaphoreCreateBinary();
    }
    else
    {
        vSemaphoreDelete(xStopTheRunningSequenceSemaphore);
        xStopTheRunningSequenceSemaphore = xSemaphoreCreateBinary();
    }

    // uint8_t usequenceToRun = *(uint8_t *)pvParameters;
    uint8_t usequenceToRun;
    memcpy(&usequenceToRun, pvParameters, sizeof(uint8_t));
    ESP_LOGI(TAG, "Running sequence %d", usequenceToRun);

    /* starting the monitor data task */
    if (xTaskHandleMonitorSensorData == NULL)
    {
        xTaskCreate(vMonitorSensorDataTask, "monitor data", 8 * 1024, NULL, 5, &xTaskHandleMonitorSensorData);
    }
    else
    {
        ESP_LOGW(TAG, "monitor task handle is not null, it should be null here.Not creating the monitor task");
    }

    /* calculating and saving the end summary of the variables */
    vInitiateSequenceSummaryStart();
    /* setting the delete the update value task to false so that task does not deleted when created */
    bDeleteUpdateScreenAndVaulesTask = false;
    /*  showing the work in progress screen */
    vShowWorkInProgressScreen();
    /* creating the task which updates the screen values and saving the values to the nvs flash */
    xTaskCreate(vUpdateScreenAndSaveValuesEverySecond, "updateScreen", 4 * 1024, &usequenceToRun, 5, &xTaskHandleUpdateScreenAndSaveValuesEverySecond);
    /*  seting the flow set point */
    setMotorPIDSetTargetValue(totalSequence[usequenceToRun - 1].fFlowSetPoint);
    /*  starting the motor so we have some initial value for sdp sensor and calculate flow
     * rate and the pid controller can compute  */
    MotorPWMStart(motorPID_DEFAULT_ENTRY_POINT);
    setStateOfMotor(true); // run the motor
    ESP_LOGD(TAG, "Starting the motor");
    ESP_LOGD(TAG, "get state of motor %d", getIsMotorRunning());
    uint32_t udurationInMs = ((totalSequence[usequenceToRun - 1].uDurationHour * 3600) + (totalSequence[usequenceToRun - 1].uDurationMinutes * 60)) * 1000;
    ESP_LOGI(TAG, "Duration in ms %d", udurationInMs);
    /*  delaying the task so that motor can run this time period */
    // vTaskDelay(pdMS_TO_TICKS(udurationInMs));
    /* this will block the task until the semaphore is given or the sequnce has been run to the whole time */
    if (xSemaphoreTake(xStopTheRunningSequenceSemaphore, (udurationInMs / portTICK_PERIOD_MS)) == pdTRUE)
    {
        ESP_LOGW(TAG, "Sequence has been terminated early, due to the force stop");
    }
    else
    {
        ESP_LOGI(TAG, "Sequence has been terminated due to expiry of time");
    }

    /* now sequence end is reached now stopping the current sequnce and notify the sample management to carry on the task  */
    vStopCurrentSequence();
}

int32_t uGetNumberOfSecondRemainingToStartSequence(uint8_t uSequenceNumber)
{
    uint8_t uStartHour = totalSequence[uSequenceNumber - 1].uStartHour;
    uint8_t uStartMin = totalSequence[uSequenceNumber - 1].uStartMin;
    struct tm timeinfo = {0};

    strptime(&totalSequence[uSequenceNumber - 1].cStartDate, "%Y/%m/%e", &timeinfo);
    timeinfo.tm_hour = uStartHour;
    timeinfo.tm_min = uStartMin;
    timeinfo.tm_sec = 0;
    /* getting the current time of the system */
    struct tm now = {0};
    vGetCurrentDateAndTime(&now);
    // now.tm_year = now.tm_year - 1900;
    // now.tm_year = now.tm_year;

    // now.tm_mon = now.tm_mon;

    // now.tm_mon = now.tm_mon - 1;

    int32_t seconds = difftime(mktime(&timeinfo), mktime(&now));
    ESP_LOGD(TAG, "Seconds remaining is %d", seconds);
    return seconds;
}

void vSetSequenceToRun(uint8_t *uSequenceNumber)
{
    ESP_LOGI(TAG, "setting sequence to run %d", *uSequenceNumber);
    if (*uSequenceNumber == 0)
        return;
    xTaskCreate(taskRunSample, "taskRunSample", 8 * 1024, (void *)uSequenceNumber, 5, &xTaskHandleRunSequnceTask);
}

uint8_t uGetSequenceNumberToBeSaved()
{
    return uTotalSequenceCount + 1;
}

/* @brief get the total sequene count from the nvs flash */
void vGetTotalSequenceCountFromNvs()
{
    (void) nvsread_value_parameter(NVS_STORGE_NAME, TOTAL_SEQUENCE_COUNT_KEY, (void *)&uTotalSequenceCount);
}

/** set the total sequnce count to the nvs flash */
void vSetTotalSequenceCountToNvs()
{
    (void) nvswrite_value_parameters(NVS_STORGE_NAME, TOTAL_SEQUENCE_COUNT_KEY, (void *)&uTotalSequenceCount, sizeof(uTotalSequenceCount));
}

void nvsread_sequence_parameters()
{
    vGetTotalSequenceCountFromNvs();
    vGetSequenceFromNvsToArray();
}

static void vUpdateScreenAndSaveValuesEverySecond(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting the task to update the screen and save the values to the nvs flash");
    uint8_t usequenceToRun;
    memcpy(&usequenceToRun, pvParameters, sizeof(uint8_t));
    ESP_LOGI(TAG, "Running sequence %d", usequenceToRun);

    float fTotalHourCountTemp = 0;
    uint32_t uTotalTimePassInSec = 0;
    // uint16_t uTotalDurationOfSequenceInSec = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint64_t uSequnceDurationInSec = ((totalSequence[usequenceToRun - 1].uDurationHour * 3600) + (totalSequence[usequenceToRun - 1].uDurationMinutes * 60));
    float fPercentageOfJobDone = 0;

    /* setting the total volume and seconds pass in the sequence to zero */
    vSetTotalSecondPassesInGivenSequence(0);
    vSetTotalLitersHasBeenPassInGivenSequence(0);

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); // ensure that the calling frequnency be constant

        /*  When we delete the task using the task delete function, there may be time that task is in middle of something and holding the semaphore. this varialbe ensure that while delting the task, the task
         * does not hold any semaphore. */
        if (!bDeleteUpdateScreenAndVaulesTask)
        {
            uTotalTimePassInSec++;
            /* Setting number of second pass in the sequence */
            vSetTotalSecondPassesInGivenSequence(uTotalTimePassInSec);
            fPercentageOfJobDone = ((float)uTotalTimePassInSec / (float)uSequnceDurationInSec) * 100;
            /* Setting the percent of job done */
            fSetPercentageOfJobDone(fPercentageOfJobDone);
            fTotalHourCountTemp = fGetTotalHoursCount();
            /* adding each second pass to the total hours count */
            fTotalHourCountTemp += EACH_SEC_INTO_HOURS;
            vSetTotalHoursCount(fTotalHourCountTemp);
            /* updating the screen for the total hours and liters */
            vUpdateWorkInProgressScreen();
            vSetTotalHoursValueToNvs();
            vSetTotalLitersValueToNvs();
        }
        else
        {
            ESP_LOGI(TAG, "Deleteing the update values and screen task");
            vTaskDelete(NULL);
        }
    }
}

void vGetNthSaequenceFromArray(sequence_t *xNthSequece, uint8_t uSequenceNumber)
{
    memcpy(xNthSequece, &totalSequence[uSequenceNumber - 1], sizeof(sequence_t));
}

void vStopCurrentSequence()
{
    ESP_LOGI(TAG, "Now stopping the current sequence");
    /* turning off the motor */
    setStateOfMotor(false);

    /* deleting the task which update the values on the screen and saving to the flash */
    bDeleteUpdateScreenAndVaulesTask = true;
    /* stopping the monitor task */
    if (xTaskHandleMonitorSensorData)
    {
        ESP_LOGI(TAG, "Deleting the moniotr task");
        vTaskDelete(xTaskHandleMonitorSensorData);
        xTaskHandleMonitorSensorData = NULL;
    }

    /* calculating the end summary of the sequence */
    vCalculateSequneceEndSummary();
    /* giving the time to stop the ongoing task to finish */
    vTaskDelay(pdMS_TO_TICKS(1500));

    /* Notify the sample task about the completion of the sequence */
    vNotifySampleMangementToProceed();
    vTaskDelay(pdMS_TO_TICKS(50));
    if (xTaskHandleRunSequnceTask != NULL)
    {
        TaskHandle_t xHandle = xTaskHandleRunSequnceTask;
        xTaskHandleRunSequnceTask = NULL;
        ESP_LOGI(TAG, "Deleting the Sequence Task");
        vTaskDelete(xHandle);
    }
}

void vStopTheRunningSequnence()
{
    /* Giving the semaphore for stopping the sequence */
    xSemaphoreGive(xStopTheRunningSequenceSemaphore);
}

bool bIsSequenceRunning()
{
    if (xTaskHandleRunSequnceTask != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void vInitiateSequenceSummaryStart()
{
    memset(&xCurrentSequenceSummary, 0, sizeof(sequenceSummary_t)); /* setting the summary to 0 */
    sequence_t xSequenceNumber;                                     /* variable for the sequence number */

    /* getting the current  sequence from the sequence array */
    vGetNthSaequenceFromArray(&xSequenceNumber, uGetCurrentRunningSequenceNumber());
    strcpy(xCurrentSequenceSummary.summary.cStartDate, xSequenceNumber.cStartDate);
    /* setting the flow set point */
    xCurrentSequenceSummary.airflowVolumetric.fAirflowSetPoint = xSequenceNumber.fFlowSetPoint;
    /* getting the initial total lites before the starting of the sequnece */
    xCurrentSequenceSummary.summary.xVolumeCounter.fStartVolume = fGetTotalLiterCount();
    /* setting the target volume count */
    xCurrentSequenceSummary.summary.xVolumeCounter.fTargetVolume = (xSequenceNumber.fFlowSetPoint * ((xSequenceNumber.uDurationHour * 60) + xSequenceNumber.uDurationMinutes));
    /* setting the starting hour counter */
    xCurrentSequenceSummary.summary.xHourCounter.fStartHour = fGetTotalHoursCount();
    /* setting the target hour count */
    xCurrentSequenceSummary.summary.xHourCounter.fTargetHour = (float)((float)xSequenceNumber.uDurationHour + ((float)xSequenceNumber.uDurationMinutes / 60.00));

    ESP_LOGI(TAG, "Target hour assighned is %f", xCurrentSequenceSummary.summary.xHourCounter.fTargetHour);
    /*setting the in range variables in the summary to be true, if as the sensor values are measured, then we can set to false, if they outside the range */
    xCurrentSequenceSummary.airflowVolumetric.bIsInRange = true;
    xCurrentSequenceSummary.ambientHumidity.bIsInRange = true;
    xCurrentSequenceSummary.ambientPressure.bIsInRange = true;
    xCurrentSequenceSummary.ambientTemperature.bIsInRange = true;
    xCurrentSequenceSummary.headLoss.bIsInRange = true;

    /* end of the starting of sequnece summary */
}

static void vCalculateSequneceEndSummary()
{
    struct tm timeinfo = {0};
    vGetCurrentDateAndTime(&timeinfo);
    char cStopDate[40];

    // sprintf(cStopDate, "%d/%d/%d", timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
    timeinfo.tm_year = timeinfo.tm_year + 1900;
    timeinfo.tm_mon = timeinfo.tm_mon + 1;
    sprintf(cStopDate, "%d/%2d/%2d", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);

    /* copy end date */
    strcpy(xCurrentSequenceSummary.summary.cStopDate, cStopDate);
    /* setting  the end volume and the effective volume */
    xCurrentSequenceSummary.summary.xVolumeCounter.fEndVolume = fGetTotalLiterCount();
    xCurrentSequenceSummary.summary.xVolumeCounter.fEffectiveVolume = fGetTotalLiterCount() - xCurrentSequenceSummary.summary.xVolumeCounter.fStartVolume;
    /* calculating the variation in volume */
    xCurrentSequenceSummary.summary.xVolumeCounter.fVariation = fabs(((xCurrentSequenceSummary.summary.xVolumeCounter.fTargetVolume - xCurrentSequenceSummary.summary.xVolumeCounter.fEffectiveVolume) / xCurrentSequenceSummary.summary.xVolumeCounter.fTargetVolume) * 100);

    xCurrentSequenceSummary.summary.xHourCounter.fEndHour = fGetTotalHoursCount();
    xCurrentSequenceSummary.summary.xHourCounter.fEffectiveHour = fGetTotalHoursCount() - xCurrentSequenceSummary.summary.xHourCounter.fStartHour;

    /* calculating the variation in hours */
    xCurrentSequenceSummary.summary.xHourCounter.fVariation = fabs(((xCurrentSequenceSummary.summary.xHourCounter.fTargetHour - xCurrentSequenceSummary.summary.xHourCounter.fEffectiveHour) / xCurrentSequenceSummary.summary.xHourCounter.fTargetHour) * 100);

    ESP_LOGD(TAG, "Hour counter target and effective values are %.2f and %.2f", xCurrentSequenceSummary.summary.xHourCounter.fTargetHour, xCurrentSequenceSummary.summary.xHourCounter.fEffectiveHour);

    ESP_LOGD(TAG, "variation in hour is %.2f", xCurrentSequenceSummary.summary.xHourCounter.fVariation);

    /* calculating the variation of extenal sensor data and the air flow */
    xCurrentSequenceSummary.airflowVolumetric.fAirflowVariation = fCalculateVariationInPercentage(xCurrentSequenceSummary.airflowVolumetric.fAirflowMaxValue, xCurrentSequenceSummary.airflowVolumetric.fAirflowMinValue, xCurrentSequenceSummary.airflowVolumetric.fAirflowSetPoint);
    xCurrentSequenceSummary.ambientHumidity.fAmbientHumidityVariation = fCalculateVariationInPercentage(xCurrentSequenceSummary.ambientHumidity.fMaxAmbientHumidity, xCurrentSequenceSummary.ambientHumidity.fMinAmbientHumidity, xCurrentSequenceSummary.ambientHumidity.fMeanAmbientHumidity);
    xCurrentSequenceSummary.ambientPressure.fAmbientPressureVariation = fCalculateVariationInPercentage(xCurrentSequenceSummary.ambientPressure.fMaxAmbientPressure, xCurrentSequenceSummary.ambientPressure.fMinAmbientPressure, xCurrentSequenceSummary.ambientPressure.fMeanAmbientPressure);
    xCurrentSequenceSummary.ambientTemperature.fTemperatureVariation = fCalculateVariationInPercentage(xCurrentSequenceSummary.ambientTemperature.fMaxTemperature, xCurrentSequenceSummary.ambientTemperature.fMinTemperature, xCurrentSequenceSummary.ambientTemperature.fTemperatureVariation);
    xCurrentSequenceSummary.headLoss.fHeadLossVariation = fCalculateVariationInPercentage(xCurrentSequenceSummary.headLoss.fMaxHeadLoss, xCurrentSequenceSummary.headLoss.fMinHeadLoss, xCurrentSequenceSummary.headLoss.fMeanHeadLoss);

    /*  If all sensors values are with in range then the sequence is ok. So we can say that the sequence run sucessfully */
    if (xCurrentSequenceSummary.airflowVolumetric.bIsInRange && (xCurrentSequenceSummary.airflowVolumetric.fAirflowVariation < MAX_TOLERATION_IN_FLOW_VARIATION) &&
        xCurrentSequenceSummary.ambientHumidity.bIsInRange && (xCurrentSequenceSummary.ambientHumidity.fAmbientHumidityVariation < MAX_TOLERACE_IN_HUMIDITY) &&
        xCurrentSequenceSummary.ambientPressure.bIsInRange && (xCurrentSequenceSummary.ambientPressure.fAmbientPressureVariation < MAX_TOLERACE_IN_PRESSURE) &&
        xCurrentSequenceSummary.ambientTemperature.bIsInRange && (xCurrentSequenceSummary.ambientTemperature.fTemperatureVariation < MAX_TOLERACE_IN_TEMPERATURE) &&
        xCurrentSequenceSummary.headLoss.bIsInRange && (xCurrentSequenceSummary.headLoss.fHeadLossVariation < MAX_TOLERACE_IN_HEAD_LOSS) &&
        (xCurrentSequenceSummary.summary.xHourCounter.fVariation < MAX_TOLERATION_IN_HOUR_VARIATION) &&
        (xCurrentSequenceSummary.summary.xVolumeCounter.fVariation < MAX_TOLERATION_IN_VOLUME_VARIATION))
    {
        totalSequence[uGetCurrentRunningSequenceNumber() - 1].bSucessfullyRun = true;
    }

    else
    {
        totalSequence[uGetCurrentRunningSequenceNumber() - 1].bSucessfullyRun = false;
    }

    /*  save summary to the db */
    vInsertSequenceSummaryIntoDataBase(uGetCurrentSampleNumber(), uGetCurrentRunningSequenceNumber(), xCurrentSequenceSummary);
}

static void vMonitorSensorDataTask(void *pvParameters)
{
    ESP_LOGI(TAG, "Monitoring the sensor data");
    uint64_t uCounterForCalculatingMeanValues = 0;

    TickType_t last_wakeup = xTaskGetTickCount();
    external_sensor_data_t external_sensor_data;
    external_sensor_data_t raw_sensor_data;
    INA3231_sensor_data_t xInaSensorData[INA3221_CHANNEL];
    bool bOneTime = false;

    while (1)
    {
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(1000));
        uCounterForCalculatingMeanValues++;

        /* getting the extenal sensor data from sensor management */
        vGetExternalSensorDataUserCompensated(&external_sensor_data);
        vGetExternalSensorData(&raw_sensor_data);
        /* getting the INA data */
        vGet_INA3221_sensor_data(&xInaSensorData);

        /* calulating the mean temp, humidity and pressure */
        xCurrentSequenceSummary.ambientTemperature.fMeanTemperature = ((xCurrentSequenceSummary.ambientTemperature.fMeanTemperature * (uCounterForCalculatingMeanValues - 1)) + external_sensor_data.fTemperature) / uCounterForCalculatingMeanValues;
        xCurrentSequenceSummary.ambientHumidity.fMeanAmbientHumidity = (xCurrentSequenceSummary.ambientHumidity.fMeanAmbientHumidity * (uCounterForCalculatingMeanValues - 1) + external_sensor_data.fHumidity) / uCounterForCalculatingMeanValues;
        xCurrentSequenceSummary.ambientPressure.fMeanAmbientPressure = (xCurrentSequenceSummary.ambientPressure.fMeanAmbientPressure * (uCounterForCalculatingMeanValues - 1) + external_sensor_data.fPressure) / uCounterForCalculatingMeanValues;

        /*  setting the min and max values for the external sensor data */
        xCurrentSequenceSummary.ambientTemperature.fMaxTemperature = (xCurrentSequenceSummary.ambientTemperature.fMaxTemperature > external_sensor_data.fTemperature) ? xCurrentSequenceSummary.ambientTemperature.fMaxTemperature : external_sensor_data.fTemperature;
        xCurrentSequenceSummary.ambientTemperature.fMinTemperature = (xCurrentSequenceSummary.ambientTemperature.fMinTemperature < external_sensor_data.fTemperature) ? xCurrentSequenceSummary.ambientTemperature.fMinTemperature : external_sensor_data.fTemperature;
        xCurrentSequenceSummary.ambientHumidity.fMaxAmbientHumidity = (xCurrentSequenceSummary.ambientHumidity.fMaxAmbientHumidity > external_sensor_data.fHumidity) ? xCurrentSequenceSummary.ambientHumidity.fMaxAmbientHumidity : external_sensor_data.fHumidity;
        xCurrentSequenceSummary.ambientHumidity.fMinAmbientHumidity = (xCurrentSequenceSummary.ambientHumidity.fMinAmbientHumidity < external_sensor_data.fHumidity) ? xCurrentSequenceSummary.ambientHumidity.fMinAmbientHumidity : external_sensor_data.fHumidity;
        xCurrentSequenceSummary.ambientPressure.fMaxAmbientPressure = (xCurrentSequenceSummary.ambientPressure.fMaxAmbientPressure > external_sensor_data.fPressure) ? xCurrentSequenceSummary.ambientPressure.fMaxAmbientPressure : external_sensor_data.fPressure;
        xCurrentSequenceSummary.ambientPressure.fMinAmbientPressure = (xCurrentSequenceSummary.ambientPressure.fMinAmbientPressure < external_sensor_data.fPressure) ? xCurrentSequenceSummary.ambientPressure.fMinAmbientPressure : external_sensor_data.fPressure;

        /* checking the external sensor temprature is valid or not */
        if ((external_sensor_data.fTemperature < EXTERNAL_SENSOR_TEMPERATURE_MIN_VALUE) || (external_sensor_data.fTemperature > EXTERNAL_SENSOR_TEMPERATURE_MAX_VALUE))
        {
            /* raise the flag inside the sequence summary to indicate the error */
            xCurrentSequenceSummary.ambientTemperature.bIsInRange = false;
        }

        /* checking if the external sensor humidity is valid or not */
        if ((external_sensor_data.fHumidity < EXTERNAL_SENSOR_HUMIDITY_MIN_VALUE) || (external_sensor_data.fHumidity > EXTERNAL_SENSOR_HUMIDITY_MAX_VALUE))
        {
            /* raise the flag inside the sequence summary to indicate the error */
            xCurrentSequenceSummary.ambientHumidity.bIsInRange = false;
        }

        /* checking if the external sensor pressure is valid or not */
        if ((external_sensor_data.fPressure < EXTERNAL_SENSOR_PRESSURE_MIN_VALUE) || (external_sensor_data.fPressure > EXTERNAL_SENOSR_PRESSURE_MAX_VALUE))
        {
            /* raise the flag inside the sequence summary to indicate the error */
            xCurrentSequenceSummary.ambientPressure.bIsInRange = false;
        }

        uint64_t uConterForAirFlowRate = uCounterForCalculatingMeanValues - 10; // we need to calulate the air flow rate after the 10 seconds or iteration has been passed. All above calculations will be performed for the air flow as well

        if (uConterForAirFlowRate > 0) // after 10 seconds or iteration has been passed
        {
            /* calculating the mean air flow rate */
            xCurrentSequenceSummary.airflowVolumetric.fAirflowMean = ((xCurrentSequenceSummary.airflowVolumetric.fAirflowMean * (uConterForAirFlowRate - 1)) + xCurrentSequenceSummary.airflowVolumetric.fAirflowMean) / uConterForAirFlowRate;

            /* calculating the max and min for the air flow rate */
            xCurrentSequenceSummary.airflowVolumetric.fAirflowMaxValue = (xCurrentSequenceSummary.airflowVolumetric.fAirflowMaxValue > fGetVolumetricFlowUserCompensated()) ? xCurrentSequenceSummary.airflowVolumetric.fAirflowMaxValue : fGetVolumetricFlowUserCompensated();
            xCurrentSequenceSummary.airflowVolumetric.fAirflowMinValue = (xCurrentSequenceSummary.airflowVolumetric.fAirflowMinValue < fGetVolumetricFlowUserCompensated()) ? xCurrentSequenceSummary.airflowVolumetric.fAirflowMinValue : fGetVolumetricFlowUserCompensated();
        }

        if (!bOneTime)
        {
            printf("------------------Settings-----------------------\n");
            printf(" kp is: %0.2f || Ki is: %0.2f || kd is: %0.2f || Akp is :%0.2f || Aki is: %0.2f||Akd is: %0.2f \n", fGetPIDParameterKp(), fGetPIDParameterKi(), fGetPIDParameterKd(), fGetPIDParameterAkp(), fGetPIDParameterAki(), fGetPIDParameterAkd());
            printf("------------------Settings-----------------------\n");
            bOneTime = true;
            //printf(" hardware Time,SDP_temperature,SDP_MassFlow,External_pressure_BME,External_temp_BME,External_humidity_BME,totalliter,TotalHour,C1BusVolt,C1ShuntVolt,C1ShuntCurr,C2BusVolt,C2ShuntVolt,C2ShuntCurr,C3BusVolt,C3ShuntVolt,C3ShuntCurr,RealFlowRate,External AirDensity,internal BME280 temperature,internal BME280 pressure,internal BME280 humidity,internal BME280 Airdensity\n");
        }
        printf("Time: RTC Time: %s, Hardware Time: %llu,\n",  guiTime, esp_timer_get_time());
        printf("SDP: Temperature: %0.2f, Dp : %0.2f Pa, MassFlow : %0.2f STDL,\n", fGetSdp32TemperatuerAverageValue(), fGetSdp32DiffPressureAverageValue(), fGetMassFlowUserCompensated());
        printf("Channel 0: Bus Voltage: %0.2f V, Shunt Voltage: %0.2f mV, Shunt Current: %0.2f mA,\n", xInaSensorData[0].fBusVoltage, xInaSensorData[0].fShuntVoltage, xInaSensorData[0].fShuntCurrent);
        printf("Channel 1: Bus Voltage: %0.2f V, Shunt Voltage: %0.2f mV, Shunt Current: %0.2f mA,\n", xInaSensorData[1].fBusVoltage, xInaSensorData[1].fShuntVoltage, xInaSensorData[1].fShuntCurrent);
        printf("Channel 2: Bus Voltage: %0.2f V, Shunt Voltage: %0.2f mV, Shunt Current: %0.2f mA,\n", xInaSensorData[2].fBusVoltage, xInaSensorData[2].fShuntVoltage, xInaSensorData[2].fShuntCurrent);
        printf("External: Temperature Raw: %0.2f, Humidity Raw: %0.2f %%, Pressure Raw: %0.2f hPa, Air Density Raw: %0.2f\n", raw_sensor_data.fTemperature, raw_sensor_data.fHumidity, raw_sensor_data.fPressure, fGetExternal_AirDesity_Raw());
        printf("Internal: Temperature Raw: %0.2f, Humidity Raw: %0.2f %%, Pressure Raw: %0.2f hPa, Air Density Raw: %0.2f\n", fGetBme280TemperatureAverages(), fGetBme280HumidityAverages(), fGetBme280PressureAverages(),  fGetInternalAirDensity_Raw());
        printf("External: Temperature Comp: %0.2f, Humidity Comp: %0.2f %%, Pressure Comp: %0.2f hPa, Air Density Raw: %0.2f\n", external_sensor_data.fTemperature, external_sensor_data.fHumidity, external_sensor_data.fPressure, fGetExternal_AirDesity_Comp());
        printf("Internal: Temperature Comp: %0.2f, Humidity Comp: %0.2f %%, Pressure Comp: %0.2f hPa, Air Density Raw: %0.2f\n", fGetInternalTemperatureUserCompesated(), fGetInternalHumidityUserCompesated(), fGetInternalPressureUserCompensated(),  fGetInternalAirDensity_Comp());
        printf("Feature: Volumetric Flow Comp: %0.2f LPM", fGetVolumetricFlowUserCompensated());


        // printf("%llu,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n", esp_timer_get_time(), fGetSdp32TemperatuerAverageValue(), fGetSdp32DiffPressureAverageValue(), external_sensor_data.fPressure, external_sensor_data.fTemperature, external_sensor_data.fHumidity, fGetTotalLiterCount(), fGetTotalHoursCount(), xInaSensorData[0].fBusVoltage, xInaSensorData[0].fShuntVoltage, xInaSensorData[0].fShuntCurrent, xInaSensorData[1].fBusVoltage, xInaSensorData[1].fShuntVoltage, xInaSensorData[1].fShuntCurrent, xInaSensorData[2].fBusVoltage, xInaSensorData[2].fShuntVoltage, xInaSensorData[2].fShuntCurrent, fGetVolumetricFlowUserCompensated(), fGetExternal_AirDesity_Raw(), fGetInternalTemperatureUserCompesated(), fGetInternalPressureUserCompensated(), fGetInternalHumidityUserCompesated(), fGetInternalAirDensityUserCompensated());
    }

    vTaskDelete(NULL);
}

static float fCalculateVariationInPercentage(float fMaxValue, float fMinValue, float fTargetValue)
{
    float fVariationInPercentage = 0;
    fVariationInPercentage = fabs((fMaxValue - fMinValue) / (fTargetValue)) * 100;
    return fVariationInPercentage;
}