/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_upm.h"
#include "lvgl_helpers.h"
#include <timeManagement.h>
#include <sensorManagement.h>
#include <sampleManagement.h>
#include "gui/screens/screen_includes.h"
#include "driver/uart.h"

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

#define DEBUG_UART_NUM      UART_NUM_0
#define BUF_SIZE            (1024)
#define RD_BUF_SIZE         (BUF_SIZE)
static QueueHandle_t        uart0_queue;

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
static void IRAM_ATTR screen_timeout_handler(void *arg);
static void guiTask(void *pvParameter);             /* This it the lvgl task */
static void create_demo_application(void);          /* This function intiate the first screen to show */
static void wakeupmodeInit(void);                   /* This function wakeup the screen*/
static uint8_t nvsread_device_mode_data( void );
static void debug_uart_init(void);

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
        // ESP_LOGI(TAG, "screen timeout handler idle count : %d, %d, %d, %d, %d", idle_minute_counter, get_touchcount(),
        //                  savedtouchcount, get_lcdsleep_status(), dashboardflg);
        if((get_touchcount() == savedtouchcount) && (get_lcdsleep_status() == false))
        {
            idle_minute_counter++;
            if(idle_minute_counter == devicesettings.screen_timeout_value)
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
static void debug_uart_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    //Install UART driver, and get the queue.
    uart_driver_install(DEBUG_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(DEBUG_UART_NUM, &uart_config);

    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(DEBUG_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(DEBUG_UART_NUM, '\n', 1, 9, 0, 0);
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(DEBUG_UART_NUM, 20);
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);

    for(;;) {
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            switch(event.type) {
                case UART_DATA:
                    break;
            
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    uart_flush_input(DEBUG_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
            
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    uart_flush_input(DEBUG_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                
                case UART_PATTERN_DET:
                    uart_get_buffered_data_len(DEBUG_UART_NUM, &buffered_size);
                    int pos = uart_pattern_pop_pos(DEBUG_UART_NUM);
                    int len = 0;
                    if (pos == -1) {
                        uart_flush_input(DEBUG_UART_NUM);
                    } else {
                        len = uart_read_bytes(DEBUG_UART_NUM, dtmp, (pos+1), 100 / portTICK_PERIOD_MS);
                        if( len )
                        {
                            dtmp[len] = '\0';
                            if(strcasestr((char *)dtmp, "AT+SSID=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    ESP_LOGI(TAG, "SSID=%s", devicesettings.wifi_ssid);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        memset(devicesettings.wifi_ssid, 0x00, sizeof(devicesettings.wifi_ssid));
                                        memcpy(devicesettings.wifi_ssid, pch, (strlen(pch)-2));
                                        ESP_LOGI(TAG, "WiFi SSID = %s", devicesettings.wifi_ssid);
                                        if(nvswrite_device_settings(&devicesettings) == false)
                                        {
                                            ESP_LOGE(TAG, "device settings write error for wifi ssid");
                                        }
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+PASSWORD=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    ESP_LOGI(TAG, "PASSWORD=%s", devicesettings.wifi_password);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        memset(devicesettings.wifi_password, 0x00, sizeof(devicesettings.wifi_password));
                                        memcpy(devicesettings.wifi_password, pch, (strlen(pch)-2));
                                        if(nvswrite_device_settings(&devicesettings) == false)
                                        {
                                            ESP_LOGE(TAG, "device settings write error for wifi password");
                                        }
                                        ESP_LOGI(TAG, "WiFi PASSWORD = %s", devicesettings.wifi_password);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+SRNUM=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    ESP_LOGI(TAG, "SERIAL NUMBER = %s", devicesettings.device_serial_number);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        memset(devicesettings.device_serial_number, 0x00, sizeof(devicesettings.device_serial_number));
                                        memcpy(devicesettings.device_serial_number, pch, (strlen(pch)-2));               
                                        if(nvswrite_device_settings(&devicesettings) == false)
                                        {
                                            ESP_LOGE(TAG, "device settings write error for serial number");
                                        }
                                        ESP_LOGI(TAG, "WiFi Serial number = %s", devicesettings.device_serial_number);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+KP=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    struct_PID_parameters_t pid_parameters; 
                                    nvsread_pid_parameters(&pid_parameters);
                                    ESP_LOGI(TAG, "KP Value = %0.2f", pid_parameters.fKp);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        struct_PID_parameters_t pid_parameters; 
                                        nvsread_pid_parameters(&pid_parameters);
                                        pid_parameters.fKp = atof(pch);
                                        nvswrite_pid_parameters(&pid_parameters);
                                        ESP_LOGI(TAG, "KP Value = %0.2f", pid_parameters.fKp);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                    
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+KI=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    struct_PID_parameters_t pid_parameters; 
                                    nvsread_pid_parameters(&pid_parameters);
                                    ESP_LOGI(TAG, "KI Value = %0.2f", pid_parameters.fKi);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        struct_PID_parameters_t pid_parameters; 
                                        nvsread_pid_parameters(&pid_parameters);
                                        pid_parameters.fKi = atof(pch);
                                        nvswrite_pid_parameters(&pid_parameters);
                                        ESP_LOGI(TAG, "KI Value = %0.2f", pid_parameters.fKi);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+KD=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    struct_PID_parameters_t pid_parameters; 
                                    nvsread_pid_parameters(&pid_parameters);
                                    ESP_LOGI(TAG, "KD Value = %0.2f", pid_parameters.fKd);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        struct_PID_parameters_t pid_parameters; 
                                        nvsread_pid_parameters(&pid_parameters);
                                        pid_parameters.fKd = atof(pch);
                                        nvswrite_pid_parameters(&pid_parameters);
                                        ESP_LOGI(TAG, "KD Value = %0.2f", pid_parameters.fKd);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else if(strcasestr((char *)dtmp, "AT+CUSTOMER=") != NULL)
                            {
                                if(strcasestr((char *)dtmp, "?") != NULL)
                                {
                                    ESP_LOGI(TAG, "Customer Name = %s", devicesettings.customer_name);
                                }
                                else
                                {
                                    char *pch;
                                    pch = strchr((char *)dtmp, '=');
                                    if(pch && strcasestr((char *)dtmp, "\r\n") != NULL)
                                    {
                                        pch++;
                                        memset(devicesettings.customer_name, 0x00, sizeof(devicesettings.customer_name));
                                        memcpy(devicesettings.customer_name, pch, (strlen(pch)-2));               
                                        if(nvswrite_device_settings(&devicesettings) == false)
                                        {
                                            ESP_LOGE(TAG, "device settings write error for serial number");
                                        }
                                        ESP_LOGI(TAG, "WiFi Serial number = %s", devicesettings.customer_name);
                                        ESP_LOGI(TAG, "OK");
                                    }
                                }
                            }
                            else{
                                ESP_LOGE(TAG, "incorrect command entered");
                            }
                        }   
                    }
                    break;

                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
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
    debug_uart_init();
    
    gpio_reset_pin(13);
    gpio_pad_select_gpio(13);                 // Set GPIO as OUTPUT
    gpio_set_direction(13, GPIO_MODE_OUTPUT); // WakeMode
    gpio_set_level(13, 1);

    ESP_ERROR_CHECK(i2cdev_init());
    vTaskDelay(500 / portTICK_PERIOD_MS);
    nvs_storage_initialize();     // Initiating the data managament api
    nvsread_device_settings();              //Read device settings from flash
    if(devicesettings.buzzer_enable == 1)
        buzzer_initialization();                // This will initiate the buzze in the system

    uint8_t devicemode = nvsread_device_mode_data();

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);

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
            ESP_LOGI(TAG, "device mode is not valid");
            device_state.fw_update_state = NO_ERROR_FW_UDPATE;
            device_state.device_operating_mode = NORMAL_MODE;
            bool ret = nvswrite_device_mode_settings( &device_state );
            if ( ret != true )
            {
                ESP_LOGE(TAG, "device mode write error");
            }
            esp_restart();
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

    const esp_timer_create_args_t screen_timeout_timer_args = {
        .callback = &screen_timeout_handler,
        .name = "screen_timer"};
    esp_timer_handle_t screen_timeout_timer;

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 100));

    ESP_ERROR_CHECK(esp_timer_create(&screen_timeout_timer_args, &screen_timeout_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(screen_timeout_timer, 60*1000*1000));

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
static uint8_t nvsread_device_mode_data( void )
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
    
    return device_state.device_operating_mode;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
uint8_t get_firmware_update_error( void )
{
    return device_state.fw_update_state;
}

/********************************************************************************************
 *                              
 ********************************************************************************************/
uint8_t get_device_working_mode( void )
{
    return device_state.device_operating_mode;
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

/********************************************************************************************
 *                              
 ********************************************************************************************/
void set_device_normal_operating_mode( void )
{
    device_state.fw_update_state = NO_ERROR_FW_UDPATE;
    device_state.device_operating_mode = NORMAL_MODE;
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