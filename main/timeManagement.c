/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
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
void rtc_sensor_initialize()
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
void set_time_date_device(struct tm time)
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
    set_time_date_device(tm);
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
void get_end_date_time_sequence(char *startdate, char *endDate, uint8_t sequenceNumber, uint8_t sizeOfEndDate, char *endTime, uint8_t sizeOfEndTimes)
{
    struct tm stopTimeTm = {0};     /* converting the time_t format into the struct tm */
    struct tm startTimeTm = {0};
    sequence_t *seq;
    time_t startTimeT;
    uint32_t year;
    uint32_t month;
    uint32_t date;

    sscanf(startdate, "%4d/%2d/%2d", &year, &month, &date);
    sequenceNumber--;
    seq = get_sequence_array();

    startTimeTm.tm_year = year-1900;
    startTimeTm.tm_mon = month;
    startTimeTm.tm_mday = date;
    startTimeTm.tm_hour = seq[0].uStartHour;
    startTimeTm.tm_min = seq[0].uStartMin;

    ESP_LOGD(TAG, "Start date = %s time = %d %d", startdate, startTimeTm.tm_hour, startTimeTm.tm_min);
    startTimeT = mktime(&startTimeTm); // converting the time into the time_t format epoch time   
    startTimeT = startTimeT + (seq[0].uDurationHour * 3600) + (seq[0].uDurationMinutes * 60);   /* now adding the duration in second to get the new time */

    gmtime_r(&startTimeT, &stopTimeTm);
    sprintf(endDate, "%4d/%2d/%2d", (1900+stopTimeTm.tm_year), stopTimeTm.tm_mon, stopTimeTm.tm_mday);
    sprintf(endTime, "%2dH %2dM", stopTimeTm.tm_hour, stopTimeTm.tm_min);
    ESP_LOGD(TAG, "End Date = %s, End Time = %s", endDate, endTime);
}

/********************************************************************************************
*                          
********************************************************************************************/
void get_start_date_time_sequence(uint8_t uSequenceNumber, char *cStartDate, uint8_t sizeOfStartDate, char *cStartTime, uint8_t uSizeOfStartTime)
{
    sequence_t *seq = get_sequence_array();
    sprintf(cStartDate, "%s", seq[uSequenceNumber - 1].cStartDate);
    sprintf(cStartTime, "%02d:%02d:00", seq[uSequenceNumber - 1].uStartHour, seq[uSequenceNumber - 1].uStartMin);
}

/********************************************************************************************
*                          
********************************************************************************************/
void get_total_duration_sample(char *cTotalDuration, uint8_t uSizeOfArray)
{
    uint16_t uTotalDurationMinutes = 0;                     /* iterating over the sequence array to get the total minutes of duration */
    sequence_t *seq = get_sequence_array();         /* getting the pointer to the sequence array  */
    
    for (uint8_t i = 0; i < get_no_of_sequence_in_array(); i++)
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
void get_remaining_hours_minutes_sequence(uint16_t *hour, uint8_t *minutes)
{
    sequence_t *seq = get_sequence_array();
    uint32_t uTotalDurationSecondsLeft = 0;

    uTotalDurationSecondsLeft = seq[uGetCurrentRunningSequenceNumber() - 1].uDurationHour * 3600 + seq[uGetCurrentRunningSequenceNumber() - 1].uDurationMinutes * 60;
    uTotalDurationSecondsLeft = uTotalDurationSecondsLeft - uGetTotalSecondPassesInGivenSequence();  /* subtracting the number of seconds already elapsed  in the running sequnence */
    ESP_LOGV(TAG, "uTotalDurationSecondsLeft %d", uTotalDurationSecondsLeft);
    *hour = uTotalDurationSecondsLeft / 3600;   /* getting the number of hours and minutes left */
    *minutes = (uTotalDurationSecondsLeft % 3600) / 60;
    ESP_LOGD(TAG, "%d hours and %d minutes left", *hour, *minutes);
}