/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <string.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <sensorManagement.h>
#include <timeManagement.h>
#include "esp_upm.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                                     "sensorManagement"
#define NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP 5
#define NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BME 5
#define BME280_SENSOR_READ_IN_MS                200
#define BME_280_AVERAGING_TIME_IN_MS            1000    /* bme 280 internal sensor averaging time in ms */
#define BME_680_AVERAGING_TIME_IN_MS            1000    /* bme 680 external sensor averating time in ms */
#define BME680_SENSOR_READ_IN_MS                200
#define INA3221_CURRENT_SENSOR_IN_MS            1000    /* INA sensor averaging time in ms */
#define SDP32_SENSOR_READ_DURATION_IN_MS        20      /* This is the duration after which the sensor will update the data into the array. */
#define SDP32_SENSOR_AVERAGE_DURATION_IN_MS     200     /* sdp sensor average time out */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR        240.0   /* scale factor for the sdp32 diff sensor */
#define SDP32_DIFF_TEMPERATURE_SCALE_FACTOR     200.0   /*sdp32 temperater scale factor */
#define NO_OF_SAMPLES_SDP32                     10
#define SDA_GPIO                                21      /* i2c sda pin */
#define SCL_GPIO                                22      /* i2c scl pin */

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
static float internal_temperature_average = 0;       /* internal temperature sensor */
static float internal_humidity_average = 0;      
static float internal_pressure_average = 0;          /* internal pressure sensor */
static external_sensor_data_t external_sensor_data_average = {0};   /* Data variable for the bme680 sensor */    
float sdp32_pressure_value = 0; /* sp32 diff pressure sensor average values */
float sdp32_temperature_value = 0;    /* sdp32 average temperature sensor value*/
INA3231_sensor_data_t ina3221_sensor_data[INA3221_CHANNEL];         /* array to store the sensor data */


/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void external_sensor_read_task(void *pvParameters);
static void ina3221_sensor_read_task(void *pvParameters);
static void internal_sensor_read_task(void *pvParamters);
static void sdp32_sensor_read_task(void *pvParameters);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
* 
********************************************************************************************/
static void external_sensor_read_task(void *pvParameters)
{
    bme680_t sensor;
    esp_err_t err = ESP_FAIL;
    static external_sensor_data_t temp_external_sensor_average;
    static uint8_t external_sensor_read_count = 0;         /* keep track of the sdp sensor read count */

    memset(&sensor, 0, sizeof(bme680_t));
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(bme680_init_desc(&sensor, BME680_I2C_ADDR_0, 0, SDA_GPIO, SCL_GPIO));
        err = bme680_init_sensor(&sensor);       // init the sensor
        xSemaphoreGive(i2c_communication_semaphore);
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Unable to initaite the bme680 sensor");
        vTaskDelete(NULL);
    }
    ESP_LOGD(TAG, "Bme 680 sucessfully initiated");

    bme680_set_oversampling_rates(&sensor, BME680_OSR_8X, BME680_OSR_8X, BME680_OSR_8X);    // Changes the oversampling rates to 4x oversampling for all
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7); // Change the IIR filter size for temperature and pressure to 7 for accuracy
    bme680_use_heater_profile(&sensor, -1); // Change the heater profile 0 to 200 degree Celsius for 100 ms.
    
    uint32_t duration;      // as long as sensor configuration isn't changed, duration is constant
    bme680_get_measurement_duration(&sensor, &duration);
    ESP_LOGD(TAG, "Bme 680 duration is %d", duration);
    TickType_t last_wakeup = xTaskGetTickCount();
    bme680_values_float_t values;
   
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    { 
        if (bme680_force_measurement(&sensor) == ESP_OK)        // trigger the sensor to start one TPHG measurement cycle
        {
            xSemaphoreGive(i2c_communication_semaphore);
            vTaskDelay(duration);    // passive waiting until measurement results are available
            xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY);
            if (bme680_get_results_float(&sensor, &values) == ESP_OK)       // get the results and do something with them
            {
                temp_external_sensor_average.temperature = values.temperature;
                temp_external_sensor_average.humidity = values.humidity;
                temp_external_sensor_average.pressure = values.pressure;
                memcpy(&external_sensor_data_average, &temp_external_sensor_average, sizeof(temp_external_sensor_average));
            }            
        }
        xSemaphoreGive(i2c_communication_semaphore);
    }

    while (1)
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            if (bme680_force_measurement(&sensor) == ESP_OK)    // trigger the sensor to start one TPHG measurement cycle
            {
                xSemaphoreGive(i2c_communication_semaphore);
                vTaskDelay(duration);   // passive waiting until measurement results are available
                if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
                {
                    if (bme680_get_results_float(&sensor, &values) == ESP_OK)        // get the results and do something with them
                    {
                        temp_external_sensor_average.temperature += values.temperature;
                        temp_external_sensor_average.humidity += values.humidity;
                        temp_external_sensor_average.pressure += values.pressure; // converting to Pa unit
                        temp_external_sensor_average.gas_resistance += values.gas_resistance;
                        external_sensor_read_count++;
                        if (external_sensor_read_count == NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BME)
                        {
                            external_sensor_read_count = 0;
                            external_sensor_data_average.pressure = temp_external_sensor_average.pressure / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                            external_sensor_data_average.temperature = temp_external_sensor_average.temperature / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                            external_sensor_data_average.humidity = temp_external_sensor_average.humidity / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                            external_sensor_data_average.gas_resistance = temp_external_sensor_average.gas_resistance / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                            ESP_LOGD(TAG, "External sensor Temp: %0.2f, Humidity: %0.2f, Pressure: %0.2f", external_sensor_data_average.temperature, external_sensor_data_average.humidity, external_sensor_data_average.pressure);
                            memset(&temp_external_sensor_average, 0x00, sizeof(temp_external_sensor_average));
                        }
                    }
                }
            }
            xSemaphoreGive(i2c_communication_semaphore);
        }
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(BME680_SENSOR_READ_IN_MS));     // passive waiting until 1 second is over
    }
}

/********************************************************************************************
* 
********************************************************************************************/ 
static void internal_sensor_read_task(void *pvParamters)
{
    float temp_bme280_temperature_average = 0;  
    float temp_bme280_pressure_average = 0;     
    float temp_bme280_humidity_average = 0; 
    uint8_t internal_sensor_read_count = 0;
    float pressure, temperature, humidity;
    bmp280_t dev;
    bmp280_params_t params;
    
    bmp280_init_default_params(&params);
    params.filter = BMP280_FILTER_8;
    memset(&dev, 0, sizeof(bmp280_t));

    TickType_t last_wakeup = xTaskGetTickCount();
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_1, 0, SDA_GPIO, SCL_GPIO));
        esp_err_t err = bmp280_init(&dev, &params);
        xSemaphoreGive(i2c_communication_semaphore);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to initialize the bmp280 task");
            vTaskDelete(NULL);
        }
        else
        {
            ESP_LOGD(TAG, "Initiated the bme280 sensor");
        }
    }

    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_read_float(&dev, &temperature, &pressure, &humidity));
        xSemaphoreGive(i2c_communication_semaphore);
        internal_temperature_average = temperature;
        internal_humidity_average = humidity;
        internal_pressure_average = pressure;
    }

    while (1)
    {
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            if (bmp280_read_float(&dev, &temperature, &pressure, &humidity) != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to read Internal Sensor Reading");
                continue;
            }
            else
            {
                temp_bme280_temperature_average += temperature;
                temp_bme280_humidity_average += humidity;
                temp_bme280_pressure_average += pressure;
                internal_sensor_read_count++;
                if (internal_sensor_read_count == NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP)
                {
                    internal_sensor_read_count = 0;
                    internal_temperature_average = temp_bme280_temperature_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                    internal_pressure_average = temp_bme280_pressure_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                    internal_humidity_average = temp_bme280_humidity_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
                    ESP_LOGD(TAG, "Internal sensor Temp: %0.2f, Humidity: %0.2f, Pressure: %0.2f", internal_temperature_average, internal_humidity_average, (internal_pressure_average/100));
                    temp_bme280_temperature_average = 0;
                    temp_bme280_pressure_average = 0;
                    temp_bme280_humidity_average = 0;
                }
            }
            xSemaphoreGive(i2c_communication_semaphore);
        }
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(BME280_SENSOR_READ_IN_MS));
    }
    vTaskDelete(NULL);
}

/********************************************************************************************
* 
********************************************************************************************/
static void ina3221_sensor_read_task(void *pvParameters)
{
    ina3221_t dev = {
        .shunt = {100, 100, 100}, // shunt values are 100 mOhm for each channel
        .config.config_register = INA3221_DEFAULT_CONFIG,
        .mask.mask_register = INA3221_DEFAULT_MASK};
    memset(&dev.i2c_dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_init_desc(&dev, INA3221_I2C_ADDR, 0, SDA_GPIO, SCL_GPIO));
    ESP_LOGD(TAG, "Ina3231 has been inititated");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_options(&dev, MODE, true, true));              // Mode selection, bus and shunt activated
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_enable_channel(&dev, true, true, true));           // Enable all channels
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_average(&dev, INA3221_AVG_4));                 // 4 samples average
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_bus_conversion_time(&dev, INA3221_CT_2116));   // 2ms by channel
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_shunt_conversion_time(&dev, INA3221_CT_2116)); // 2ms by channel
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_warning_alert(&dev, WARNING_CHANNEL - 1, WARNING_CURRENT)); // Set overcurrent security flag
        xSemaphoreGive(i2c_communication_semaphore);
    }
    ESP_LOGD(TAG, "Ina3231 has been inititated");
    vTaskDelay(pdMS_TO_TICKS(500));
    float bus_voltage;
    float shunt_voltage;
    float shunt_current;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(INA3221_CURRENT_SENSOR_IN_MS));
#if !MODE
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_trigger(&dev)); // Start a measure
            xSemaphoreGive(i2c_communication_semaphore);
        }

        do
        {
            if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
            {
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_status(&dev)); // get mask
                xSemaphoreGive(i2c_communication_semaphore);
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);

        } while (!(dev.mask.cvrf)); // check if measure done
#else
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_status(&dev)); // get mask
            xSemaphoreGive(i2c_communication_semaphore);
        }

        if (dev.mask.wf & (1 << (3 - WARNING_CHANNEL)))
            warning = true;
#endif
        for (uint8_t i = 0; i < INA3221_BUS_NUMBER; i++)
        {
            if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
            {
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_bus_voltage(&dev, i, &bus_voltage));  // Get voltage in volts   
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_shunt_value(&dev, i, &shunt_voltage, &shunt_current));    // Get voltage in millivolts and current in milliamperes
                xSemaphoreGive(i2c_communication_semaphore);
                ina3221_sensor_data[i].bus_voltage = bus_voltage;
                ina3221_sensor_data[i].shunt_voltage = shunt_voltage;
                ina3221_sensor_data[i].shunt_current = shunt_current;
                ESP_LOGD(TAG, "Channel %d: Bus voltage: %.2f V, Shunt voltage: %.2f mV, Shunt current: %.2f mA", i, bus_voltage, shunt_voltage, shunt_current);
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}

/********************************************************************************************
* 
********************************************************************************************/
static void sdp32_sensor_read_task(void *pvParameters)
{
    sdp32_t dev;
    uint8_t read_buff[9] = {0}; /* read buffer 0 initialized */
    uint8_t checksum;       /* variable to store the checksum  */
    uint8_t stopContRead_cmd[2] = {0x3F, 0xF9}; // Stop Countinous measure command
    uint8_t massFlowRead[2] = {0x36, 0x03}; /* mass flow, temperature compensated diff pressure with average mode */
    static float temp_sdp32_pressure = 0;
    static uint8_t sdp32_read_count = 0;     /* index for calculating the average values */
    int16_t massFlow;
    int16_t temperature;
    float temp_value = 0;
    int16_t sensorvalue = 0;
    
    memset(&dev, 0, sizeof(sdp32_t));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdp32_init_desc(&dev, SDP32_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO)); /* initializing spd32 sensor */
    vTaskDelay(pdMS_TO_TICKS(20));

    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        sdp32_send_cmd_read(&dev, stopContRead_cmd);     /* sending stop continuous read command */
        xSemaphoreGive(i2c_communication_semaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    ESP_LOGD(TAG, "Starting sdp32 sensor in massflow mode");
    if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
    {
        sdp32_send_cmd_read(&dev, massFlowRead);
        xSemaphoreGive(i2c_communication_semaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(SDP32_SENSOR_READ_DURATION_IN_MS));
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            sdp32_read_pressure(&dev, read_buff); // actually reading the mass flow
            xSemaphoreGive(i2c_communication_semaphore);
        }
        checksum = read_buff[2];     /* checksum is the third byte of the array after the first two bytes which ar actual readings */
        bool check = CheckCrc(read_buff, 2, checksum);  /* checking the checksum, as the values we are getting are correct or not */
        if (!check)  /* if checksum failed then continue the loop, else store the value in the array */
        {
            continue;
        }
        else
        {
            sensorvalue = (read_buff[0] << 8) | read_buff[1];
            temp_value =  ((float)sensorvalue) / SDP32_DIFF_PRESSURE_SCALE_FACTOR;
            temp_sdp32_pressure += temp_value;
            temperature = (read_buff[3] << 8) | read_buff[4];
            sdp32_temperature_value = ((float)temperature) / SDP32_DIFF_TEMPERATURE_SCALE_FACTOR;
            sdp32_read_count++;
            if (sdp32_read_count == NO_OF_SAMPLES_SDP32)
            {
                sdp32_pressure_value = temp_sdp32_pressure / NO_OF_SAMPLES_SDP32;
                sdp32_read_count = 0;
                temp_sdp32_pressure = 0;
                ESP_LOGD(TAG, "SDP32 Temperature %0.2f, Pressure  %0.2f, massflow : %d", sdp32_temperature_value, sdp32_pressure_value, massFlow);
            }

        }
    }
    vTaskDelete(NULL);
}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
 
/********************************************************************************************
* 
********************************************************************************************/
float get_internal_temperature_value(void)
{
    return internal_temperature_average;
}

/********************************************************************************************
* 
********************************************************************************************/
float get_internal_humidity_value(void)
{
    return internal_humidity_average;
}

/********************************************************************************************
* 
********************************************************************************************/
float get_internal_pressure_value(void)
{
    return (internal_pressure_average/100);
}

/********************************************************************************************
* 
********************************************************************************************/
void get_ina3221_sensor_data(INA3231_sensor_data_t *ina_sensor)
{
    memcpy(ina_sensor, ina3221_sensor_data, INA3221_CHANNEL * sizeof(INA3231_sensor_data_t));
}

/********************************************************************************************
* 
********************************************************************************************/
void get_external_sensor_data_raw(external_sensor_data_t *external_sensor_reading)
{
    memcpy(external_sensor_reading, &external_sensor_data_average, sizeof(external_sensor_data_t));
}

/********************************************************************************************
* 
********************************************************************************************/
float get_sdp32_pressure_value(void)
{
    return sdp32_pressure_value;
}

/********************************************************************************************
* 
********************************************************************************************/
float get_sdp32_temperature_value(void)
{
    return sdp32_temperature_value;
}

/********************************************************************************************
*                           
********************************************************************************************/
float get_sdp32_massflow_value(void)
{
    float fResult = get_sdp32_pressure_value();

    return fResult;
}
/********************************************************************************************
* 
********************************************************************************************/
void sensor_initialization(void)
{
    rtc_sensor_initialize();
    vTaskDelay(1000);
    xTaskCreate(external_sensor_read_task, "external", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(internal_sensor_read_task, "internal", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(sdp32_sensor_read_task, "sdp32", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(ina3221_sensor_read_task, "ina3231", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
}