/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
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

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                         "Sequnce_Management"
#define NVS_STORGE_NAME             "sequence"
#define STORAGE_KEY                 "arrStrg"
#define TOTAL_SEQUENCE_COUNT_KEY    "ttlSeqCnt"
#define EACH_SEC_INTO_HOURS         0.000277778         //value of seconds in an hour (1/3600)

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern int dashboardflg;    /* this is the dash board flag to set the mode of the dashboard in the gui, need to set before calling the DashboardInfoWidget() to set the mode. */
extern SemaphoreHandle_t gui_update_semaphore;  /* semaphore for the gui handle  */
extern char guiTime[25];

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
sequenceSummary_t sequencesummary;  /* variable to store the sequence summary data */
static sequence_t totalSequence[MAXIMUM_NO_OF_SAMPLES];     /* this will store all the samples in the system */
uint8_t uTotalSequenceCount = 0;        /*  total number of samples in the system */
uint16_t uTotalTimePassInSeconds = 0;   /* this counts the number of seconds passed since the start of the sequence */
bool bDeleteUpdateScreenAndVaulesTask = false;  /* it stores is it safe to delete the task or not */

static TaskHandle_t xTaskHandleUpdateScreenAndSaveValuesEverySecond = NULL;
static TaskHandle_t xTaskHandleRunSequnceTask = NULL;   /* task handle which contols the sequence task */
static TaskHandle_t xTaskHandleMonitorSensorData = NULL;
static SemaphoreHandle_t xStopTheRunningSequenceSemaphore = NULL;   /* This semaphore will stop the ongoing sequence. This will be used for the force stop the running sequnence  */
/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void vUpdateScreenAndSaveValuesEverySecond(void *pvParameters);
static void vCalculateSequneceEndSummary();
static void vInitiateSequenceSummaryStart();
static void vMonitorSensorDataTask(void *pvParameters);
static float fCalculateVariationInPercentage(float fMaxValue, float fMinValue, float fTargetValue);
static void print_on_terminal(void);
static void nvread_totalsequence_count(void);
static void nvsread_sequencedata(void);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
* 
********************************************************************************************/
static void print_on_terminal(void)
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

/********************************************************************************************
* 
********************************************************************************************/
static void vUpdateScreenAndSaveValuesEverySecond(void *pvParameters)
{
    ESP_LOGD(TAG, "Starting the task to update the screen and save the values to the nvs flash");
    uint8_t usequenceToRun;
    memcpy(&usequenceToRun, pvParameters, sizeof(uint8_t));
    ESP_LOGD(TAG, "Running sequence %d", usequenceToRun);

    float fTotalHourCountTemp = 0;
    uint32_t uTotalTimePassInSec = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint64_t uSequnceDurationInSec = ((totalSequence[usequenceToRun - 1].uDurationHour * 3600) + (totalSequence[usequenceToRun - 1].uDurationMinutes * 60));
    float fPercentageOfJobDone = 0;
    vSetTotalSecondPassesInGivenSequence(0);    /* setting the total volume and seconds pass in the sequence to zero */
    vSetTotalLitersHasBeenPassInGivenSequence(0);

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); // ensure that the calling frequnency be constant

        /*  When we delete the task using the task delete function, there may be time that task is in middle of something and holding the semaphore. this varialbe ensure that while delting the task, the task
         * does not hold any semaphore. */
        if (!bDeleteUpdateScreenAndVaulesTask)
        {
            uTotalTimePassInSec++;
            vSetTotalSecondPassesInGivenSequence(uTotalTimePassInSec);   /* Setting number of second pass in the sequence */
            fPercentageOfJobDone = ((float)uTotalTimePassInSec / (float)uSequnceDurationInSec) * 100;
            
            fSetPercentageOfJobDone(fPercentageOfJobDone);  /* Setting the percent of job done */
            fTotalHourCountTemp = fGetTotalHoursCount();
           
            fTotalHourCountTemp += EACH_SEC_INTO_HOURS;  /* adding each second pass to the total hours count */
            vSetTotalHoursCount(fTotalHourCountTemp);
            vUpdateWorkInProgressScreen();  /* updating the screen for the total hours and liters */
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

/********************************************************************************************
* 
********************************************************************************************/
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

        get_external_sensor_calibratedvalue(&external_sensor_data); /* getting the extenal sensor data from sensor management */
        get_external_sensor_data_raw(&raw_sensor_data);
        get_ina3221_sensor_data(&xInaSensorData[0]);   /* getting the INA data */

        /* calulating the mean temp, humidity and pressure */
        sequencesummary.ambientTemperature.fMeanTemperature = ((sequencesummary.ambientTemperature.fMeanTemperature * (uCounterForCalculatingMeanValues - 1)) + external_sensor_data.temperature) / uCounterForCalculatingMeanValues;
        sequencesummary.ambientHumidity.fMeanAmbientHumidity = (sequencesummary.ambientHumidity.fMeanAmbientHumidity * (uCounterForCalculatingMeanValues - 1) + external_sensor_data.humidity) / uCounterForCalculatingMeanValues;
        sequencesummary.ambientPressure.fMeanAmbientPressure = (sequencesummary.ambientPressure.fMeanAmbientPressure * (uCounterForCalculatingMeanValues - 1) + external_sensor_data.pressure) / uCounterForCalculatingMeanValues;

        /*  setting the min and max values for the external sensor data */
        sequencesummary.ambientTemperature.fMaxTemperature = (sequencesummary.ambientTemperature.fMaxTemperature > external_sensor_data.temperature) ? sequencesummary.ambientTemperature.fMaxTemperature : external_sensor_data.temperature;
        sequencesummary.ambientTemperature.fMinTemperature = (sequencesummary.ambientTemperature.fMinTemperature < external_sensor_data.temperature) ? sequencesummary.ambientTemperature.fMinTemperature : external_sensor_data.temperature;
        sequencesummary.ambientHumidity.fMaxAmbientHumidity = (sequencesummary.ambientHumidity.fMaxAmbientHumidity > external_sensor_data.humidity) ? sequencesummary.ambientHumidity.fMaxAmbientHumidity : external_sensor_data.humidity;
        sequencesummary.ambientHumidity.fMinAmbientHumidity = (sequencesummary.ambientHumidity.fMinAmbientHumidity < external_sensor_data.humidity) ? sequencesummary.ambientHumidity.fMinAmbientHumidity : external_sensor_data.humidity;
        sequencesummary.ambientPressure.fMaxAmbientPressure = (sequencesummary.ambientPressure.fMaxAmbientPressure > external_sensor_data.pressure) ? sequencesummary.ambientPressure.fMaxAmbientPressure : external_sensor_data.pressure;
        sequencesummary.ambientPressure.fMinAmbientPressure = (sequencesummary.ambientPressure.fMinAmbientPressure < external_sensor_data.pressure) ? sequencesummary.ambientPressure.fMinAmbientPressure : external_sensor_data.pressure;

        if ((external_sensor_data.temperature < EXTERNAL_SENSOR_TEMPERATURE_MIN_VALUE) || (external_sensor_data.temperature > EXTERNAL_SENSOR_TEMPERATURE_MAX_VALUE)) /* checking the external sensor temprature is valid or not */
        {
            sequencesummary.ambientTemperature.bIsInRange = false;  /* raise the flag inside the sequence summary to indicate the error */
        }
        if ((external_sensor_data.humidity < EXTERNAL_SENSOR_HUMIDITY_MIN_VALUE) || (external_sensor_data.humidity > EXTERNAL_SENSOR_HUMIDITY_MAX_VALUE)) /* checking if the external sensor humidity is valid or not */
        {
            sequencesummary.ambientHumidity.bIsInRange = false;  /* raise the flag inside the sequence summary to indicate the error */
        }
        if ((external_sensor_data.pressure < EXTERNAL_SENSOR_PRESSURE_MIN_VALUE) || (external_sensor_data.pressure > EXTERNAL_SENOSR_PRESSURE_MAX_VALUE)) /* checking if the external sensor pressure is valid or not */
        {
            sequencesummary.ambientPressure.bIsInRange = false;  /* raise the flag inside the sequence summary to indicate the error */
        }

        uint64_t uConterForAirFlowRate = uCounterForCalculatingMeanValues - 10; // we need to calulate the air flow rate after the 10 seconds or iteration has been passed. All above calculations will be performed for the air flow as well
        if (uConterForAirFlowRate > 0) // after 10 seconds or iteration has been passed
        {
            sequencesummary.airflowVolumetric.fAirflowMean = ((sequencesummary.airflowVolumetric.fAirflowMean * (uConterForAirFlowRate - 1)) + sequencesummary.airflowVolumetric.fAirflowMean) / uConterForAirFlowRate; /* calculating the mean air flow rate */
            sequencesummary.airflowVolumetric.fAirflowMaxValue = (sequencesummary.airflowVolumetric.fAirflowMaxValue > get_volumetric_flow()) ? sequencesummary.airflowVolumetric.fAirflowMaxValue : get_volumetric_flow();/* calculating the max and min for the air flow rate */
            sequencesummary.airflowVolumetric.fAirflowMinValue = (sequencesummary.airflowVolumetric.fAirflowMinValue < get_volumetric_flow()) ? sequencesummary.airflowVolumetric.fAirflowMinValue : get_volumetric_flow();
        }

        if (!bOneTime)
        {
            printf("------------------Settings-----------------------\n");
            printf(" kp is: %0.2f || Ki is: %0.2f || kd is: %0.2f || Akp is :%0.2f || Aki is: %0.2f||Akd is: %0.2f \n", fGetPIDParameterKp(), fGetPIDParameterKi(), fGetPIDParameterKd(), fGetPIDParameterAkp(), fGetPIDParameterAki(), fGetPIDParameterAkd());
            printf("------------------Settings-----------------------\n");
            bOneTime = true;

            printf("Hardware Time, SDP Temp, SDP DP, SDP Massflow, Ch0 Voltage, Ch0 Shunt Voltage, Ch0 Current, Ch1 Voltage, Ch1 Shunt Voltage, Ch1 Current, Ch2 Voltage, Ch2 Shunt Voltage, Ch2 Current, Ext Temp Raw, Ext Humidity Raw, Ext Pressure Raw, Ext AirDensity Raw, Int Temp Raw, Int Humidity Raw, Int Pressure Raw, Int AirDensity Raw, Ext Temp user, Ext Humidity user, Ext Pressure user, Ext AirDensity user, Int Temp user, Int Humidity user, Int Pressure user, Int AirDensity user, Volumetric Flow, Hour counter, Liter Counter\n");
        }
        printf("%llu,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n", esp_timer_get_time(),get_sdp32_temperature_value(), get_sdp32_pressure_value(), get_sdp32_massflow_value(), xInaSensorData[0].bus_voltage, xInaSensorData[0].shunt_voltage, xInaSensorData[0].shunt_current, xInaSensorData[1].bus_voltage, xInaSensorData[1].shunt_voltage, xInaSensorData[1].shunt_current, xInaSensorData[2].bus_voltage, xInaSensorData[2].shunt_voltage, xInaSensorData[2].shunt_current, raw_sensor_data.temperature, raw_sensor_data.humidity, raw_sensor_data.pressure, get_external_air_density_raw(), get_internal_temperature_value(), get_internal_humidity_value(), get_internal_pressure_value(),  get_internal_air_density_raw(), external_sensor_data.temperature, external_sensor_data.humidity, external_sensor_data.pressure, get_external_air_density_calibrated(), get_internal_temperature_calibrated(), get_internal_humidity_calibrated(), get_internal_pressure_calibrated(),  get_internal_air_density_calibrated(), get_volumetric_flow(), fGetTotalHoursCount(), fGetTotalLiterCount());
    }

    vTaskDelete(NULL);
}

/********************************************************************************************
* 
********************************************************************************************/
static float fCalculateVariationInPercentage(float fMaxValue, float fMinValue, float fTargetValue)
{
    float fVariationInPercentage = 0;
    fVariationInPercentage = fabs((fMaxValue - fMinValue) / (fTargetValue)) * 100;
    return fVariationInPercentage;
}

/********************************************************************************************
* 
********************************************************************************************/
static void vInitiateSequenceSummaryStart()
{
    memset(&sequencesummary, 0, sizeof(sequenceSummary_t)); /* setting the summary to 0 */
    sequence_t xSequenceNumber;                                     /* variable for the sequence number */

   
    get_sequence_info(&xSequenceNumber, uGetCurrentRunningSequenceNumber());     /* getting the current  sequence from the sequence array */
    strcpy(sequencesummary.summary.cStartDate, xSequenceNumber.cStartDate);
    
    sequencesummary.airflowVolumetric.fAirflowSetPoint = xSequenceNumber.fFlowSetPoint; /* setting the flow set point */
    sequencesummary.summary.xVolumeCounter.fStartVolume = fGetTotalLiterCount();    /* getting the initial total lites before the starting of the sequnece */
    sequencesummary.summary.xVolumeCounter.fTargetVolume = (xSequenceNumber.fFlowSetPoint * ((xSequenceNumber.uDurationHour * 60) + xSequenceNumber.uDurationMinutes)); /* setting the target volume count */
    sequencesummary.summary.xHourCounter.fStartHour = fGetTotalHoursCount();     /* setting the starting hour counter */
    sequencesummary.summary.xHourCounter.fTargetHour = (float)((float)xSequenceNumber.uDurationHour + ((float)xSequenceNumber.uDurationMinutes / 60.00));   /* setting the target hour count */

    ESP_LOGI(TAG, "Target hour assighned is %f", sequencesummary.summary.xHourCounter.fTargetHour);
    /*setting the in range variables in the summary to be true, if as the sensor values are measured, then we can set to false, if they outside the range */
    sequencesummary.airflowVolumetric.bIsInRange = true;
    sequencesummary.ambientHumidity.bIsInRange = true;
    sequencesummary.ambientPressure.bIsInRange = true;
    sequencesummary.ambientTemperature.bIsInRange = true;
    sequencesummary.headLoss.bIsInRange = true;
}

/********************************************************************************************
* 
********************************************************************************************/
static void vCalculateSequneceEndSummary()
{
    struct tm timeinfo = {0};
    char cStopDate[40];

    get_current_date_time(&timeinfo);
    timeinfo.tm_year = timeinfo.tm_year + 1900;
    timeinfo.tm_mon = timeinfo.tm_mon + 1;
    sprintf(cStopDate, "%d/%2d/%2d", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);    
    strcpy(sequencesummary.summary.cStopDate, cStopDate);   /* copy end date */
   
    sequencesummary.summary.xVolumeCounter.fEndVolume = fGetTotalLiterCount();   /* setting  the end volume and the effective volume */
    sequencesummary.summary.xVolumeCounter.fEffectiveVolume = fGetTotalLiterCount() - sequencesummary.summary.xVolumeCounter.fStartVolume;
    sequencesummary.summary.xVolumeCounter.fVariation = fabs(((sequencesummary.summary.xVolumeCounter.fTargetVolume - sequencesummary.summary.xVolumeCounter.fEffectiveVolume) / sequencesummary.summary.xVolumeCounter.fTargetVolume) * 100);  /* calculating the variation in volume */

    sequencesummary.summary.xHourCounter.fEndHour = fGetTotalHoursCount();
    sequencesummary.summary.xHourCounter.fEffectiveHour = fGetTotalHoursCount() - sequencesummary.summary.xHourCounter.fStartHour;
    sequencesummary.summary.xHourCounter.fVariation = fabs(((sequencesummary.summary.xHourCounter.fTargetHour - sequencesummary.summary.xHourCounter.fEffectiveHour) / sequencesummary.summary.xHourCounter.fTargetHour) * 100);    /* calculating the variation in hours */

    ESP_LOGD(TAG, "Hour counter target and effective values are %.2f and %.2f", sequencesummary.summary.xHourCounter.fTargetHour, sequencesummary.summary.xHourCounter.fEffectiveHour);
    ESP_LOGD(TAG, "variation in hour is %.2f", sequencesummary.summary.xHourCounter.fVariation);

    /* calculating the variation of extenal sensor data and the air flow */
    sequencesummary.airflowVolumetric.fAirflowVariation = fCalculateVariationInPercentage(sequencesummary.airflowVolumetric.fAirflowMaxValue, sequencesummary.airflowVolumetric.fAirflowMinValue, sequencesummary.airflowVolumetric.fAirflowSetPoint);
    sequencesummary.ambientHumidity.fAmbientHumidityVariation = fCalculateVariationInPercentage(sequencesummary.ambientHumidity.fMaxAmbientHumidity, sequencesummary.ambientHumidity.fMinAmbientHumidity, sequencesummary.ambientHumidity.fMeanAmbientHumidity);
    sequencesummary.ambientPressure.fAmbientPressureVariation = fCalculateVariationInPercentage(sequencesummary.ambientPressure.fMaxAmbientPressure, sequencesummary.ambientPressure.fMinAmbientPressure, sequencesummary.ambientPressure.fMeanAmbientPressure);
    sequencesummary.ambientTemperature.fTemperatureVariation = fCalculateVariationInPercentage(sequencesummary.ambientTemperature.fMaxTemperature, sequencesummary.ambientTemperature.fMinTemperature, sequencesummary.ambientTemperature.fTemperatureVariation);
    sequencesummary.headLoss.fHeadLossVariation = fCalculateVariationInPercentage(sequencesummary.headLoss.fMaxHeadLoss, sequencesummary.headLoss.fMinHeadLoss, sequencesummary.headLoss.fMeanHeadLoss);

    /*  If all sensors values are with in range then the sequence is ok. So we can say that the sequence run sucessfully */
    if (sequencesummary.airflowVolumetric.bIsInRange && (sequencesummary.airflowVolumetric.fAirflowVariation < MAX_TOLERATION_IN_FLOW_VARIATION) &&
        sequencesummary.ambientHumidity.bIsInRange && (sequencesummary.ambientHumidity.fAmbientHumidityVariation < MAX_TOLERACE_IN_HUMIDITY) &&
        sequencesummary.ambientPressure.bIsInRange && (sequencesummary.ambientPressure.fAmbientPressureVariation < MAX_TOLERACE_IN_PRESSURE) &&
        sequencesummary.ambientTemperature.bIsInRange && (sequencesummary.ambientTemperature.fTemperatureVariation < MAX_TOLERACE_IN_TEMPERATURE) &&
        sequencesummary.headLoss.bIsInRange && (sequencesummary.headLoss.fHeadLossVariation < MAX_TOLERACE_IN_HEAD_LOSS) &&
        (sequencesummary.summary.xHourCounter.fVariation < MAX_TOLERATION_IN_HOUR_VARIATION) &&
        (sequencesummary.summary.xVolumeCounter.fVariation < MAX_TOLERATION_IN_VOLUME_VARIATION))
    {
        totalSequence[uGetCurrentRunningSequenceNumber() - 1].bSucessfullyRun = true;
    }
    else
    {
        totalSequence[uGetCurrentRunningSequenceNumber() - 1].bSucessfullyRun = false;
    }
    database_save_sequence_summary(uGetCurrentSampleNumber(), uGetCurrentRunningSequenceNumber(), sequencesummary); /*  save summary to the db */
}

/********************************************************************************************
* 
********************************************************************************************/
static void nvread_totalsequence_count(void)
{
    bool ret = nvsread_value_u8(NVS_STORGE_NAME, TOTAL_SEQUENCE_COUNT_KEY, &uTotalSequenceCount);
    if(ret == false)
    {
      ESP_LOGE(TAG, "Total sequence count read error");
    }
    else
    {
        ESP_LOGI(TAG, "Total Sequence count : %d", uTotalSequenceCount);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
static void nvsread_sequencedata(void)
{
    bool ret = nvsread_value_parameter(NVS_STORGE_NAME, STORAGE_KEY, (void *)totalSequence);
    if(ret == false)
    {
      ESP_LOGE(TAG, "Total sequence summary read error");
    }
}


/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
 
/********************************************************************************************
* 
********************************************************************************************/
void vTimerUpdateTimePassInSec(TimerHandle_t xTimer)
{
    uTotalTimePassInSeconds++;
}

/********************************************************************************************
* 
********************************************************************************************/
void set_sequence_values(uint8_t uSequenceNumber, char *pStartDate, uint8_t uStartHour, uint8_t uStartMin, float fFlowSetPoint, uint8_t uDurationHour, uint8_t uDurationMinutes, char *pStartPerson)
{
    totalSequence[uSequenceNumber - 1].uSequenceNo = uSequenceNumber;
    strcpy((char *)&totalSequence[uSequenceNumber - 1].cStartDate, pStartDate);
    totalSequence[uSequenceNumber - 1].uStartHour = uStartHour;
    totalSequence[uSequenceNumber - 1].uStartMin = uStartMin;
    totalSequence[uSequenceNumber - 1].fFlowSetPoint = fFlowSetPoint;
    totalSequence[uSequenceNumber - 1].uDurationHour = uDurationHour;
    totalSequence[uSequenceNumber - 1].uDurationMinutes = uDurationMinutes;
    totalSequence[uSequenceNumber - 1].bSucessfullyRun = false; // setting the sucessfully run state to false
    strcpy((char *)&totalSequence[uSequenceNumber - 1].cStartPerson, pStartPerson);

    print_on_terminal();
    uTotalSequenceCount++;
}

/********************************************************************************************
* 
********************************************************************************************/
void sequence_array_initialize()
{
    memset(totalSequence, 0, MAXIMUM_NO_OF_SAMPLES * sizeof(sequence_t));   /* initializind the sample arrey to 0 */
    uTotalSequenceCount = 0;    /* setting the number of samples to 0 */
}

/********************************************************************************************
* 
********************************************************************************************/
sequence_t *get_sequence_array()
{
    return totalSequence;
}

/********************************************************************************************
* 
********************************************************************************************/
void nvswrite_sequence_array(void)
{
    bool ret = nvswrite_value_parameters(NVS_STORGE_NAME, STORAGE_KEY, (void *)totalSequence, sizeof(totalSequence));
    if(ret == false)
    {
      ESP_LOGE(TAG, "Total sequence summary write error");
    }
}


/********************************************************************************************
* 
********************************************************************************************/
uint8_t get_no_of_sequence_in_array(void)
{
    return uTotalSequenceCount;
}

/********************************************************************************************
* 
********************************************************************************************/
sequence_t *get_sequencedata(uint8_t uSequenceNumber)
{
    return &totalSequence[uSequenceNumber - 1];
}

/********************************************************************************************
* 
********************************************************************************************/
void taskRunSample(void *pvParameters)
{
    uint8_t usequenceToRun;

    if (xStopTheRunningSequenceSemaphore == NULL)   /* creating semaphore for stopping the task if, the semaphore handle is NULL */
    {   
        xStopTheRunningSequenceSemaphore = xSemaphoreCreateBinary();
    }
    else
    {
        vSemaphoreDelete(xStopTheRunningSequenceSemaphore);
        xStopTheRunningSequenceSemaphore = xSemaphoreCreateBinary();
    }
   
    memcpy(&usequenceToRun, pvParameters, sizeof(uint8_t));
    ESP_LOGD(TAG, "Running sequence %d", usequenceToRun);

    if (xTaskHandleMonitorSensorData == NULL)    /* starting the monitor data task */
    {
        xTaskCreate(vMonitorSensorDataTask, "monitor data", 8 * 1024, NULL, 5, &xTaskHandleMonitorSensorData);
    }
    else
    {
        ESP_LOGW(TAG, "monitor task handle is not null, it should be null here.Not creating the monitor task");
    }

    vInitiateSequenceSummaryStart();     /* calculating and saving the end summary of the variables */
    bDeleteUpdateScreenAndVaulesTask = false;   /* setting the delete the update value task to false so that task does not deleted when created */
    vShowWorkInProgressScreen();    /*  showing the work in progress screen */
    
    /* creating the task which updates the screen values and saving the values to the nvs flash */
    xTaskCreate(vUpdateScreenAndSaveValuesEverySecond, "updateScreen", 4 * 1024, &usequenceToRun, 5, &xTaskHandleUpdateScreenAndSaveValuesEverySecond);
    
    setMotorPIDSetTargetValue(totalSequence[usequenceToRun - 1].fFlowSetPoint);     /*  seting the flow set point */
    MotorPWMStart(motorPID_DEFAULT_ENTRY_POINT);    /*  starting the motor so we have some initial value for sdp sensor and calculate flowrate and the pid controller can compute  */
    setStateOfMotor(true);                      // run the motor
    ESP_LOGD(TAG, "Starting the motor");
    ESP_LOGD(TAG, "get state of motor %d", getIsMotorRunning());
    uint32_t udurationInMs = ((totalSequence[usequenceToRun - 1].uDurationHour * 3600) + (totalSequence[usequenceToRun - 1].uDurationMinutes * 60)) * 1000;
    ESP_LOGD(TAG, "Duration in ms %d", udurationInMs);

    if (xSemaphoreTake(xStopTheRunningSequenceSemaphore, (udurationInMs / portTICK_PERIOD_MS)) == pdTRUE)
    {
        ESP_LOGW(TAG, "Sequence has been terminated early, due to the force stop");
    }
    else
    {
        ESP_LOGI(TAG, "Sequence has been terminated due to expiry of time");
    }

    stop_current_sequence(); /* now sequence end is reached now stopping the current sequnce and notify the sample management to carry on the task  */
}

/********************************************************************************************
* 
********************************************************************************************/
int32_t get_reamining_time_start_sequence(uint8_t uSequenceNumber)
{
    uint8_t uStartHour = totalSequence[uSequenceNumber - 1].uStartHour;
    uint8_t uStartMin = totalSequence[uSequenceNumber - 1].uStartMin;
    struct tm timeinfo = {0};

    strptime((char *)&totalSequence[uSequenceNumber - 1].cStartDate, "%Y/%m/%e", &timeinfo);
    timeinfo.tm_hour = uStartHour;
    timeinfo.tm_min = uStartMin;
    timeinfo.tm_sec = 0;
  
    struct tm now = {0};
    get_current_date_time(&now);     /* getting the current time of the system */
    int32_t seconds = difftime(mktime(&timeinfo), mktime(&now));
    ESP_LOGD(TAG, "Seconds remaining is %d", seconds);
    return seconds;
}

/********************************************************************************************
* 
********************************************************************************************/
void set_sequence_running(uint8_t *sequenceindex)
{
    ESP_LOGI(TAG, "setting sequence to run %d", *sequenceindex);
    if (*sequenceindex == 0)
        return;
    xTaskCreate(taskRunSample, "taskRunSample", 8 * 1024, (void *)sequenceindex, 5, &xTaskHandleRunSequnceTask);
}

uint8_t get_sequence_number_tosave(void)
{
    return uTotalSequenceCount + 1;
}



/********************************************************************************************
* 
********************************************************************************************/
void nvswrite_totalsequence_count(void)
{
    bool ret = nvswrite_value_u8(NVS_STORGE_NAME, TOTAL_SEQUENCE_COUNT_KEY, uTotalSequenceCount);
    if(ret == false)
    {
      ESP_LOGE(TAG, "Total sequence count write error");
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void nvsread_sequence_parameters()
{
    nvread_totalsequence_count();
    nvsread_sequencedata();
}


/********************************************************************************************
* 
********************************************************************************************/
void get_sequence_info(sequence_t *sequencebuf, uint8_t index)
{
    memcpy(sequencebuf, &totalSequence[index - 1], sizeof(sequence_t));
}

/********************************************************************************************
* 
********************************************************************************************/
void stop_current_sequence(void)
{
    ESP_LOGI(TAG, "Now stopping the current sequence");
    setStateOfMotor(false); /* turning off the motor */
    bDeleteUpdateScreenAndVaulesTask = true;    /* deleting the task which update the values on the screen and saving to the flash */
    if (xTaskHandleMonitorSensorData)   /* stopping the monitor task */
    {
        ESP_LOGI(TAG, "Deleting the moniotr task");
        vTaskDelete(xTaskHandleMonitorSensorData);
        xTaskHandleMonitorSensorData = NULL;
    }

    vCalculateSequneceEndSummary();     /* calculating the end summary of the sequence */
    vTaskDelay(pdMS_TO_TICKS(1500));    /* giving the time to stop the ongoing task to finish */

    vNotifySampleMangementToProceed();  /* Notify the sample task about the completion of the sequence */
    vTaskDelay(pdMS_TO_TICKS(50));
    if (xTaskHandleRunSequnceTask != NULL)
    {
        TaskHandle_t xHandle = xTaskHandleRunSequnceTask;
        xTaskHandleRunSequnceTask = NULL;
        ESP_LOGI(TAG, "Deleting the Sequence Task");
        vTaskDelete(xHandle);
    }
}

/********************************************************************************************
* 
********************************************************************************************/
void stop_running_sequence(void)
{
    xSemaphoreGive(xStopTheRunningSequenceSemaphore);   /* Giving the semaphore for stopping the sequence */
}

/********************************************************************************************
* 
********************************************************************************************/
bool is_sequence_running(void)
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