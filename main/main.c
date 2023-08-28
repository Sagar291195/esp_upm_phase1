/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "nvs.h"
#include <motor.h>
#include <espupm_tasks.h>
#include <timeManagement.h>
#include <stateMachine.h>
#include <nvs_flash.h>
#include <dataMangement.h>
#include <sensorManagement.h>
#include <calibration.h>


#include "esp_timer.h" // added this to check High resolution timer

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "lvgl_helpers.h"
#include "../components/lv_examples/lv_examples/src/ESPUPM_SIM_01/GUI/Screens/screen_includes.h"

#include "espupm_tasks.h"
#include "middlewareTest.h"
#include <sampleManagement.h>
#include "esp_upm_sensors.h"
#include <calibration.h>


/*********************
 *      DEFINES
 *********************/
#define TAG                 "Demo"
#define LV_TICK_PERIOD_MS   1
// #define WAKEMODE 32

#ifdef CONFIG_NEW_HW
#define WAKEMODE   GPIO_NUM_27
#else
#define WAKEMODE   GPIO_NUM_32
#endif

int result1 = 1;
int result2 = 1;

float KpVal;
float KiVal;
float KdVal;

int Count12 = 0;
int64_t C21 = 0;
int revCountDefJobStart;

int TotalliterInt;
int TotalLiterFloat;
int sampleNumber;
double total_liters1;
float totalhourVal;
long totalHourInt;
int totalHourFloat;
float StartHrEND;
bool navier_set_time;

float FlowPloyCoffA;
float FlowPloyCoffB;
float FlowPloyCoffC;
float FlowPloyCoffD;
float FlowPloyCoffE;
float FlowPloyCoffF;
float FlowPloyCoffG;
float FlowPloyCoffH;
float FlowPloyCoffI;
float FlowPloyCoffJ;
int FlowPolyDeg;

float kp, ki, kd;

int minutes_left, minute_result, hour_result, day_result, mon_result, year_result;
int second = 0, last_current, day_count, count_case = 1, minute_duration;

char guiDate[40];

float *totalhourPtr;

float pidCoffofCoff = 10.0;
uint8_t screenid;

SemaphoreHandle_t xGuiSemaphore;  // semaphore to Processes
SemaphoreHandle_t xGuiSemaphore1; // semaphore to GUId

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**
 * @brief lvgl task to count the ticks
 *
 * @param arg
 */
static void IRAM_ATTR lv_tick_task(void *arg);
/**
 * @brief this it the lvgl task
 *
 * @param pvParameter
 */
static void guiTask(void *pvParameter);

/**
 * @brief This function intiate the first screen to show
 *
 */
static void create_demo_application(void);

/**
 * @brief This function wakeup the screen
 *
 */
static void wakeupmodeInit(void);


/**********************
 * CODE
 **********************/
static void create_demo_application(void)
{
    Init_Screen();
}

static void IRAM_ATTR lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_RATE_MS);
}

static void wakeupmodeInit(void)
{
    // Set GPIO as OUTPUT
    gpio_pad_select_gpio(WAKEMODE);
    gpio_set_direction(WAKEMODE, GPIO_MODE_OUTPUT); // WakeMode
    gpio_set_level(WAKEMODE, 0);
}

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main()
{
    esp_err_t err = nvs_flash_init();     //Initializing the nvs for save and retriving the data
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }else if(err != ESP_OK){
        ESP_LOGI(TAG, "Nvs cannot be initialized due to %s", esp_err_to_name(err));
    }else{
        ESP_LOGD(TAG, "Nvs initialized");
    }

    xGuiSemaphore = xSemaphoreCreateMutex();
    xGuiSemaphore1 = xSemaphoreCreateMutex();

    wakeupmodeInit();                   //enabling the device from the wake mode
    vInitiateTheStateMachine();         //initaiting the state machine of the device
    Init_Buzzer();                      //This will initiate the buzze in the system
    ESP_ERROR_CHECK(i2cdev_init());
    vTaskDelay(500 / portTICK_PERIOD_MS);
    vInitializeDataManagementApi();     //Initiating the data managament api
    nvsread_calibrationdata();          //Read calibration data from flash
    vInitiateSensorsOnBoard();          //Initiating all i2c sensors on the board
    vGetTheCounterValuesFromNvsFlash(); //loading the various conter values from the nvs flash
    vGetSequceManagementFromNVS();      //Before loading  sample management we need to load the sequence of the sample
    vInitializeTimeManagement();        //Initializing the time management of the device
    vStartSampleManagementService();    //Installing the sample management service
    vInitializeMotor();                 //Installing the sample management service
    
    vTaskDelay(500 / portTICK_PERIOD_MS);
    // readTotalLiters();                //Reading the tolal volume in the system from nvs flash

    ESP_LOGI(TAG, "Code Version: ESPUPM 8-04-2022 V2");

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0
     * */

    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 4, NULL, 1, NULL, 1); // 0 LCD +Touch
    vTaskDelay(500 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(ds3231_task, "ds3231_task", 2048, NULL, 1, NULL, 1); //  RTS

    xTaskCreatePinnedToCore(ws2812_task, "ws2812_task", 4096, NULL, 1, NULL, 1); // 0 /Leg
    xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 4096, NULL, 1, NULL, 1); // 0 //Bizzer
}

/* Creates a semaphore to handle concurrent call to lvgl stuff*****
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! ********************
 *
 * */
static void guiTask(void *pvParameter)
{

    (void)pvParameter;

    lv_init();
    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();
    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    // static lv_color_t buf1[LV_HOR_RES_MAX * 40];
    assert(buf1 != NULL);
    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t *buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    // static lv_color_t buf2[LV_HOR_RES_MAX * 40];
    assert(buf2 != NULL);
#else
    static lv_color_t *buf2 = NULL;
#endif

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;
    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /* Register an input device when enabled on the menuconfig */
    // #if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read; // function process touch
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
    //#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 100));

    lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
    /* Create the demo application */
    create_demo_application();

    while (1)
    {
        /* Delay 10ms tick assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(1));
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore1, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore1);
        }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}




