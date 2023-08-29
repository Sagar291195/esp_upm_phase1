/**
 * @file controller.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the implementaton of the controller api
 * @version 1.1
 * @date 2022-05-10
 *
 * @copyright Copyright (c) 2022
 *
 */
/************************************Inlcude*********************************************************/
#include <controller.h>
#include <esp_log.h>
#include <middlewareTest.h>
#include <motor.h>
#include <sampleManagement.h>
#include <sequenceManagement.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/************************************defines******************************************************/

#define TAG "controller"

extern SemaphoreHandle_t xGuiSemaphore1;

/******************************** prototypes ******************************************************/

/**
 * @brief this function start the job
 *
 */
void vStartJob();

/*****************************function*********************************/
void vSetPIDParameters(float fKp, float fKi, float fKd, float fAkp, float fAki, float fAkd, float fNcoff, float fACoff)
{
    ESP_LOGI(TAG, "kp %0.2f, ki %0.2f, kd %0.2f, akp %0.2f, aki %0.2f, akd %0.2f, ncoff %0.2f, acoff %0.2f", fKp, fKi, fKd, fAkp, fAki, fAkd, fNcoff, fACoff);
    struct_PID_parameters_t PID_parameters;
    PID_parameters.fKp = fKp;
    PID_parameters.fKi = fKi;
    PID_parameters.fKd = fKd;
    PID_parameters.fAkp = fAkp;
    PID_parameters.fAki = fAki;
    PID_parameters.fAkd = fAkd;
    PID_parameters.fNcoff = fNcoff;
    PID_parameters.fACoff = fACoff;
    if (fNcoff == 0)
    {
        fNcoff = 1;
    }

    if (fACoff == 0)
    {
        fACoff = 1;
    }

    /**
     * @brief setting the pid parameters to the nvs flash
     *
     */
    vSetPIDParametersToNvs(&PID_parameters);

    /**
     * @brief also setting the pid parameters to the pid control block
     *
     */
    setMotorPIDParameters(fKp / fNcoff, fKi / fNcoff, fKd / fNcoff, fAkp / fACoff, fAki / fACoff, fAkd / fACoff);
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
    vSetTotalSequenceCountFromNvs();

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
    vStopCurrentSample();
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