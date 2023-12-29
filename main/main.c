/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_upm.h"
#include "lvgl_helpers.h"
#include <timeManagement.h>
#include <sensorManagement.h>
#include <sampleManagement.h>
#include "gui/screens/screen_includes.h"

#include "fw_update/ota.h"
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
static device_state_t device_state;
/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void IRAM_ATTR lv_tick_task(void *arg);      /* lvgl task to count the ticks */
static void guiTask(void *pvParameter);             /* This it the lvgl task */
static void create_demo_application(void);          /* This function intiate the first screen to show */
static void wakeupmodeInit(void);                   /* This function wakeup the screen*/
static uint8_t get_device_operating_mode( void );
static uint8_t get_firmware_update_error( void );

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
    wakeupmodeInit();                   // enabling the device from the wake mode
    buzzer_initialization();              // This will initiate the buzze in the system
    ESP_ERROR_CHECK(i2cdev_init());
    vTaskDelay(500 / portTICK_PERIOD_MS);

    nvs_storage_initialize();     // Initiating the data managament api
    uint8_t devicemode = get_device_operating_mode();
    
    switch(devicemode)
    {
        case DO_FIRMWARE_UPDATE:
            /*execute firmware update task here*/
            ESP_LOGI(TAG, "device is in firmware update state");
            xTaskCreatePinnedToCore(ota_task, "ota_task", 8192, NULL, 1, NULL, 1);
            break;

        case FIRMWARE_UPDATE_ERROR:
            /* start gui and display error message or warning */
            ESP_LOGI(TAG, "device is in firmware update error state");
            xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 4, NULL, 1, NULL, 1); // 0 LCD +Touch
            vTaskDelay(500 / portTICK_PERIOD_MS);
            break;

        case NORMAL_MODE:
            ESP_LOGI(TAG, "device is in normal state");
            nvsread_calibrationdata();          // Read calibration data from flash
            nvsread_hours_liters_value();
            nvsread_sequence_parameters();
            sensor_initialization();          // Initiating all i2c sensors on the board
            start_samplemanagement_service();    // Installing the sample management service
            motor_initialization();                 // Installing the sample management service
            vTaskDelay(500 / portTICK_PERIOD_MS);
            
            xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 4, NULL, 1, NULL, 1); // 0 LCD +Touch
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xTaskCreatePinnedToCore(ds3231_task, "ds3231_task", 2048, NULL, 1, NULL, 1); //  RTS
            xTaskCreatePinnedToCore(ws2812_task, "ws2812_task", 4096, NULL, 1, NULL, 1); // 0 /Leg
            xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 4096, NULL, 1, NULL, 1); // 0 //Bizzer
            break;

        default:
            break;    
    }
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
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 100));

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

/********************************************************************************************
 *                              
 ********************************************************************************************/
static uint8_t get_device_operating_mode( void )
{
    bool ret = false;
    ret = nvsread_device_mode_settings(&device_state);
    if( ret == false )
    {   
        ESP_LOGE(TAG, "device mode settings  are not available");
        device_state.device_operating_mode = NORMAL_MODE;
        device_state.fw_update_state = NO_ERROR_FW_UDPATE;
        ret = nvswrite_device_mode_settings( &device_state );
        if ( ret == false )
        {
            ESP_LOGE( TAG, "device mode settings write error" );
        }
    }
    else
    {

        if ( device_state.device_operating_mode == DONE_FIRMWARE_UPDATE )
        {
            device_state.device_operating_mode =  NORMAL_MODE;
            device_state.fw_update_state = NO_ERROR_FW_UDPATE;
            ret = nvswrite_device_mode_settings( &device_state );
            if ( ret == false )
            {
                ESP_LOGE( TAG, "device mode settings write error" );
            }
        }
        else if ( device_state.device_operating_mode == DO_FIRMWARE_UPDATE )
        {
            if( device_state.fw_update_state != NO_ERROR_FW_UDPATE )
            {
                ESP_LOGI(TAG, "error occured during firmware update");
            }
        } 
    }
    return device_state.device_operating_mode;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static uint8_t get_firmware_update_error( void )
{
    return device_state.fw_update_state;
}


/********************************************************************************************
 *                              
 ********************************************************************************************/
void set_fw_update_errorcode( uint8_t errorcode )
{
    device_state.fw_update_state = errorcode;
    device_state.device_operating_mode = FIRMWARE_UPDATE_ERROR;
    bool ret = nvswrite_device_mode_settings( &device_state );
    if ( ret )
    {
        vTaskDelay (5000/portTICK_PERIOD_MS );
        esp_restart();
    }
    else{
        ESP_LOGE(TAG, "firmware update info write error");
    }
}