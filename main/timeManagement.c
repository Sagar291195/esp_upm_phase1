/**
 * @file timeManagement.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief Create the implementation of the time related function in the system. This function also initialize the rtc time function
 * @version 1.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <timeManagement.h>

#include <esp_log.h>
#include <time.h>
#include <i2cdev.h>
#include <string.h>
#include <freertos/semphr.h>
#include <nvs_flash.h>
#include <esp_ota_ops.h>
#include <sequenceManagement.h>
#include <sampleManagement.h>

#include "esp_upm.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG         "timeManagement"
#define STORAGE     "timeManagement"
#define SDA_GPIO    21  /* i2c sda pin */
#define SCL_GPIO    22  /* i2c scl pin */

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern SemaphoreHandle_t i2c_communication_semaphore;

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
i2c_dev_t dev;

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
 
/********************************************************************************************
*                          
********************************************************************************************/
void vInitiateRTCSensor()
{
    ESP_LOGI(TAG, "Initializing RTC Sensor");

    memset(&dev, 0, sizeof(i2c_dev_t));
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
        xSemaphoreGive(i2c_communication_semaphore);
    }
}

/********************************************************************************************
*                          
********************************************************************************************/
void vSetDateAndTimeOfTheDevice(struct tm time)
{
    time.tm_year = time.tm_year - 1900; // adjusting the year
    time.tm_mon = time.tm_mon - 1;      // adjusting the month

    ESP_LOGD(TAG, "Setting time to %d-%d-%d %d:%d:%d", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK(ds3231_set_time(&dev, &time));
        xSemaphoreGive(i2c_communication_semaphore);
    }
}

/********************************************************************************************
*                          
********************************************************************************************/
void vGetCurrentDateAndTime(struct tm *time)
{
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK(ds3231_get_time(&dev, time));
        xSemaphoreGive(i2c_communication_semaphore);
    }
    time->tm_year = time->tm_year; // adjusting the year
    time->tm_mon = time->tm_mon; // adjusting the month
}

/********************************************************************************************
*                          
********************************************************************************************/
void vSetSystemTimeUsingCompileTime()
{
    struct tm tm = {0};

    char now[255];
    memset(now, 0, 255);
    strcat(now, __DATE__); // copying the date
    strcat(now, " ");
    strcat(now, __TIME__); // copying the time
    ESP_LOGI(TAG, "%s", now);

    /* converting the time to the tm structure */
    strptime(now, "%b %e %Y %H:%M:%S", &tm);
    tm.tm_year = tm.tm_year + 1900; // adjusting the year
    tm.tm_mon = tm.tm_mon + 1;
    ESP_LOGD(TAG, "setting the system time");
    vSetDateAndTimeOfTheDevice(tm);

    struct tm currentTime = {0};
    vGetCurrentDateAndTime(&currentTime);
    strftime(now, sizeof(now), "%d %b %Y %H:%M", &currentTime);
    ESP_LOGI(TAG, "Current time is %s", now);
}

/********************************************************************************************
*                          
********************************************************************************************/
void timemanagement_intialization()
{
    ESP_LOGI(TAG, "Initializing time management");
    vSetSystemTimeUsingCompileTime();
}

/********************************************************************************************
*                          
********************************************************************************************/
void vGetEndDateAndTimeOfSequence(char *endDate, uint8_t sequenceNumber, uint8_t sizeOfEndDate, char *endTime, uint8_t sizeOfEndTimes)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    sequenceNumber--;

    sequence_t *seq = pGetAddressOfSequenceArray();
    struct tm startTimeTm = {0};
    startTimeTm.tm_hour = seq[0].uStartHour;
    startTimeTm.tm_min = seq[0].uStartMin;

    time_t startTimeT = mktime(&startTimeTm); // converting the time into the time_t format epoch time
    ESP_LOGD(TAG, "startTimeT %lu", startTimeT);
    
    startTimeT = startTimeT + (seq[0].uDurationHour * 3600) + (seq[0].uDurationMinutes * 60);   /* now adding the duration in second to get the new time */
    ESP_LOGD(TAG, "startTimeT %lu", startTimeT);

   
    struct tm stopTimeTm = {0};     /* converting the time_t format into the struct tm */\
    gmtime_r(&startTimeT, &stopTimeTm);

    ESP_LOGD(TAG, "stop year is %d", stopTimeTm.tm_year);
    ESP_LOGD(TAG, "stop month is %d", stopTimeTm.tm_mon);
    ESP_LOGD(TAG, "stop day is %d", stopTimeTm.tm_mday);
    ESP_LOGD(TAG, "stop hour is %d", stopTimeTm.tm_hour);
    ESP_LOGD(TAG, "stop min is %d", stopTimeTm.tm_min);

    strftime(endDate, sizeOfEndDate, "%Y/%m/%d", &stopTimeTm);
    ESP_LOGD(TAG, "End date of sequence %d is %s", sequenceNumber, endDate);
    strftime(endTime, sizeOfEndTimes, "%HH %MM", &stopTimeTm);
    ESP_LOGD(TAG, "End time of sequence %d is %s", sequenceNumber, endTime);
}

/********************************************************************************************
*                          
********************************************************************************************/
void uGetStartSequenceDateAndTime(uint8_t uSequenceNumber, char *cStartDate, uint8_t sizeOfStartDate, char *cStartTime, uint8_t uSizeOfStartTime)
{
    sequence_t *seq = pGetAddressOfSequenceArray();
    sprintf(cStartDate, "%s", seq[uSequenceNumber - 1].cStartDate);
    sprintf(cStartTime, "%02d:%02d:00", seq[uSequenceNumber - 1].uStartHour, seq[uSequenceNumber - 1].uStartMin);
}

/********************************************************************************************
*                          
********************************************************************************************/
void vGetTotalDuartionOfSample(char *cTotalDuration, uint8_t uSizeOfArray)
{
    uint16_t uTotalDurationMinutes = 0;                     /* iterating over the sequence array to get the total minutes of duration */
    sequence_t *seq = pGetAddressOfSequenceArray();         /* getting the pointer to the sequence array  */
    
    for (uint8_t i = 0; i < uGetNoOfSequenceInArray(); i++)
    {
        uTotalDurationMinutes += seq[i].uDurationHour * 60 + seq[i].uDurationMinutes;
    }

    uint16_t uTotalDurationHours = uTotalDurationMinutes / 60;   /* converting the minutes into hours and minutes */
    uint8_t uTotalDurationMinutesRemaining = uTotalDurationMinutes % 60;
    ESP_LOGD(TAG, "Total duration is %d hours and %d minutes", uTotalDurationHours, uTotalDurationMinutesRemaining);
    sprintf(cTotalDuration, "%dH %02dM", uTotalDurationHours, uTotalDurationMinutesRemaining);  /* converting the hours and minutes into a string */
}

/********************************************************************************************
*                          
********************************************************************************************/
void vGetNumberOfHourAndMinutesLeftInStartingSequence(uint16_t *hour, uint8_t *minutes)
{
    sequence_t *seq = pGetAddressOfSequenceArray();
    uint32_t uTotalDurationSecondsLeft = 0;

    uTotalDurationSecondsLeft = seq[uGetCurrentRunningSequenceNumber() - 1].uDurationHour * 3600 + seq[uGetCurrentRunningSequenceNumber() - 1].uDurationMinutes * 60;
    uTotalDurationSecondsLeft = uTotalDurationSecondsLeft - uGetTotalSecondPassesInGivenSequence();  /* subtracting the number of seconds already elapsed  in the running sequnence */
    ESP_LOGV(TAG, "uTotalDurationSecondsLeft %d", uTotalDurationSecondsLeft);
    *hour = uTotalDurationSecondsLeft / 3600;   /* getting the number of hours and minutes left */
    *minutes = (uTotalDurationSecondsLeft % 3600) / 60;
    ESP_LOGD(TAG, "%d hours and %d minutes left", *hour, *minutes);
}