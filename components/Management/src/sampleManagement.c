/**
 * @file sampleManagement.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file consist the sample management implementation
 * @version 1.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */

/************************************************Includes**********************************************/
#include <sampleManagement.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <time.h>
#include <controller.h>
#include <string.h>
#include <math.h>
/************************************************defines**********************************************/

#define TAG "sampleManagement"
#define NVS_STORGE_NAME "storage"
#define SAMPLE_STORAGE_KEY "smplStrg"
#define SEQUENCE_STORAGE_KEY "seqStrg"
#define END_SUMMARY_STORAGE_KEY "endStrg"

/**
 * @brief used in the case for the deep sleep configuration
 *
 */
#define MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS (30)

/***************************************************variables*******************************************/

/**
 * @brief This is the unique sample number in the system. Since sample
 *
 */
static uint32_t uUniqueSampleNumber = 0;

/**
 * @brief tells about the current running sequnce.
 *
 */
static uint8_t uCurrentRunningSequenceNumber = 0;

QueueHandle_t xSequenceQueue;

/**
 * @brief task handle for the sample management Service
 *
 */
static TaskHandle_t xHandleSampleManagementService = NULL;

/**
 * @brief this variable stores the end summary for the current sample
 *
 */
static xSampleSummary_t xEndSummary;

/**
 * @brief this variable stores the state of the sample. true signifies that a force stop for the sample has been achieved
 *
 */
bool bSampleForcedStop = false;

/***********************************************function prototypes***************************************/

/**
 * @brief initialize the end summary variable to zero
 *
 */
void vInitializeEndSummaryVariableToZero();

/**
 * @brief save the volume counters, hours counters and other values to the end summary variable.
 *
 */
void vSaveInitialCountersValuesEndSummary();

void vSetCounterValuesEndSummaryDetails();

/********************************************functions*******************************************************/

uint32_t uGetCurrentSampleNumber()
{
    return uUniqueSampleNumber;
}

void vSetCurrentSampleNumber(uint32_t uSampleNumber)
{
    uUniqueSampleNumber = uSampleNumber;
}

void vIncrementCurrentSampleNumber()
{
    uUniqueSampleNumber++;
}

void vSetSampleNumberToNvsFlash()
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_set_u32(nvsHandle, SAMPLE_STORAGE_KEY, uUniqueSampleNumber);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        }
        else
        {
            err = nvs_commit(nvsHandle);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
            }
        }
        nvs_close(nvsHandle);
    }
}

void vGetSampleNumberFromNvsFlash()
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_get_u32(nvsHandle, SAMPLE_STORAGE_KEY, &uUniqueSampleNumber);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s\n", esp_err_to_name(err), __func__);
        }
        nvs_close(nvsHandle);
    }
}

uint8_t uGetCurrentRunningSequenceNumber()
{
    return uCurrentRunningSequenceNumber;
}

void vSetCurrentRunningSequenceNumber(uint8_t uSequenceNumber)
{
    uCurrentRunningSequenceNumber = uSequenceNumber;
}

void vSetCurrentSequenceNumberToNvsFlash()
{
    nvs_handle nvsHandle;

    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {

        err = nvs_set_u8(nvsHandle, SEQUENCE_STORAGE_KEY, uCurrentRunningSequenceNumber);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        }
        else

        {

            err = nvs_commit(nvsHandle);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
            }
        }

        nvs_close(nvsHandle);
    }
}

void vGetCurrentSequenceNumberFromNvsFlash()
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_get_u8(nvsHandle, SEQUENCE_STORAGE_KEY, &uCurrentRunningSequenceNumber);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s\n", esp_err_to_name(err), __func__);
        }
        ESP_LOGD(TAG, "Current saved sample number in nvs flash is %d", uCurrentRunningSequenceNumber);
        nvs_close(nvsHandle);
    }
}

static void vSampleManagementServiceFunction(void *pvParamaters)
{

    esp_log_level_set(TAG, ESP_LOG_DEBUG);

    ESP_LOGD(TAG, "Sample Management Service Started");

    bSampleForcedStop = false;

    /**
     * @brief Getting the current sample number from nvs flash
     *
     */
    vGetSampleNumberFromNvsFlash();

    /**
     * @brief gettting the current running sequence number from nvs flash
     *
     */
    vGetCurrentSequenceNumberFromNvsFlash();

    /**
     * @brief saving the current system state to calculate the end summary
     *
     */
    vInitializeEndSummaryVariableToZero();

    /**
     * @brief Getting the values from the nvs flash
     *
     */
    vGetEndSummaryFromNvsFlash();

    int32_t ulrequiredDelay = 0;

    /**
     * @brief if the system restarted or wake up from sleep then the system will start from the currenct sequence to be run. To do this first cheeck if the any sequence is pending to be run.
     *
     */
    if (uCurrentRunningSequenceNumber != 0)
    {
        /**
         * @brief give the notification to start the sample sequence
         *
         */
        xTaskNotifyGive(xHandleSampleManagementService);
    }
    while (1)
    { /**
       * @brief waitingg for the signal to start the sequences
       *
       */
        ESP_LOGD(TAG, "Waiting for the new sequnce to run");
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(TAG, "Starting the sequence");

        /**
         * @brief iterating over the sequences.valid sequence are those which are less thatn the number of the sequnces and also the invalid sequnce number 0
         *
         */
        while ((uCurrentRunningSequenceNumber <= uGetNoOfSequenceInArray()) && (uCurrentRunningSequenceNumber != 0))
        {

            ESP_LOGD(TAG, "Starting sequence %d/%d", uCurrentRunningSequenceNumber, uGetNoOfSequenceInArray());

            /**
             * @brief showing the wait in progress screen. wait sreen for the 1st sequene has been set from the front end. this need to be changed in the phase 2
             *
             */
            if ((uCurrentRunningSequenceNumber != 0) && (uCurrentRunningSequenceNumber != 1))
            {
                ESP_LOGD(TAG, "showing the waitn in progress screeen");
                vShowWaitInProgressScreen(); // first sample wait screen has been shown by the gui itself
            }
            // vShowWaitInProgressScreen();

            /**
             * @brief getting the delay time for the current sequence
             *
             */
            ulrequiredDelay = uGetNumberOfSecondRemainingToStartSequence(uCurrentRunningSequenceNumber);
            // below functionallity will be implemented later
            // if (ulrequiredDelay>MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS)
            // {
            //     /**
            //      * @brief putting the device into the deep sleep mode for time ulrequiredDelay - MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS
            //      *
            //      */
            //     ESP_LOGD(TAG, "Going to sleep for %lu seconds", ulrequiredDelay - MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS);
            // }

            if (ulrequiredDelay >= 0)
            {
                vTaskDelay(pdMS_TO_TICKS(ulrequiredDelay * 1000));
            }

            else
            {
                ESP_LOGE(TAG, "Error: Delay time is less than 0 means that time has already passed, but can force start the sequence now");
                // uCurrentRunningSequenceNumber++;
                // continue;
            }

            ESP_LOGD(TAG, "Starting the sequence %d/%d", uCurrentRunningSequenceNumber, uGetNoOfSequenceInArray());
            /**
             * @brief created the sequence to run
             *
             */

            vSetSequenceToRun(&uCurrentRunningSequenceNumber);

            /**
             * @brief waiting for the sequence to finish
             *
             */
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            ESP_LOGD(TAG, "Task notification has been received");
            if (uCurrentRunningSequenceNumber == 0)
            {
                break;
            }
            ESP_LOGD(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber);
            /**
             * @brief show the summary screen, when all the sequneces are finished or we forced stop the samples by pressing the stop button
             *
             */
            if ((uCurrentRunningSequenceNumber == uGetNoOfSequenceInArray()) || bSampleForcedStop == true)
            {

                /**
                 * @brief filling the end summary, when not force stopped
                 *
                 */
                vSetCounterValuesEndSummaryDetails();

                // if (!bSampleForcedStop)
                {
                    // vSetCounterValuesEndSummaryDetails();
                }
                /**
                 * @brief now need to save the end summary
                 *
                 */

                ESP_LOGD(TAG, "end of the Sample reached");
                /**
                 * @brief our job is finished, need to show the finised job summary screen only when the sample is time end
                 *
                 */
                if (!bSampleForcedStop)
                {
                    vShowJobFinishedScreen();
                }
                /**
                 * @brief if the sequence is the last sequence in the array, then the sample is over, not set the sequence to run to 0. indicating that no sequnce is in progress
                 *
                 */
                ESP_LOGD(TAG, "Setting the sequence number to 0");
                vSetCurrentRunningSequenceNumber(0);
                ESP_LOGD(TAG, "Saving the sequence number to nvs flash");
                vSetCurrentSequenceNumberToNvsFlash();
                bSampleForcedStop = false;
                break;
            }
            else
            {
                ESP_LOGD(TAG, "Icrementing the sequence number");
                /**
                 * @brief if the sequence is not the last sequence in the array, then  increment the sequence number and set it to nvs flash
                 *
                 */
                uCurrentRunningSequenceNumber++;

                vSetCurrentSequenceNumberToNvsFlash();
            }
            ESP_LOGI(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber - 1);
        }
        ESP_LOGD(TAG, "END of the Sample task ha been reached, now iterating for the new loop");
    }

    ESP_LOGE(TAG, "Error: Sample Management Service Task is exiting");
    vTaskDelete(NULL);
}

void vStartSampleManagementService()
{

    xTaskCreate(vSampleManagementServiceFunction, "SampleManagementService", 4 * 1028, NULL, 5, &xHandleSampleManagementService);
}

TaskHandle_t xGetHandleSampleManagementService()
{
    return xHandleSampleManagementService;
}

void vInitializeEndSummaryVariableToZero()
{
    memset(&xEndSummary, 0, sizeof(xEndSummary));
}

void vSaveEndSummaryToNvsFlash()
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_set_blob(nvsHandle, END_SUMMARY_STORAGE_KEY, &xEndSummary, sizeof(xEndSummary));
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        }
        else
        {
            err = nvs_commit(nvsHandle);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
            }
        }
        nvs_close(nvsHandle);
    }
}

void vGetEndSummaryFromNvsFlash()
{
    nvs_handle nvsHandle;
    esp_err_t err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        size_t requiredSize = sizeof(xEndSummary);
        err = nvs_get_blob(nvsHandle, END_SUMMARY_STORAGE_KEY, &xEndSummary, &requiredSize);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error (%s) getting NVS value! func %s\n", esp_err_to_name(err), __func__);
        }
        nvs_close(nvsHandle);
    }
}

void vSetInitialCounterValuesToEndSummary()
{

    /**
     * @brief Construct a new v Initialize End Summary Variable To Zero object
     *
     */
    vInitializeEndSummaryVariableToZero();
    sequence_t xSequenceNumber;

    /**
     * @brief getting the 1 sequence from the sequence array
     *
     */
    vGetNthSaequenceFromArray(&xSequenceNumber, 1);

    strcpy(xEndSummary.xGenericSummary.cStartDate, xSequenceNumber.cStartDate);

    /**
     * @brief getting the start time of the sequnce
     *
     */
    sprintf(xEndSummary.cStartTime, "%dH:%dM", xSequenceNumber.uStartHour, xSequenceNumber.uStartMin);

    /**
     * @brief setiing the sample number
     *
     */
    xEndSummary.uSampleNumber = uGetCurrentSampleNumber();

    /**
     * @brief flow set point      *
     */
    xEndSummary.fFlowSetPoint = xSequenceNumber.fFlowSetPoint;

    /**
     * @brief setting the total number of sequnce in the sample
     *
     */

    xEndSummary.uSequenceNumber = uGetNoOfSequenceInArray();

    char cDuration[10];
    /**
     * @brief getting the duration of the sample
     */
    vGetTotalDuartionOfSample(cDuration, sizeof(cDuration));

    strcpy(xEndSummary.cDuration, cDuration);

    /**
     * @brief coping the start person
     *
     */
    strcpy(xEndSummary.cStartPerson, xSequenceNumber.cStartPerson);

    /**
     * @brief getting the initial total lites before the starting of the sequnece
     *
     */
    xEndSummary.xGenericSummary.xVolumeCounter.fStartVolume = fGetTotalLiterCount();

    /**
     * @brief getting the total hours count before the starting of the sequence
     *
     */
    xEndSummary.xGenericSummary.xHourCounter.fStartHour = fGetTotalHoursCount();

    /**
     * @brief checking if sample runs sucessfully
     *
     */
    if (bIsSampleRunsWithoutProblem())
    {
        strcpy(xEndSummary.hasProblem, "No");
    }
    else
    {
        strcpy(xEndSummary.hasProblem, "Yes");
    }

    /**
     * @brief setting the targeted volume and hour values
     *
     */

    xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume = fGetTargetVolumeCount();

    xEndSummary.xGenericSummary.xHourCounter.fTargetHour = fGetTargetHourCount();

    /**
     * @brief saving the values to the nvs flash
     *
     */
    vSaveEndSummaryToNvsFlash();
}

bool bIsSampleRunsWithoutProblem()
{
    /**
     * @brief iterating over the sequence to check about  if the given sequcence has runs sucessfully
     *
     */
    sequence_t *seq = pGetAddressOfSequenceArray();

    for (uint8_t uSequenceNumber = 0; uSequenceNumber < uGetNoOfSequenceInArray(); uSequenceNumber++)
    {
        if (seq->bSucessfullyRun == false)
        {
            ESP_LOGI(TAG, "Sequence %d has not run sucessfully not checking further", uSequenceNumber);
            return false;
        }
    }
    return true;
}

float fGetTargetVolumeCount()
{
    float fTargetVolumeCount = 0;
    /**
     * @brief iterating over the sequence to get the target volume count
     *
     */
    sequence_t *seq = pGetAddressOfSequenceArray();

    for (uint8_t uSequenceNumber = 0; uSequenceNumber < uGetNoOfSequenceInArray(); uSequenceNumber++)
    {
        /**
         * @brief targer volume is determine by formula, target volume = start volume + (flow set point(per min) * duration(min))
         *
         */
        fTargetVolumeCount = fTargetVolumeCount + (seq->fFlowSetPoint * ((seq->uDurationHour * 60) + seq->uDurationMinutes));
    }
    ESP_LOGD(TAG, "target volume count is %f", fTargetVolumeCount);
    return fTargetVolumeCount;
}

float fGetTargetHourCount()
{
    float fTargerHourCounter = 0;
    /**
     * @brief iterating over the sequence to get the target hour count
     *
     */
    sequence_t *seq = pGetAddressOfSequenceArray();

    for (uint8_t uSequenceNumber = 0; uSequenceNumber < uGetNoOfSequenceInArray(); uSequenceNumber++)
    {
        /**
         * @brief converting the duration to minutes and adding it
         */
        fTargerHourCounter = fTargerHourCounter + (((seq->uDurationHour * 60) + seq->uDurationMinutes));
    }

    /**
     * @brief converting the hours into the hours
     *
     */

    fTargerHourCounter = fTargerHourCounter / 60;
    ESP_LOGD(TAG, "target hour count is %f", fTargerHourCounter);
    return fTargerHourCounter;
}

void vGetEndSummaryVariable(xSampleSummary_t *xSampleSummary)
{
    memset(xSampleSummary, 0, sizeof(xSampleSummary_t));

    memcpy(xSampleSummary, &xEndSummary, sizeof(xSampleSummary_t));
}

void vNotifySampleMangementToProceed()
{
    xTaskNotifyGive(xGetHandleSampleManagementService());
}

void vStopCurrentSample()
{
    /**
     * @brief setting the force stop sample flag to true
     *
     */
    bSampleForcedStop = true; //TODO: check

    /**
     * @brief if sequce is running stopping it
     *
     */
    if (bIsSequenceRunning())
    {
        ESP_LOGD(TAG, "Sequnce is running stopping it");
        vStopTheRunningSequnence();
    }
    else // is sequnce is not running then, take out the sample management service out of the blocked state so that the current sample will stop.
    {
        ESP_LOGD(TAG, "No sample is currently running, deleting the sample management service");

        // vSetCounterValuesEndSummaryDetails();

        // vTaskDelay(pdMS_TO_TICKS(50));
        // vControllerShowEndSummayScreen();

        /**
         * @brief setting the current sample number to invalidate or zero
         *
         */
        vSetCurrentRunningSequenceNumber(0);

        vSetCurrentSequenceNumberToNvsFlash(); // settin to the nvs flash

        /**
         * @brief deleting the sample management service task
         *
         **/
        if (xHandleSampleManagementService != NULL)
        {

            vTaskDelete(xHandleSampleManagementService);
            xHandleSampleManagementService = NULL;
        }

        /**
         * @brief restarting the sample management service once again
         *
         */

        vStartSampleManagementService();
    }
}

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
void vSetCounterValuesEndSummaryDetails()
{
    ESP_LOGD(TAG,"Setting the end summary details");
    struct tm timeinfo = {0};
    vGetCurrentDateAndTime(&timeinfo);
    char cStopDate[40];

    // sprintf(cStopDate, "%d/%d/%d", timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
    // timeinfo.tm_year +=1900;
    // timeinfo.tm_mon+=1;
    timeinfo.tm_year = timeinfo.tm_year +1900;
    timeinfo.tm_mon = timeinfo.tm_mon +1;

    sprintf(cStopDate, "%d/%d/%d", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);

    /**
     * @brief copy end date
     *
     */
    strcpy(xEndSummary.xGenericSummary.cStopDate, cStopDate);
    /**
     * @brief copy end times
     *
     */
    sprintf(xEndSummary.cStopTime, "%dH:%dM", timeinfo.tm_hour, timeinfo.tm_min);

    /**
     * @brief checking if sample runs sucessfully
     *
     */
    if (bIsSampleRunsWithoutProblem())
    {
        strcpy(xEndSummary.hasProblem, "No");
    }
    else
    {
        strcpy(xEndSummary.hasProblem, "Yes");
    }

    xEndSummary.xGenericSummary.xVolumeCounter.fEndVolume = fGetTotalLiterCount();

    xEndSummary.xGenericSummary.xVolumeCounter.fEffectiveVolume = fGetTotalLiterCount() - xEndSummary.xGenericSummary.xVolumeCounter.fStartVolume;

    /**
     * @brief calculating the variation in volume
     *
     */
    xEndSummary.xGenericSummary.xVolumeCounter.fVariation = fabs(((xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume - xEndSummary.xGenericSummary.xVolumeCounter.fEffectiveVolume) / xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume) * 100);

    xEndSummary.xGenericSummary.xHourCounter.fEndHour = fGetTotalHoursCount();
    xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour = fGetTotalHoursCount() - xEndSummary.xGenericSummary.xHourCounter.fStartHour;

    /**
     * @brief calculating the variation in hours
     *
     */
    xEndSummary.xGenericSummary.xHourCounter.fVariation = fabs(((xEndSummary.xGenericSummary.xHourCounter.fTargetHour - xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour) / xEndSummary.xGenericSummary.xHourCounter.fTargetHour) * 100);

    ESP_LOGD(TAG, "Hour counter target and effective values are %.2f and %.2f", xEndSummary.xGenericSummary.xHourCounter.fTargetHour, xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour);

    ESP_LOGD(TAG, "variation in hour is %.2f", xEndSummary.xGenericSummary.xHourCounter.fVariation);

    /**
     * @brief setting the end summary name to be ankit
     *
     */
    strcpy(xEndSummary.cEndPerson, "Time Finish");

    /**
     * @brief saving the values to the nvs flash
     *
     */
    vSaveEndSummaryToNvsFlash();

    /**
     * @brief save end summay to the db
     *
     */
}