/**
 * @file timeManagement.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file contains the declaration of the time management class. Here we manage the time of the system. Time related function are avaliable in this class.
 * @version 1.1
 * @date 2022-05-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __TIMEMANAGEMENT_H__
#define __TIMEMANAGEMENT_H__

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <esp_app_format.h>


/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/**
 * @brief Set the system date and time to the given time. Front end should give the structure like that
 * @code struct tm navier_time_set 
 * @param time time to be set
 */
void set_time_date_device(struct tm time);

/**
 * @brief Get the current date and time of the system and return it in the given structure
 * @code  year_Roller_int = tm.tm_year;
* @param time[in] pointer to the structure to which the current time is to be returned
 */
void get_current_date_time(struct tm *time);

/**
 * @brief initialize the time management and set the system time to complile time of the system
 *
 */
void timemanagement_intialization();

/**
 * @brief get the end date and itme of the sequence in the given array
 * @param endDate array pointer of end date
 * @param sequenceNumber sequence number of the sequence
 * @param sizeOfEndDate size of end date array max 255
 * @param endTime pointer to array of end time
 * @param sizeOfEndTimes size of end time array max 255
 */
void get_end_date_time_sequence(char *endDate, uint8_t sequenceNumber, uint8_t sizeOfEndDate,char *endTime,uint8_t sizeOfEndTimes);


/**
 * @brief Get the start date and time of the sequence in the given array in string format
 * @param[in] uSequenceNumber Sequnce numbet to which date and time has been determine
 * @param[out] cStartDate  array pointer of start date
 * @param[in] sizeOfStartDate size of start date array
 * @param[out] starttime start time array pointer
 * @param[in] uSizeOfStartTime size of start time array
 */
void get_start_date_time_sequence(uint8_t uSequenceNumber, char *cStartDate, uint8_t sizeOfStartDate, char *cStartTime, uint8_t uSizeOfStartTime);

/**
 * @brief Get the total run time  of the sample  in given format like 15H 45M
 * @param[out] cTotalDuration place holder to put the string
 * @param[in] uSizeOfArray size of the array
 */
void get_total_duration_sample(char* cTotalDuration, uint8_t uSizeOfArray);

/**
 * @brief measure the number of hours and the minutes left in the sequence
 *
 * @param[out] hour number of hours left in the sequence
 * @param[out] minutes number of minutes left in the sequence
 */
void get_remaining_hours_minutes_sequence(uint16_t *hour, uint8_t *minutes);

/**
 * @brief intitiate the RTC sensor on board
 *
 */
void rtc_sensor_initialize();

#endif // __TIMEMANAGEMENT_H__