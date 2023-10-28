/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/\
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
#include <sampleManagement.h>

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                                         "sampleManagement"
#define NVS_STORGE_NAME                             "storage"
#define SAMPLE_STORAGE_KEY                          "smplStrg"
#define SEQUENCE_STORAGE_KEY                        "seqStrg"
#define END_SUMMARY_STORAGE_KEY                     "endStrg"
#define MINIMUM_DELAY_TO_START_SAMPLE_IN_SECONDS    (30)   /*used in the case for the deep sleep configuration  */

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern SemaphoreHandle_t gui_update_semaphore;

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
static uint32_t uUniqueSampleNumber = 0;    /* This is the unique sample number in the system. Since sample  */
static uint8_t uCurrentRunningSequenceNumber = 0;   /* tells about the current running sequnce. */
static sample_summary_t endsummary;    /* this variable stores the end summary for the current sample */
bool bSampleForcedStop = false;     /* this variable stores the state of the sample. true signifies that a force stop for the sample has been achieved */

QueueHandle_t xSequenceQueue;
static TaskHandle_t xHandleSampleManagementService = NULL;  /* task handle for the sample management Service */

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void vInitializeEndSummaryVariableToZero(void);
static void vSetCounterValuesEndSummaryDetails(void);
static void vSampleManagementServiceFunction(void *pvParamaters);
static void vGetSampleNumberFromNvsFlash(void);
static void vGetCurrentSequenceNumberFromNvsFlash(void);
static void vSetCurrentSequenceNumberToNvsFlash(void);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
* 
********************************************************************************************/
static void vInitializeEndSummaryVariableToZero(void)
{
    memset(&endsummary, 0, sizeof(endsummary));
}

/********************************************************************************************
* 
********************************************************************************************/
void vSetCounterValuesEndSummaryDetails()
{
    ESP_LOGD(TAG, "Setting the end summary details");
    struct tm timeinfo = {0};
    get_current_date_time(&timeinfo);
    char cStopDate[40];

    timeinfo.tm_year = timeinfo.tm_year + 1900;
    timeinfo.tm_mon = timeinfo.tm_mon + 1;
    sprintf(cStopDate, "%d/%d/%d", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);

    
    strcpy(endsummary.genericsummary.cStopDate, cStopDate);   /* copy end date */
    sprintf(endsummary.cStopTime, "%dH:%dM", timeinfo.tm_hour, timeinfo.tm_min);   /*  copy end times  */

    if (bIsSampleRunsWithoutProblem())   /* checking if sample runs sucessfully */
    {
        strcpy(endsummary.hasProblem, "No");
    }
    else
    {
        strcpy(endsummary.hasProblem, "Yes");
    }

    endsummary.genericsummary.xVolumeCounter.fEndVolume = fGetTotalLiterCount();
    endsummary.genericsummary.xVolumeCounter.fEffectiveVolume = fGetTotalLiterCount() - endsummary.genericsummary.xVolumeCounter.fStartVolume;

    endsummary.genericsummary.xVolumeCounter.fVariation = fabs(((endsummary.genericsummary.xVolumeCounter.fTargetVolume - endsummary.genericsummary.xVolumeCounter.fEffectiveVolume) / endsummary.genericsummary.xVolumeCounter.fTargetVolume) * 100);  /* calculating the variation in volume */
    endsummary.genericsummary.xHourCounter.fEndHour = fGetTotalHoursCount();
    endsummary.genericsummary.xHourCounter.fEffectiveHour = fGetTotalHoursCount() - endsummary.genericsummary.xHourCounter.fStartHour;
    
    endsummary.genericsummary.xHourCounter.fVariation = fabs(((endsummary.genericsummary.xHourCounter.fTargetHour - endsummary.genericsummary.xHourCounter.fEffectiveHour) / endsummary.genericsummary.xHourCounter.fTargetHour) * 100);    /* calculating the variation in hours */
    ESP_LOGD(TAG, "Hour counter target and effective values are %.2f and %.2f", endsummary.genericsummary.xHourCounter.fTargetHour, endsummary.genericsummary.xHourCounter.fEffectiveHour);
    ESP_LOGD(TAG, "variation in hour is %.2f", endsummary.genericsummary.xHourCounter.fVariation);

    strcpy(endsummary.cEndPerson, "Time Finish");  
    ESP_LOGD(TAG, "saving end summary to flash");
    vSaveEndSummaryToNvsFlash();    /* saving the values to the nvs flash */
}

/********************************************************************************************
* 
********************************************************************************************/
static void vSampleManagementServiceFunction(void *pvParamaters)
{
    bSampleForcedStop = false;
    int32_t ulrequiredDelay = 0;

    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    ESP_LOGD(TAG, "Sample Management Service Started");
   
    vGetSampleNumberFromNvsFlash();             /* Getting the current sample number from nvs flash */
    vGetCurrentSequenceNumberFromNvsFlash();    /* gettting the current running sequence number from nvs flash */
    vInitializeEndSummaryVariableToZero();      /* saving the current system state to calculate the end summary */
    vGetEndSummaryFromNvsFlash();               /*  Getting the values from the nvs flash */

    /* if the system restarted or wake up from sleep then the system will start from the currenct sequence
     * to be run. To do this first cheeck if the any sequence is pending to be run. */
    if (uCurrentRunningSequenceNumber != 0)
    {
        xTaskNotifyGive(xHandleSampleManagementService);         /* give the notification to start the sample sequence */
    }
    while (1)
    {
        
        ESP_LOGD(TAG, "Waiting for the new sequnce to run");    /* waitingg for the signal to start the sequences */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ESP_LOGD(TAG, "Starting the sequence");

        /* iterating over the sequences.valid sequence are those which are less thatn the number of the sequnces and
         * also the invalid sequnce number 0 */
        while ((uCurrentRunningSequenceNumber <= get_no_of_sequence_in_array()) && (uCurrentRunningSequenceNumber != 0))
        {

            ESP_LOGD(TAG, "Starting sequence %d/%d", uCurrentRunningSequenceNumber, get_no_of_sequence_in_array());

            if ((uCurrentRunningSequenceNumber != 0) && (uCurrentRunningSequenceNumber != 1))               /* showing the wait in progress screen. wait sreen for the 1st sequene has been set from */
            {                                                                                               /* the front end. this need to be changed in the phase 2 */                            
                ESP_LOGD(TAG, "showing the wait in progress screeen");
                vShowWaitInProgressScreen(); // first sample wait screen has been shown by the gui itself
            }

            ulrequiredDelay = get_reamining_time_start_sequence(uCurrentRunningSequenceNumber);    /*  getting the delay time for the current sequence */
            if (ulrequiredDelay >= 0)
            {
                vTaskDelay(pdMS_TO_TICKS(ulrequiredDelay * 1000));
            }
            else
            {
                ESP_LOGW(TAG, "Delay time is less than 0 means that time has already passed, but can force start the sequence now");
            }

            ESP_LOGD(TAG, "Starting the sequence %d/%d", uCurrentRunningSequenceNumber, get_no_of_sequence_in_array());

            set_sequence_running(&uCurrentRunningSequenceNumber);   /*  created the sequence to run */
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    /* waiting for the sequence to finish */
            ESP_LOGD(TAG, "Task notification has been received");
            if (uCurrentRunningSequenceNumber == 0)
            {
                break;
            }
            ESP_LOGD(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber);
         
            if ((uCurrentRunningSequenceNumber == get_no_of_sequence_in_array()) || bSampleForcedStop == true)     /* show the summary screen, when all the sequneces are finished or we forced stop the samples by pressing the stop button*/
            {
                
                vSetCounterValuesEndSummaryDetails();   /* filling the end summary, when not force stopped */
                ESP_LOGD(TAG, "end of the Sample reached"); /* now need to save the end summary */
                if (!bSampleForcedStop) /* our job is finished, need to show the finised job summary screen only when the sample is time end */
                {   
                    vShowJobFinishedScreen();
                }
                
                ESP_LOGD(TAG, "Setting the sequence number to 0");  /* if the sequence is the last sequence in the array, then the sample is over, not set the sequence to run to 0. indicating that no sequnce is in progress */
                vSetCurrentRunningSequenceNumber(0);
                ESP_LOGD(TAG, "Saving the sequence number to nvs flash");
                vSetCurrentSequenceNumberToNvsFlash();
                bSampleForcedStop = false;
                break;
            }
            else
            {
                ESP_LOGD(TAG, "Icrementing the sequence number");
                uCurrentRunningSequenceNumber++;    /* if the sequence is not the last sequence in the array, then  increment the sequence number and set it to nvs flash */
                vSetCurrentSequenceNumberToNvsFlash();
            }
            ESP_LOGD(TAG, "Sequence %d finished", uCurrentRunningSequenceNumber - 1);
        }
        ESP_LOGD(TAG, "END of the Sample task ha been reached, now iterating for the new loop");
    }

    ESP_LOGE(TAG, "Error: Sample Management Service Task is exiting");
    vTaskDelete(NULL);
}

/********************************************************************************************
* 
********************************************************************************************/
static void vGetSampleNumberFromNvsFlash(void)
{
    bool ret = nvsread_value_u32(NVS_STORGE_NAME, SAMPLE_STORAGE_KEY, &uUniqueSampleNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "Sample number read error");
    }
    else
    {
        ESP_LOGD(TAG, "Unique saved sample number in nvs flash is %u", uUniqueSampleNumber);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
static void vGetCurrentSequenceNumberFromNvsFlash(void)
{
    bool ret = nvsread_value_u8(NVS_STORGE_NAME, SEQUENCE_STORAGE_KEY, &uCurrentRunningSequenceNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "current sample number read error");
    }
    else
    {
        ESP_LOGD(TAG, "Current saved sample number in nvs flash is %d", uCurrentRunningSequenceNumber);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
static void vSetCurrentSequenceNumberToNvsFlash(void)
{
    bool ret = nvswrite_value_u8(NVS_STORGE_NAME, SEQUENCE_STORAGE_KEY, uCurrentRunningSequenceNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "current sample number error");
    }

}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
* 
********************************************************************************************/
uint32_t uGetCurrentSampleNumber()
{
    return uUniqueSampleNumber;
}

/********************************************************************************************
* 
********************************************************************************************/
void vSetCurrentSampleNumber(uint32_t uSampleNumber)
{
    uUniqueSampleNumber = uSampleNumber;
}

/********************************************************************************************
* 
********************************************************************************************/
void vIncrementCurrentSampleNumber()
{
    uUniqueSampleNumber++;
}

/********************************************************************************************
* 
********************************************************************************************/
void vSetSampleNumberToNvsFlash()
{
    bool ret = nvswrite_value_u32(NVS_STORGE_NAME, SAMPLE_STORAGE_KEY, uUniqueSampleNumber);
    if(ret == false)
    {
      ESP_LOGE(TAG, "sample number write error");
    }
}



/********************************************************************************************
* 
********************************************************************************************/
uint8_t uGetCurrentRunningSequenceNumber(void)
{
    return uCurrentRunningSequenceNumber;
}

/********************************************************************************************
* 
********************************************************************************************/
void vSetCurrentRunningSequenceNumber(uint8_t uSequenceNumber)
{
    uCurrentRunningSequenceNumber = uSequenceNumber;
}

/********************************************************************************************
* 
********************************************************************************************/
void start_samplemanagement_service()
{
    xTaskCreate(vSampleManagementServiceFunction, "SampleManagementService", 4 * 1028, NULL, 5, &xHandleSampleManagementService);
}

/********************************************************************************************
* 
********************************************************************************************/
TaskHandle_t xGetHandleSampleManagementService()
{
    return xHandleSampleManagementService;
}

/********************************************************************************************
* 
********************************************************************************************/
void vSaveEndSummaryToNvsFlash()
{
    bool ret = nvswrite_value_parameters(NVS_STORGE_NAME, END_SUMMARY_STORAGE_KEY, &endsummary, sizeof(endsummary));
    if(ret == false)
    {
      ESP_LOGE(TAG, "End summary write error");
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vGetEndSummaryFromNvsFlash()
{
    bool ret = nvsread_value_parameter(NVS_STORGE_NAME, END_SUMMARY_STORAGE_KEY, &endsummary);
    if(ret == false)
    {
      ESP_LOGE(TAG, "End summary read error");
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vSetInitialCounterValuesToEndSummary()
{
    sequence_t xSequenceNumber;
    char cDuration[10];

   
    vInitializeEndSummaryVariableToZero();   /* Construct a new v Initialize End Summary Variable To Zero object */
    
    get_sequence_info(&xSequenceNumber, 1); /*  getting the 1 sequence from the sequence array  */
    strcpy(endsummary.genericsummary.cStartDate, xSequenceNumber.cStartDate);
    sprintf(endsummary.starttime, "%dH:%dM", xSequenceNumber.uStartHour, xSequenceNumber.uStartMin);  /* getting the start time of the sequnce */
    endsummary.uSampleNumber = uGetCurrentSampleNumber();   /* setiing the sample number */
    endsummary.fFlowSetPoint = xSequenceNumber.fFlowSetPoint;  /* flow set point  */
    endsummary.uSequenceNumber = get_no_of_sequence_in_array();    /* setting the total number of sequnce in the sample */
    get_total_duration_sample(cDuration, sizeof(cDuration));    /* getting the duration of the sample */
    strcpy(endsummary.cDuration, cDuration);
    strcpy(endsummary.cStartPerson, xSequenceNumber.cStartPerson); /* coping the start person */
    endsummary.genericsummary.xVolumeCounter.fStartVolume = fGetTotalLiterCount();    /* getting the initial total lites before the starting of the sequnece */
    endsummary.genericsummary.xHourCounter.fStartHour = fGetTotalHoursCount();        /* getting the total hours count before the starting of the sequence  */

    if (bIsSampleRunsWithoutProblem())   /* checking if sample runs sucessfully */
    {
        strcpy(endsummary.hasProblem, "No");
    }
    else
    {
        strcpy(endsummary.hasProblem, "Yes");
    }

    
    endsummary.genericsummary.xVolumeCounter.fTargetVolume = get_target_volumecount(); /* setting the targeted volume and hour values */
    endsummary.genericsummary.xHourCounter.fTargetHour = get_target_hourcount();
    vSaveEndSummaryToNvsFlash();    /* saving the values to the nvs flash */
}

/********************************************************************************************
* 
********************************************************************************************/
bool bIsSampleRunsWithoutProblem()
{
    sequence_t *seq = get_sequence_array(); /* iterating over the sequence to check about  if the given sequcence has runs sucessfully  */
    for (uint8_t uSequenceNumber = 0; uSequenceNumber < get_no_of_sequence_in_array(); uSequenceNumber++)
    {
        if (seq->bSucessfullyRun == false)
        {
            ESP_LOGD(TAG, "Sequence %d has not run sucessfully not checking further", uSequenceNumber);
            return false;
        }
    }
    return true;
}

/********************************************************************************************
* 
********************************************************************************************/
float get_target_volumecount(void)
{
    float target_volumecount = 0;

    sequence_t *seq = get_sequence_array();  /* iterating over the sequence to get the target volume count  */
    for (uint8_t uSequenceNumber = 0; uSequenceNumber < get_no_of_sequence_in_array(); uSequenceNumber++)
    {
        /* target volume is determine by formula, target volume = start volume + (flow set point(per min) * duration(min)) */
        target_volumecount = target_volumecount + (seq->fFlowSetPoint * ((seq->uDurationHour * 60) + seq->uDurationMinutes));
    }
    ESP_LOGD(TAG, "target volume count is %f", target_volumecount);
    return target_volumecount;
}

/********************************************************************************************
* 
********************************************************************************************/
float get_target_hourcount(void)
{
    float fTargerHourCounter = 0;

    sequence_t *seq = get_sequence_array(); /* iterating over the sequence to get the target hour count */
    for (uint8_t uSequenceNumber = 0; uSequenceNumber < get_no_of_sequence_in_array(); uSequenceNumber++)
    {
        fTargerHourCounter = fTargerHourCounter + (((seq->uDurationHour * 60) + seq->uDurationMinutes));            /* converting the duration to minutes and adding it */
    }
    fTargerHourCounter = fTargerHourCounter / 60;   /* converting the hours into the hours */
    ESP_LOGD(TAG, "target hour count is %f", fTargerHourCounter);
    return fTargerHourCounter;
}

/********************************************************************************************
* 
********************************************************************************************/
void vGetEndSummaryVariable(sample_summary_t *samplebuf)
{
    memset(samplebuf, 0, sizeof(sample_summary_t));
    memcpy(samplebuf, &endsummary, sizeof(sample_summary_t));
}

/********************************************************************************************
* 
********************************************************************************************/
void vNotifySampleMangementToProceed()
{
    xTaskNotifyGive(xGetHandleSampleManagementService());
}

/********************************************************************************************
* 
********************************************************************************************/
void vStopCurrentSample()
{
    
    bSampleForcedStop = true; /* setting the force stop sample flag to true */

    
    if (is_sequence_running())   /* if sequce is running stopping it */
    {
        ESP_LOGD(TAG, "Sequnce is running stopping it");
        stop_running_sequence();
    }
    else // is sequnce is not running then, take out the sample management service out of the blocked state so that the current sample will stop.
    {
        ESP_LOGD(TAG, "No sample is currently running, deleting the sample management service");
        vSetCurrentRunningSequenceNumber(0);    /* setting the current sample number to invalidate or zero */
        vSetCurrentSequenceNumberToNvsFlash(); // settin to the nvs flash
        if (xHandleSampleManagementService != NULL) /* deleting the sample management service task */
        {
            vTaskDelete(xHandleSampleManagementService);
            xHandleSampleManagementService = NULL;
        }
        start_samplemanagement_service();    /* restarting the sample management service once again */
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vSaveSampleValues(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson)
{
    set_sequence_values(uSequenceNumber, pStartDate, uStartHour, uStartMin, fFlowSetPoint, uDurationHour, uDurationMinutes, pStartPerson);
}

/********************************************************************************************
* 
********************************************************************************************/
void vControllerInitializeSampleArray()
{
    sequence_array_initialize();
}

/********************************************************************************************
* 
********************************************************************************************/
void vControllerSampleIsValid(void)
{
    ESP_LOGD(TAG, "Sample is valid");

    vSetCurrentRunningSequenceNumber(1);    /*  Set the current running sequece number to 1. because we are starting the sequence from 1 */
    vIncrementCurrentSampleNumber();    /* incrementing the unique sample number  */
    vSetSampleNumberToNvsFlash();       /* saving the sample number in the flash memory */
    nvswrite_sequence_array();           /* saving the sequences to the nvs flash */
    nvswrite_totalsequence_count();      /* Setting the total sequence to the nvs flash */
    vSetInitialCounterValuesToEndSummary(); /* we need to set some values which we mention in the end summary, like start date and total liter and hour counts */
    vStartJob();     /* start the job */
}

/********************************************************************************************
* 
********************************************************************************************/
void vStartJob()
{
    ESP_LOGD(TAG, "Starting the job");
    vNotifySampleMangementToProceed();  /* giving the task notification to start the sample management */
}

/********************************************************************************************
* 
********************************************************************************************/
void vShowWaitInProgressScreen()
{
    ESP_LOGD(TAG, "Showing the wait in progress screen");

    if (xSemaphoreTake(gui_update_semaphore, portMAX_DELAY) == pdTRUE)
    {
        dashboardflg = 3; // dashboard flag for the wait screen
        DashboardInfoWidget();
        lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
        lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

        xSemaphoreGive(gui_update_semaphore);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vShowWorkInProgressScreen()
{
    ESP_LOGD(TAG, "Showing the work in progress screen");

    if (xSemaphoreTake(gui_update_semaphore, portMAX_DELAY) == pdTRUE)
    {
        dashboardflg = 1; // dashboard screen flag
        DashboardInfoWidget();
        xSemaphoreGive(gui_update_semaphore);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vUpdateWorkInProgressScreen()
{
    ESP_LOGV(TAG, "Updating the work in progress screen");

    if (xSemaphoreTake(gui_update_semaphore, portMAX_DELAY) == pdTRUE)
    {
        ESP_LOGV(TAG, "Updating dashboard screen");
        vUpdateDashboardScreen();
        xSemaphoreGive(gui_update_semaphore);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void vShowJobFinishedScreen()
{
    ESP_LOGD(TAG, "Showing the job finish screen");
    vTaskDelay(100);
    vShowJobFinishedDashboardScreen();
}

/********************************************************************************************
* 
********************************************************************************************/
void vControllerSampleStop()
{
    vStopCurrentSample();
}

/********************************************************************************************
* 
********************************************************************************************/
void vControllerShowEndSummayScreen()
{
    ESP_LOGD(TAG, "Showing the end summary screen");
    if (xSemaphoreTake(gui_update_semaphore, portMAX_DELAY) == pdTRUE)
    {
        xseSummaryEndScreen();
        xSemaphoreGive(gui_update_semaphore);
    }
}