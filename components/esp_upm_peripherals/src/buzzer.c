/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
 *  @file
 *  @brief
 *  @details
 */

/*********************
 *      INCLUDES
 *********************/

#include "buzzer.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

// 1.ready_mode:
// 2.problem_mode:
// 3.metrology_in_progress_mode:
// 4.work_in_progress_mode:
// 5.wait_mode:
// 6.job_finished_mode:
// 7.alert_service_mode:
// 8.metrology_needed_mode:
// 9.export_data_mode:

// Event Wise diffrent beep profile

// 		Modes												Buzzer Sound Profile
// ===============================						================================
// 1.READY_MODE: 												Short Beep (When switch to this mode)
// 2.PROBLEM_MODE:												Two Long Beep (When switch to this mode)
// 3.METROLOGY_IN_PROGRESS_MODE:
// 4.WORK_IN_PROGRESS_MODE:									Short Beep (When switch to this mode)
// 5.WAIT_MODE:
// 6.JOB_FINISHED_MODE:										Very Long Beep (When switch to this mode)
// 7.ALERT_SERVICE_MODE:										5 times short beep	(When switch to this mode)
// 8.METROLOGY_NEEDED_MODE:                             		5 times short beep   (When switch to this mode)
// 9.EXPORT_DATA_MODE:											One Short beep      (When switch to this mode)

void readyModeBuzzBeep(void)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
    vTaskDelay(300);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
}

void workInProgressBuzzBeep(void)
{
    for (int i = 0; i < 6; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}

void jobFinishedModeBuzzBeep(void)
{
    for (int i = 0; i < 4; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(1000);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}

void waitModeBuzzBeep(void)
{
    for (int i = 0; i < 3; i++)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(400);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
        vTaskDelay(100);
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
