/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_upm.h"
#include "lvgl_helpers.h"

#include "gui/generated/gui_guider.h"
#include "gui/custom/custom.h"
#include "gui/generated/events_init.h"


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
SemaphoreHandle_t i2c_communication_semaphore;    /* semaphore to Processes*/ 
SemaphoreHandle_t gui_update_semaphore;   /* semaphore to GUId*/ 
lv_ui guider_ui;

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void IRAM_ATTR lv_tick_task(void *arg);      /* lvgl task to count the ticks */
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
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
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
    
    i2c_communication_semaphore = xSemaphoreCreateMutex();
    gui_update_semaphore = xSemaphoreCreateMutex();
    wakeupmodeInit();                   // enabling the device from the wake mode
    
    ESP_ERROR_CHECK(i2cdev_init());
    vTaskDelay(500 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 4, NULL, 1, NULL, 1); // 0 LCD +Touch
    xTaskCreate(sensorReadTask, "sensorread", 4096, NULL, 1, NULL); 
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

