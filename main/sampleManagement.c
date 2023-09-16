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
#include <string.h>
#include <math.h>

#include <sampleManagement.h>
#include <sequenceManagement.h>

/************************************************defines**********************************************/

#define TAG                     "sampleManagement"
#define NVS_STORGE_NAME         "storage"
#define SAMPLE_STORAGE_KEY      "smplStrg"
#define SEQUENCE_STORAGE_KEY    "seqStrg"
#define END_SUMMARY_STORAGE_KEY "endStrg"

/*used in the case for the deep sleep configuration  */
#define MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS (30)

/***************************************************variables*******************************************/
extern SemaphoreHandle_t xGuiSemaphore1;
/* This is the unique sample number in the system. Since sample  */
static uint32_t uUniqueSampleNumber = 0;
/* tells about the current running sequnce. */
static uint8_t uCurrentRunningSequenceNumber = 0;
QueueHandle_t xSequenceQueue;
/* task handle for the sample management Service */
static TaskHandle_t xHandleSampleManagementService = NULL;
/* this variable stores the end summary for the current sample */
static xSampleSummary_t xEndSummary;
/* this variable stores the state of the sample. true signifies that a force stop for the sample has been achieved */
bool bSampleForcedStop = false;

/***********************************************function prototypes***************************************/

/*  initialize the end summary variable to zero */
void vInitializeEndSummaryVariableToZero();
/*  save the volume counters, hours counters and other values to the end summary variable. */
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
    bool ret = nvswrite_value_u32(NVS_STORGE_NAME, SAMPLE_STORAGE_KEY, uUniqueSampleNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "sample number write error");
    }
}

void vGetSampleNumberFromNvsFlash()
{
    bool ret = nvsread_value_u32(NVS_STORGE_NAME, SAMPLE_STORAGE_KEY, &uUniqueSampleNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "Sample number read error");
    }
    else
    {
        ESP_LOGI(TAG, "Unique saved sample number in nvs flash is %u", uUniqueSampleNumber);
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
    bool ret = nvswrite_value_u8(NVS_STORGE_NAME, SEQUENCE_STORAGE_KEY, uCurrentRunningSequenceNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "current sample number error");
    }
}

void vGetCurrentSequenceNumberFromNvsFlash()
{
    bool ret = nvsread_value_u8(NVS_STORGE_NAME, SEQUENCE_STORAGE_KEY, &uCurrentRunningSequenceNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "current sample number read error");
    }
    else
    {
        ESP_LOGI(TAG, "Current saved sample number in nvs flash is %d", uCurrentRunningSequenceNumber);
    }
}

static void vSampleManagementServiceFunction(void *pvParamaters)
{
    bSampleForcedStop = false;
    int32_t ulrequiredDelay = 0;

    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    ESP_LOGD(TAG, "Sample Management Service Started");
   
    vGetSampleNumberFromNvsFlash();             /* Getting the current sample number from nvs flash */
    vGetCurrentSequenceNumberFromNvsFlash();    /* gettting the current running sequence number from nvs flash */
    vInitializeEndSummaryVariableToZero();      /* saving the current system state to calculate the end summary */
    vGetEndSummaryFromNvsFlash();       /*  Getting the values from the nvs flash */

    /* if the system restarted or wake up from sleep then the system will start from the currenct sequence
     * to be run. To do this first cheeck if the any sequence is pending to be run. */
    if (uCurrentRunningSequenceNumber != 0)
    {
        xTaskNotifyGive(xHandleSampleManagementService);         /* give the notification to start the sample sequence */
    }
    while (1)
    {
        /* waitingg for the signal to start the sequences */
        ESP_LOGI(TAG, "Waiting for the new sequnce to run");
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ESP_LOGI(TAG, "Starting the sequence");

        /* iterating over the sequences.valid sequence are those which are less thatn the number of the sequnces and
         * also the invalid sequnce number 0 */
        while ((uCurrentRunningSequenceNumber <= uGetNoOfSequenceInArray()) && (uCurrentRunningSequenceNumber != 0))
        {

            ESP_LOGI(TAG, "Starting sequence %d/%d", uCurrentRunningSequenceNumber, uGetNoOfSequenceInArray());

            /* showing the wait in progress screen. wait sreen for the 1st sequene has been set from
             * the front end. this need to be changed in the phase 2 */
            if ((uCurrentRunningSequenceNumber != 0) && (uCurrentRunningSequenceNumber != 1))
            {
                ESP_LOGI(TAG, "showing the wait in progress screeen");
                vShowWaitInProgressScreen(); // first sample wait screen has been shown by the gui itself
            }
            // vShowWaitInProgressScreen();

            /*  getting the delay time for the current sequence */
            ulrequiredDelay = uGetNumberOfSecondRemainingToStartSequence(uCurrentRunningSequenceNumber);
            if (ulrequiredDelay >= 0)
            {
                vTaskDelay(pdMS_TO_TICKS(ulrequiredDelay * 1000));
            }
            else
            {
                ESP_LOGE(TAG, "Error: Delay time is less than 0 means that time has already passed, but can force start the sequence now");
            }

            ESP_LOGI(TAG, "Starting the sequence %d/%d", uCurrentRunningSequenceNumber, uGetNoOfSequenceInArray());

            /*  created the sequence to run */
            vSetSequenceToRun(&uCurrentRunningSequenceNumber);
            /* waiting for the sequence to finish */
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            ESP_LOGD(TAG, "Task notification has been received");
            if (uCurrentRunningSequenceNumber == 0)
            {
                break;
            }
            ESP_LOGI(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber);
            /* show the summary screen, when all the sequneces are finished or we forced stop the samples
             * by pressing the stop button
             */
            if ((uCurrentRunningSequenceNumber == uGetNoOfSequenceInArray()) || bSampleForcedStop == true)
            {
                /* filling the end summary, when not force stopped */
                vSetCounterValuesEndSummaryDetails();
                /* now need to save the end summary */
                ESP_LOGI(TAG, "end of the Sample reached");
                /* our job is finished, need to show the finised job summary screen only when the sample
                 * is time end */
                if (!bSampleForcedStop)
                {
                    vShowJobFinishedScreen();
                }
                /* if the sequence is the last sequence in the array, then the sample is over, not set
                 * the sequence to run to 0. indicating that no sequnce is in progress */
                ESP_LOGI(TAG, "Setting the sequence number to 0");
                vSetCurrentRunningSequenceNumber(0);
                ESP_LOGI(TAG, "Saving the sequence number to nvs flash");
                vSetCurrentSequenceNumberToNvsFlash();
                bSampleForcedStop = false;
                break;
            }
            else
            {
                ESP_LOGI(TAG, "Icrementing the sequence number");
                /* if the sequence is not the last sequence in the array, then  increment the sequence
                 * number and set it to nvs flash */
                uCurrentRunningSequenceNumber++;
                vSetCurrentSequenceNumberToNvsFlash();
            }
            ESP_LOGI(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber - 1);
        }
        ESP_LOGI(TAG, "END of the Sample task ha been reached, now iterating for the new loop");
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
    bool ret = nvswrite_value_parameters(NVS_STORGE_NAME, END_SUMMARY_STORAGE_KEY, &xEndSummary, sizeof(xEndSummary));
    if(ret == false)
    {
      ESP_LOGE(TAG, "End summary write error");
    }
}

void vGetEndSummaryFromNvsFlash()
{
    bool ret = nvsread_value_parameter(NVS_STORGE_NAME, END_SUMMARY_STORAGE_KEY, &xEndSummary);
    if(ret == false)
    {
      ESP_LOGE(TAG, "End summary read error");
    }
}

void vSetInitialCounterValuesToEndSummary()
{
    sequence_t xSequenceNumber;
    char cDuration[10];

    /* Construct a new v Initialize End Summary Variable To Zero object */
    vInitializeEndSummaryVariableToZero();
    /*  getting the 1 sequence from the sequence array  */
    vGetNthSaequenceFromArray(&xSequenceNumber, 1);
    strcpy(xEndSummary.xGenericSummary.cStartDate, xSequenceNumber.cStartDate);
    /* getting the start time of the sequnce */
    sprintf(xEndSummary.cStartTime, "%dH:%dM", xSequenceNumber.uStartHour, xSequenceNumber.uStartMin);
    /* setiing the sample number */
    xEndSummary.uSampleNumber = uGetCurrentSampleNumber();
    /* flow set point  */
    xEndSummary.fFlowSetPoint = xSequenceNumber.fFlowSetPoint;
    /* setting the total number of sequnce in the sample */
    xEndSummary.uSequenceNumber = uGetNoOfSequenceInArray();
    /* getting the duration of the sample */
    vGetTotalDuartionOfSample(cDuration, sizeof(cDuration));
    strcpy(xEndSummary.cDuration, cDuration);
    /* coping the start person */
    strcpy(xEndSummary.cStartPerson, xSequenceNumber.cStartPerson);
    /* getting the initial total lites before the starting of the sequnece */
    xEndSummary.xGenericSummary.xVolumeCounter.fStartVolume = fGetTotalLiterCount();
    /* getting the total hours count before the starting of the sequence  */
    xEndSummary.xGenericSummary.xHourCounter.fStartHour = fGetTotalHoursCount();

    /* checking if sample runs sucessfully */
    if (bIsSampleRunsWithoutProblem())
    {
        strcpy(xEndSummary.hasProblem, "No");
    }
    else
    {
        strcpy(xEndSummary.hasProblem, "Yes");
    }

    /* setting the targeted volume and hour values */
    xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume = fGetTargetVolumeCount();
    xEndSummary.xGenericSummary.xHourCounter.fTargetHour = fGetTargetHourCount();
    /* saving the values to the nvs flash */
    vSaveEndSummaryToNvsFlash();
}

bool bIsSampleRunsWithoutProblem()
{
    /* iterating over the sequence to check about  if the given sequcence has runs sucessfully  */
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

    /* iterating over the sequence to get the target volume count  */
    sequence_t *seq = pGetAddressOfSequenceArray();

    for (uint8_t uSequenceNumber = 0; uSequenceNumber < uGetNoOfSequenceInArray(); uSequenceNumber++)
    {
        /* target volume is determine by formula, target volume = start volume + (flow set point(per min) * duration(min)) */
        fTargetVolumeCount = fTargetVolumeCount + (seq->fFlowSetPoint * ((seq->uDurationHour * 60) + seq->uDurationMinutes));
    }
    ESP_LOGD(TAG, "target volume count is %f", fTargetVolumeCount);
    return fTargetVolumeCount;
}

float fGetTargetHourCount()
{
    float fTargerHourCounter = 0;

    /* iterating over the sequence to get the target hour count */
    sequence_t *seq = pGetAddressOfSequenceArray();

    for (uint8_t uSequenceNumber = 0; uSequenceNumber < uGetNoOfSequenceInArray(); uSequenceNumber++)
    {
        /* converting the duration to minutes and adding it */
        fTargerHourCounter = fTargerHourCounter + (((seq->uDurationHour * 60) + seq->uDurationMinutes));
    }

    /* converting the hours into the hours */
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
    /* setting the force stop sample flag to true */
    bSampleForcedStop = true; // TODO: check

    /* if sequce is running stopping it */
    if (bIsSequenceRunning())
    {
        ESP_LOGD(TAG, "Sequnce is running stopping it");
        vStopTheRunningSequnence();
    }
    else // is sequnce is not running then, take out the sample management service out of the blocked state so that the current sample will stop.
    {
        ESP_LOGI(TAG, "No sample is currently running, deleting the sample management service");
        /* setting the current sample number to invalidate or zero */
        vSetCurrentRunningSequenceNumber(0);
        vSetCurrentSequenceNumberToNvsFlash(); // settin to the nvs flash
        /* deleting the sample management service task */
        if (xHandleSampleManagementService != NULL)
        {
            vTaskDelete(xHandleSampleManagementService);
            xHandleSampleManagementService = NULL;
        }

        /* restarting the sample management service once again */
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
    ESP_LOGD(TAG, "Setting the end summary details");
    struct tm timeinfo = {0};
    vGetCurrentDateAndTime(&timeinfo);
    char cStopDate[40];

    timeinfo.tm_year = timeinfo.tm_year + 1900;
    timeinfo.tm_mon = timeinfo.tm_mon + 1;
    sprintf(cStopDate, "%d/%d/%d", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);

    /* copy end date */
    strcpy(xEndSummary.xGenericSummary.cStopDate, cStopDate);
    /*  copy end times  */
    sprintf(xEndSummary.cStopTime, "%dH:%dM", timeinfo.tm_hour, timeinfo.tm_min);

    /* checking if sample runs sucessfully */
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

    /* calculating the variation in volume */
    xEndSummary.xGenericSummary.xVolumeCounter.fVariation = fabs(((xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume - xEndSummary.xGenericSummary.xVolumeCounter.fEffectiveVolume) / xEndSummary.xGenericSummary.xVolumeCounter.fTargetVolume) * 100);
    xEndSummary.xGenericSummary.xHourCounter.fEndHour = fGetTotalHoursCount();
    xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour = fGetTotalHoursCount() - xEndSummary.xGenericSummary.xHourCounter.fStartHour;

    /* calculating the variation in hours */
    xEndSummary.xGenericSummary.xHourCounter.fVariation = fabs(((xEndSummary.xGenericSummary.xHourCounter.fTargetHour - xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour) / xEndSummary.xGenericSummary.xHourCounter.fTargetHour) * 100);
    ESP_LOGD(TAG, "Hour counter target and effective values are %.2f and %.2f", xEndSummary.xGenericSummary.xHourCounter.fTargetHour, xEndSummary.xGenericSummary.xHourCounter.fEffectiveHour);
    ESP_LOGD(TAG, "variation in hour is %.2f", xEndSummary.xGenericSummary.xHourCounter.fVariation);

    /* setting the end summary name to be ankit */
    strcpy(xEndSummary.cEndPerson, "Time Finish");
    /* saving the values to the nvs flash */
    vSaveEndSummaryToNvsFlash();
}

void vSaveSampleValues(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson)
{

    vSetSequenceValues(uSequenceNumber, pStartDate, uStartHour, uStartMin, fFlowSetPoint, uDurationHour, uDurationMinutes, pStartPerson);
}

void vControllerInitializeSampleArray()
{
    ESP_LOGD(TAG, "iniitalizing the array");
    vInitializeSequenceArray();
}

void vControllerSampleIsValid()
{
    ESP_LOGD(TAG, "Sample is valid");

    /*  Set the current running sequece number to 1. because we are starting the sequence from 1 */
    vSetCurrentRunningSequenceNumber(1);
    /* incrementing the unique sample number  */
    vIncrementCurrentSampleNumber();
    /* saving the sample number in the flash memory */
    vSetSampleNumberToNvsFlash();
    /* saving the sequences to the nvs flash */
    vSetSequenceArrayToNVS();

    /* Setting the total sequence to the nvs flash */
    vSetTotalSequenceCountToNvs();

    /* we need to set some values which we mention in the end summary, like start date and total
     * liter and hour counts */
    vSetInitialCounterValuesToEndSummary();

    /* start the job */
    vStartJob();
}

void vStartJob()
{
    ESP_LOGI(TAG, "Starting the job");

    /* giving the task notification to start the sample management */
    vNotifySampleMangementToProceed();
}

void vShowWaitInProgressScreen()
{
    ESP_LOGD(TAG, "Showing the wait in progress screen");

    if (xSemaphoreTake(xGuiSemaphore1, portMAX_DELAY) == pdTRUE)
    {
        dashboardflg = 3; // dashboard flag for the wait screen
        DashboardInfoWidget();
        lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
        lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

        xSemaphoreGive(xGuiSemaphore1);
    }
}

void vShowWorkInProgressScreen()
{
    ESP_LOGD(TAG, "Showing the work in progress screen");

    if (xSemaphoreTake(xGuiSemaphore1, portMAX_DELAY) == pdTRUE)
    {
        dashboardflg = 1; // dashboard screen flag
        DashboardInfoWidget();
        // pxDashboardScreen();
        xSemaphoreGive(xGuiSemaphore1);
    }
}

void vUpdateWorkInProgressScreen()
{
    ESP_LOGD(TAG, "Updating the work in progress screen");

    if (xSemaphoreTake(xGuiSemaphore1, portMAX_DELAY) == pdTRUE)
    {
        ESP_LOGD(TAG, "Updating dashboard screen");
        vUpdateDashboardScreen();
        xSemaphoreGive(xGuiSemaphore1);
    }
}

void vShowJobFinishedScreen()
{
    ESP_LOGI(TAG, "Showing the job finish screen");
    vTaskDelay(100);
    vShowJobFinishedDashboardScreen();
}

void vControllerSampleStop()
{
    // vStopCurrentSample();
    /* Stopping the current on going sequence */
    vStopCurrentSample();
}

void vControllerShowEndSummayScreen()
{
    ESP_LOGI(TAG, "Showing the end summary screen");
    if (xSemaphoreTake(xGuiSemaphore1, portMAX_DELAY) == pdTRUE)
    {
        xseSummaryEndScreen();
        xSemaphoreGive(xGuiSemaphore1);
    }
}