/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/

#include "esp_upm.h"
#include "lvgl_helpers.h"
#include <timeManagement.h>
#include <sensorManagement.h>
#include <sampleManagement.h>
#include "gui/screens/screen_includes.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                 "MAIN"
#define LV_TICK_PERIOD_MS   1

#ifdef CONFIG_NEW_HW
#define WAKEMODE            GPIO_NUM_27     /* wakeup GPIO new hardware*/
#else
#define WAKEMODE            GPIO_NUM_32     /* wakeup GPIO old hardware*/
#endif


/********************************************************************************************
 *                              VARIABLES
 ********************************************************************************************/
uint8_t screenid;                   /* variable to store current loaded screen id*/
SemaphoreHandle_t i2c_communication_semaphore;    /* semaphore to Processes*/ 
SemaphoreHandle_t gui_update_semaphore;   /* semaphore to GUId*/ 

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void IRAM_ATTR lv_tick_task(void *arg);      /* lvgl task to count the ticks */
static void IRAM_ATTR screen_timeout_handler(void *arg);
static void guiTask(void *pvParameter);             /* This it the lvgl task */
static void create_demo_application(void);          /* This function intiate the first screen to show */
static void wakeupmodeInit(void);                   /* This function wakeup the screen*/

/********************************************************************************************
 *                              CODE
 ********************************************************************************************/

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void create_demo_application(void)
{
    Init_Screen();
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void IRAM_ATTR lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_RATE_MS);
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void IRAM_ATTR screen_timeout_handler(void *arg)
{
    static uint32_t idle_minute_counter = 0;
    static uint32_t savedtouchcount = 0;

    if(devicesettings.screen_sleepmode_enable == 1 )
    {
        ESP_LOGI(TAG, "screen timeout handler idle count : %d, %d, %d, %d, %d", idle_minute_counter, get_touchcount(),
                         savedtouchcount, get_lcdsleep_status(), dashboardflg);
        if((get_touchcount() == savedtouchcount) && (get_lcdsleep_status() == false) && (dashboardflg != 1))
        {
            idle_minute_counter++;
            if(idle_minute_counter == 2)//devicesettings.screen_timeout_value)
            {
                savedtouchcount = 0;
				reset_touchcount();
                lcd_set_sleep();
            }
            else if( idle_minute_counter == 1)
            {
                savedtouchcount = 0;
                reset_touchcount();
            }
        }
        else{
            savedtouchcount = get_touchcount();
            idle_minute_counter = 0;
        }
    }
    else
    {
        idle_minute_counter = 0;
    }
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void wakeupmodeInit(void)
{
    gpio_pad_select_gpio(WAKEMODE);                 // Set GPIO as OUTPUT
    gpio_set_direction(WAKEMODE, GPIO_MODE_OUTPUT); // WakeMode
    gpio_set_level(WAKEMODE, 0);
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
void app_main()
{
    printf("\n\n####################################################################################\n");
    ESP_LOGI(TAG, "Firmware Version : %s", FIRMWARE_VERSION);
    esp_err_t err = nvs_flash_init(); // Initializing the nvs for save and retriving the data
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "nvs cannot be initialized due to %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "nvs initialize successfully");
    }

    i2c_communication_semaphore = xSemaphoreCreateMutex();
    gui_update_semaphore = xSemaphoreCreateMutex();
    wakeupmodeInit();                           // enabling the device from the wake mode
    buzzer_initialization();                // This will initiate the buzze in the system
    ESP_ERROR_CHECK(i2cdev_init());
    vTaskDelay(500 / portTICK_PERIOD_MS);
    nvs_storage_initialize();               // Initiating the data managament api
    nvsread_device_settings();              //Read device settings from flash
    nvsread_calibrationdata();              // Read calibration data from flash
    nvsread_hours_liters_value();
    nvsread_sequence_parameters();
    sensor_initialization();                // Initiating all i2c sensors on the board
    start_samplemanagement_service();       // Installing the sample management service
    motor_initialization();                 // Installing the sample management service
    vTaskDelay(500 / portTICK_PERIOD_MS);
    

    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 4, NULL, 1, NULL, 1); // 0 LCD +Touch
    vTaskDelay(500 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(ds3231_task, "ds3231_task", 2048, NULL, 1, NULL, 1); //  RTS
    xTaskCreatePinnedToCore(ws2812_task, "ws2812_task", 4096, NULL, 1, NULL, 1); // 0 /Leg
    xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 4096, NULL, 1, NULL, 1); // 0 //Bizzer
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void guiTask(void *pvParameter)
{

    (void)pvParameter;

    lv_init();
    lvgl_driver_init();                 /* Initialize SPI or I2C bus used by the drivers */
    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t *buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
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
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read; // function process touch
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;

    const esp_timer_create_args_t screen_timeout_timer_args = {
        .callback = &screen_timeout_handler,
        .name = "screen_timer"};
    esp_timer_handle_t screen_timeout_timer;

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 100));

    ESP_ERROR_CHECK(esp_timer_create(&screen_timeout_timer_args, &screen_timeout_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(screen_timeout_timer, 20*1000*1000));

    lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
    create_demo_application();      /* Create the demo application */

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));           /* Delay 10ms tick assumes FreeRTOS tick is 10ms */
        if (pdTRUE == xSemaphoreTake(gui_update_semaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(gui_update_semaphore);
        }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}
