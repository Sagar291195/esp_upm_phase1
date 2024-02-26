#include "esp_upm.h"
#include "lvgl_helpers.h"

#include "esp_log.h"

#include "gui/generated/gui_guider.h"
#include "gui/custom/custom.h"

#define TAG     "Sensor Test"

#define SDA_GPIO                                21      /* i2c sda pin */
#define SCL_GPIO                                22      /* i2c scl pin */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR        240.0   /* scale factor for the sdp32 diff sensor */
#define SDP32_DIFF_TEMPERATURE_SCALE_FACTOR     200.0   /*sdp32 temperater scale factor */
#define SDP32_SENSOR_READ_DURATION_IN_MS        20      /* This is the duration after which the sensor will update the data into the array. */

extern SemaphoreHandle_t i2c_communication_semaphore;    /* semaphore to Processes*/ 

static test_state_t devicestate = STATE_IDLE;
static bool sensorinitdone = false;
static bme680_t bme680;
static uint32_t duration;      // as long as sensor configuration isn't changed, duration is constant
static bmp280_t bmp280;
static bmp280_params_t params;
static sdp32_t sdp32;

static void bm680_test(void);
static void bme280_test(void);
static void sdp32_test(void);
static void led_test(void);

void sensorReadTask(void *pvParameter)
{
    while(1)
    {
        switch(devicestate)
        {
            case STATE_IDLE:
                break;

            case STATE_START_TEST:
                ESP_LOGI(TAG, "selected item = %d", get_selected_sensor());
                if(get_selected_sensor() == BME680)
                {
                    devicestate = STATE_BME680_TEST;
                }
                else if(get_selected_sensor() == BME280)
                {
                    devicestate = STATE_BME280_TEST;
                }   
                else if(get_selected_sensor() == SDP32)
                {
                    devicestate = STATE_SDP32_TEST;
                }
                else if(get_selected_sensor() == LED)
                {
                    devicestate = STATE_LED_TEST;
                }
                break;

            case STATE_BME680_TEST:
                bm680_test();
                break;

            case STATE_BME280_TEST:
                bme280_test();
                break;

            case STATE_SDP32_TEST:
                sdp32_test();
                break;

            case STATE_LED_TEST:
                led_test();
                break;               

            case STATE_STOP_TEST:
                sensorinitdone = false; 
                devicestate = STATE_IDLE;
                break;

            default:
                break;    
        }
       vTaskDelay(1000);
    }
}


void set_device_state( test_state_t state )
{
    devicestate = state;
}


test_state_t get_device_state(void)
{
    return devicestate;
}   

static void bm680_test(void)
{
    esp_err_t err = ESP_FAIL;
    bme680_values_float_t values;
    char data[100];
   
    if( sensorinitdone == false )
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            memset(&bme680, 0, sizeof(bme680_t));
            ESP_ERROR_CHECK_WITHOUT_ABORT(bme680_init_desc(&bme680, BME680_I2C_ADDR_0, 0, SDA_GPIO, SCL_GPIO));
            err = bme680_init_sensor(&bme680);       // init the sensor
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Unable to initaite the bme680 sensor");
                lv_label_set_text(guider_ui.screen_label_value, "BME680 sensor init failed");
                devicestate = STATE_STOP_TEST;
            }
            else
            {
                ESP_LOGI(TAG, "Bme 680 sucessfully initiated");
                bme680_set_oversampling_rates(&bme680, BME680_OSR_8X, BME680_OSR_8X, BME680_OSR_8X);    // Changes the oversampling rates to 4x oversampling for all
                bme680_set_filter_size(&bme680, BME680_IIR_SIZE_7); // Change the IIR filter size for temperature and pressure to 7 for accuracy
                bme680_use_heater_profile(&bme680, -1); // Change the heater profile 0 to 200 degree Celsius for 100 ms.
                bme680_get_measurement_duration(&bme680, &duration);
                sensorinitdone = true;
            }
            xSemaphoreGive(i2c_communication_semaphore);  
        }
    }
    
    if(sensorinitdone == true)
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        { 
            if (bme680_force_measurement(&bme680) == ESP_OK)        // trigger the sensor to start one TPHG measurement cycle
            {
                vTaskDelay(duration);    // passive waiting until measurement results are available
                if (bme680_get_results_float(&bme680, &values) == ESP_OK)       // get the results and do something with them
                {
                    memset(data, 0x00, sizeof(data));
                    sprintf(data, "BME680 sensor test is running\nTemperature : %0.02f\nPressure : %0.02f\nHumidity : %0.02f", 
                                values.temperature, values.pressure, values.humidity);
                    ESP_LOGI(TAG, "%s", data);            
                    lv_label_set_text(guider_ui.screen_label_value, data);
                    xSemaphoreGive(i2c_communication_semaphore);
                }
            }
        }
    }
}


static void bme280_test(void)
{
    float temperature;
    float pressure;
    float humidity;
    char data[100];

    if( sensorinitdone == false )
    {
        bmp280_init_default_params(&params);
        params.filter = BMP280_FILTER_8;
        memset(&bmp280, 0, sizeof(bmp280_t));
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_init_desc(&bmp280, BMP280_I2C_ADDRESS_1, 0, SDA_GPIO, SCL_GPIO));
            esp_err_t err = bmp280_init(&bmp280, &params);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to initialize the bmp280 task");
                devicestate = STATE_STOP_TEST;
                lv_label_set_text(guider_ui.screen_label_value, "BME280 sensor init failed");
            }
            else
            {
                ESP_LOGI(TAG, "Initiated the bme280 sensor");
                sensorinitdone = true;
            }
            xSemaphoreGive(i2c_communication_semaphore);
        }
    }

    if( sensorinitdone == true )
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_read_float(&bmp280, &temperature, &pressure, &humidity));
            ESP_LOGI(TAG, "Temperature = %.02f", temperature);
            ESP_LOGI(TAG, "Pressure = %.02f", pressure);
            ESP_LOGI(TAG, "Humidity = %.02f", humidity);
            memset(data, 0x00, sizeof(data));
            sprintf(data, "BME680 sensor test is running\nTemperature : %0.02f\nPressure : %0.02f\nHumidity : %0.02f", 
                        temperature, (pressure/100), humidity);
            ESP_LOGI(TAG, "%s", data);            
            lv_label_set_text(guider_ui.screen_label_value, data);
            xSemaphoreGive(i2c_communication_semaphore);
        }
    }
}


static void sdp32_test(void)
{
    uint8_t stopContRead_cmd[2] = {0x3F, 0xF9}; // Stop Countinous measure command
    uint8_t massFlowRead[2] = {0x36, 0x03}; /* mass flow, temperature compensated diff pressure with average mode */
    uint8_t read_buff[9] = {0}; /* read buffer 0 initialized */
    uint8_t checksum;       /* variable to store the checksum  */
    int16_t temperature;
    float sdp32_pressure_value = 0;
    float sdp32_temperature_value = 0;
    int16_t sensorvalue = 0;
    char data[100];

    if( sensorinitdone == false )
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            memset(&sdp32, 0, sizeof(sdp32_t));
            ESP_ERROR_CHECK_WITHOUT_ABORT(sdp32_init_desc(&sdp32, SDP32_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO)); /* initializing spd32 sensor */
            vTaskDelay(pdMS_TO_TICKS(20));
            sdp32_send_cmd_read(&sdp32, stopContRead_cmd);     /* sending stop continuous read command */
            vTaskDelay(pdMS_TO_TICKS(20));
            sdp32_send_cmd_read(&sdp32, massFlowRead); 
            vTaskDelay(pdMS_TO_TICKS(20)); 
            sensorinitdone = true;
            ESP_LOGI(TAG, "Initiated the sdp32 sensor");
            xSemaphoreGive(i2c_communication_semaphore);
        }
        
    }

    if(sensorinitdone == true)
    {
        vTaskDelay(pdMS_TO_TICKS(SDP32_SENSOR_READ_DURATION_IN_MS));
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            sdp32_read_pressure(&sdp32, read_buff); // actually reading the mass flow
            xSemaphoreGive(i2c_communication_semaphore);
            checksum = read_buff[2];     /* checksum is the third byte of the array after the first two bytes which ar actual readings */
            bool check = CheckCrc(read_buff, 2, checksum);  /* checking the checksum, as the values we are getting are correct or not */
            if (check)  /* if checksum failed then continue the loop, else store the value in the array */
            {
                sensorvalue = (read_buff[0] << 8) | read_buff[1];
                ESP_LOGI(TAG, "sensor value = %d", sensorvalue);
                sdp32_pressure_value =  ((float)sensorvalue) / SDP32_DIFF_PRESSURE_SCALE_FACTOR;
                temperature = (read_buff[3] << 8) | read_buff[4];
                sdp32_temperature_value = ((float)temperature) / SDP32_DIFF_TEMPERATURE_SCALE_FACTOR;
                ESP_LOGI(TAG, "Temperature = %.02f", sdp32_temperature_value);
                ESP_LOGI(TAG, "Pressure = %.02f", sdp32_pressure_value);
                memset(data, 0x00, sizeof(data));
                sprintf(data, "SDP32 sensor test is running\nTemperature : %0.02f\nPressure : %0.02f", 
                            sdp32_temperature_value, sdp32_pressure_value);
                lv_label_set_text(guider_ui.screen_label_value, data);
                ESP_LOGI(TAG, "%s", data);  
            }
        }
    }
   
}


static void led_test(void)
{
    const uint8_t pixel_count = 1;

    if(sensorinitdone == false)
    {
        ws2812_init(13);
        sensorinitdone = true;
    }

    if( sensorinitdone == true)
    {
        lv_label_set_text(guider_ui.screen_label_value, "LED Test is running");
        rgbVal color = makeRGBVal(0xFF, 0x00, 0x00);
        rgbVal *pixels;
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);

        color = makeRGBVal(0x00, 0x00, 0x00);
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);

        color = makeRGBVal(0x00, 0xFF, 0x00);
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);

        color = makeRGBVal(0x00, 0x00, 0x00);
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);
        
        color = makeRGBVal(0x00, 0x00, 0xFF);
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);

        color = makeRGBVal(0x00, 0x00, 0x00);
        pixels = malloc(sizeof(rgbVal) * pixel_count);
        pixels[0] = color;
        ws2812_setColors(pixel_count, pixels);
        vTaskDelay(500);
        free(pixels);
    }
}