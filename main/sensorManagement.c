/**
 * @file sensorManagement.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the implementaton of the sensor management api. The sensor management contain the tasks which takes the raw values from the sensors and average them in the given time frame.
 * @version 1.1
 * @date 2022-06-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sensorManagement.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sensors/bme680.h"
#include "sensors/bmp280.h"

#include <string.h>
#include "external/ina3221.h"
#include "sensors/sdp32.h"
#include <timeManagement.h>
/********************************************defines****************************************************************/

#define TAG "sensorManagement"

/* this will number of values to be average for calculating the average */
#define NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP 5
#define BME280_SENSOR_READ_IN_MS 200
/* bme 280 internal sensor averaging time in ms */
#define BME_280_AVERAGING_TIME_IN_MS 1000
/* bme 680 external sensor averating time in ms */
#define BME_680_AVERAGING_TIME_IN_MS 1000
#define BME680_SENSOR_READ_IN_MS 200
/* INA sensor averaging time in ms */
#define INA3221_CURRENT_SENSOR_IN_MS 1000
/* This is the duration after which the sensor will update the data into the array. */
#define SDP32_SENSOR_READ_DURATION_IN_MS 20
/* sdp sensor average time out */
#define SDP32_SENSOR_AVERAGE_DURATION_IN_MS 200
/* scale factor for the sdp32 diff sensor */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR 240.0
/*sdp32 temperater scale factor */
#define SDP32_DIFF_TEMPERATURE_SCALE_FACTOR 200.0
#define NO_OF_SAMPLES_SDP32 10

/* i2c bus configuration */
#define SDA_GPIO 21
#define SCL_GPIO 22

/*************************************************variables***********************************************************/

extern SemaphoreHandle_t xGuiSemaphore;

/* internal temperature sensor */
float bme280_temperature_average = 0;
/*  internal humidity sensor */
float bme280_humidity_average = 0;
/* internal pressure sensor */
float bme280_pressure_average = 0;
/* array of values for the internal temperature sensor */
static volatile float bme280_temperature_array[NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP] = {0};
/* array of values for the internal humidity sensor */
static volatile float bme280_humidity_array[NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP] = {0};
/* array of values for the internal pressure sensor */
static volatile float bme280_pressure_array[NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP] = {0};
/* keep track of the sdp sensor read count */
static volatile uint8_t last_update_sensor_value_index = 0;
/* index for calculating the average values */
static volatile uint8_t last_update_bmp_sensor_value_index = 0;
/* array to hold the raw spd32 values from the sensor */
static volatile float noOfSamplesSdp32[NO_OF_SAMPLES_SDP32] = {0};
/* sp32 diff pressure sensor average values */
float fSdp32_diff_pressure_average = 0;
/* sdp32 average temperature sensor value*/
float fSpd32Temperatuer_average = 0;
/* Data variable for the bme680 sensor */
static external_sensor_data_t external_sensor_data_average = {0};
/* array for external sensor data for calulating the average values */
static volatile external_sensor_data_t external_sensor_data[NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP] = {0};
/* array to store the sensor data */
INA3231_sensor_data_t INA3231_sensor_data[INA3221_CHANNEL];

/**
 * @brief function of this task is to calculate the average values of the bmp280 and bme680 sensors
 *
 * @param pvParameters
 */
void vAverageBMPValue(void *pvParameters);
/***********************************************************function definations************************************/

void Internal_Seneor_bme280_task(void *pvParamters)
{
    float pressure, temperature, humidity;

    bmp280_params_t params;
    bmp280_init_default_params(&params);
    params.filter = BMP280_FILTER_8;
    bmp280_t dev;
    memset(&dev, 0, sizeof(bmp280_t));

    TickType_t last_wakeup = xTaskGetTickCount();
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

        ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_1, 0, SDA_GPIO, SCL_GPIO));
        esp_err_t err = bmp280_init(&dev, &params);
        xSemaphoreGive(xGuiSemaphore);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to initialize the bmp280 task");

            vTaskDelete(NULL);
        }
        else
        {
            ESP_LOGI(TAG, "Initiated the bme280 sensor");
        }
    }

    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_read_float(&dev, &temperature, &pressure, &humidity));
        xSemaphoreGive(xGuiSemaphore);
        bme280_temperature_average = temperature;
        bme280_humidity_average = humidity;
        bme280_pressure_average = pressure;
    }

    while (1)
    {
        // ESP_LOGD(TAG, "Taking semaphore for Internal Sensor Read");
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            if (bmp280_read_float(&dev, &temperature, &pressure, &humidity) != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to read Internal Sensor Reading");
                continue;
            }
            else
            {
                bme280_temperature_array[last_update_bmp_sensor_value_index] = temperature;
                bme280_humidity_array[last_update_bmp_sensor_value_index] = humidity;
                bme280_pressure_array[last_update_bmp_sensor_value_index] = (pressure / 100);
                // ESP_LOGD(TAG, "Internal Temperature %0.2f, bme280 humidity %0.2f, bme280 pressure %0.2f ", temperature, humidity, (pressure/100));

                if (last_update_bmp_sensor_value_index == NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP - 1)
                {
                    last_update_bmp_sensor_value_index = 0;
                }
                else
                {
                    last_update_bmp_sensor_value_index++;
                }
            }
            xSemaphoreGive(xGuiSemaphore);
        }
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(BME280_SENSOR_READ_IN_MS));
    }

    vTaskDelete(NULL);
}

float fGetBme280TemperatureAverages(void)
{
    return bme280_temperature_average;
}

float fGetBme280HumidityAverages(void)
{
    return bme280_humidity_average;
}

float fGetBme280PressureAverages(void)
{
    return bme280_pressure_average;
}

void vExternalBME680SensorTask(void *pvParameters)
{

    bme680_t sensor;
    memset(&sensor, 0, sizeof(bme680_t));
    esp_err_t err = ESP_FAIL;
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(bme680_init_desc(&sensor, BME680_I2C_ADDR_0, 0, SDA_GPIO, SCL_GPIO));

        // init the sensor
        err = bme680_init_sensor(&sensor);
        xSemaphoreGive(xGuiSemaphore);
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Unable to initaite the bme680 sensor");
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "Bme 680 sucessfully initiated");

    // Changes the oversampling rates to 4x oversampling for all
    bme680_set_oversampling_rates(&sensor, BME680_OSR_8X, BME680_OSR_8X, BME680_OSR_8X);

    // Change the IIR filter size for temperature and pressure to 7 for accuracy
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

    // Change the heater profile 0 to 200 degree Celsius for 100 ms.
    // bme680_set_heater_profile(&sensor, 0, 150, 100);
    bme680_use_heater_profile(&sensor, -1);

    // Set ambient temperature to 10 degree Celsius
    // bme680_set_ambient_temperature(&sensor, 25);

    // as long as sensor configuration isn't changed, duration is constant
    uint32_t duration;
    bme680_get_measurement_duration(&sensor, &duration);

    ESP_LOGI(TAG, "Bme 680 duration is %d", duration);

    // duration += 30;

    TickType_t last_wakeup = xTaskGetTickCount();

    bme680_values_float_t values;

    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    // trigger the sensor to start one TPHG measurement cycle
    if (bme680_force_measurement(&sensor) == ESP_OK)
    {
        xSemaphoreGive(xGuiSemaphore);
        // passive waiting until measurement results are available
        vTaskDelay(duration);

        xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
        // get the results and do something with them
        if (bme680_get_results_float(&sensor, &values) == ESP_OK)
        /* averaging the values over the given time frame */
        {
            external_sensor_data_average.fTemperature = values.temperature;
            external_sensor_data_average.fHumidity = values.humidity;
            external_sensor_data_average.fPressure = values.pressure;
        }
        xSemaphoreGive(xGuiSemaphore);
    }
    else
    {
        xSemaphoreGive(xGuiSemaphore);
    }

    while (1)
    {

        xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
        // trigger the sensor to start one TPHG measurement cycle
        if (bme680_force_measurement(&sensor) == ESP_OK)
        {
            xSemaphoreGive(xGuiSemaphore);
            // passive waiting until measurement results are available
            vTaskDelay(duration);

            xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
            // get the results and do something with them
            if (bme680_get_results_float(&sensor, &values) == ESP_OK)
            {
                // ESP_LOGD(TAG, "External Temperature : %0.2f, Humidity : %0.2f, Pressure : %.02f", values.temperature, values.humidity, values.pressure);
                external_sensor_data[last_update_bmp_sensor_value_index].fTemperature = values.temperature;
                external_sensor_data[last_update_bmp_sensor_value_index].fHumidity = values.humidity;
                external_sensor_data[last_update_bmp_sensor_value_index].fPressure = values.pressure; // converting to Pa unit
                external_sensor_data[last_update_bmp_sensor_value_index].fGasResistance = values.gas_resistance;

                if (last_update_bmp_sensor_value_index == NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP - 1)
                {
                    last_update_bmp_sensor_value_index = 0;
                }
                else
                {
                    last_update_bmp_sensor_value_index++;
                }
            }

            xSemaphoreGive(xGuiSemaphore);
        }

        else
        {
            xSemaphoreGive(xGuiSemaphore);
        }
        // passive waiting until 1 second is over
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(BME680_SENSOR_READ_IN_MS));
    }
}

void vIna3221_Sensor_task(void *pvParameters)
{
    ina3221_t dev = {
        .shunt = {100, 100, 100}, // shunt values are 100 mOhm for each channel
        .config.config_register = INA3221_DEFAULT_CONFIG,
        .mask.mask_register = INA3221_DEFAULT_MASK};
    memset(&dev.i2c_dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_init_desc(&dev, INA3221_I2C_ADDR, 0, SDA_GPIO, SCL_GPIO));
    ESP_LOGI(TAG, "Ina3231 has been inititated");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_options(&dev, MODE, true, true));              // Mode selection, bus and shunt activated
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_enable_channel(&dev, true, true, true));           // Enable all channels
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_average(&dev, INA3221_AVG_4));                 // 4 samples average
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_bus_conversion_time(&dev, INA3221_CT_2116));   // 2ms by channel
        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_shunt_conversion_time(&dev, INA3221_CT_2116)); // 2ms by channel

        ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_set_warning_alert(&dev, WARNING_CHANNEL - 1, WARNING_CURRENT)); // Set overcurrent security flag

        xSemaphoreGive(xGuiSemaphore);
    }

    ESP_LOGI(TAG, "Ina3231 has been inititated");

    vTaskDelay(pdMS_TO_TICKS(500));

    uint32_t measure_number = 0;

    float bus_voltage;
    float shunt_voltage;
    float shunt_current;

    while (1)
    {
        measure_number++;

        vTaskDelay(pdMS_TO_TICKS(INA3221_CURRENT_SENSOR_IN_MS));

#if !MODE
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_trigger(&dev)); // Start a measure
            xSemaphoreGive(xGuiSemaphore);
        }

        do
        {
            if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
            {
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_status(&dev)); // get mask

                xSemaphoreGive(xGuiSemaphore);
            }

            vTaskDelay(100 / portTICK_PERIOD_MS);

        } while (!(dev.mask.cvrf)); // check if measure done
#else
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_status(&dev)); // get mask
            xSemaphoreGive(xGuiSemaphore);
        }

        if (dev.mask.wf & (1 << (3 - WARNING_CHANNEL)))
            warning = true;
#endif
        for (uint8_t i = 0; i < INA3221_BUS_NUMBER; i++)
        {

            if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
            {
                // Get voltage in volts
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_bus_voltage(&dev, i, &bus_voltage));
                // Get voltage in millivolts and current in milliamperes
                ESP_ERROR_CHECK_WITHOUT_ABORT(ina3221_get_shunt_value(&dev, i, &shunt_voltage, &shunt_current));
                xSemaphoreGive(xGuiSemaphore);
                INA3231_sensor_data[i].fBusVoltage = bus_voltage;
                INA3231_sensor_data[i].fShuntVoltage = shunt_voltage;
                INA3231_sensor_data[i].fShuntCurrent = shunt_current;
                ESP_LOGD(TAG, "Channel %d: Bus voltage: %.2f V, Shunt voltage: %.2f mV, Shunt current: %.2f mA", i, bus_voltage, shunt_voltage, shunt_current);
            }

            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}

void vGet_INA3221_sensor_data(INA3231_sensor_data_t *INA3221_sensor_data_des)
{
    memcpy(INA3221_sensor_data_des, INA3231_sensor_data, INA3221_CHANNEL * sizeof(INA3231_sensor_data_t));
}

void vGetExternalSensorData(external_sensor_data_t *external_sensor_data_des)
{
    memcpy(external_sensor_data_des, &external_sensor_data_average, sizeof(external_sensor_data_t));
}

/**
 * @brief This task function will average the sdp32 sample
 *
 * @param pvParameters
 */
void vTaskAverageDiffPressure(void *pvParameters)
{
    TickType_t last_wakeup = xTaskGetTickCount();
    while (1)
    {
        for (uint8_t i = 0; i < NO_OF_SAMPLES_SDP32; i++)
        {
            fSdp32_diff_pressure_average += noOfSamplesSdp32[i];
            fSdp32_diff_pressure_average = fSdp32_diff_pressure_average / NO_OF_SAMPLES_SDP32;
        }
        ESP_LOGV(TAG, "Average diff pressure is %f", fSdp32_diff_pressure_average);
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(SDP32_SENSOR_AVERAGE_DURATION_IN_MS));
    }
}

/**
 * @brief This task will calculate the diff pressure which is mass flaw compensation, temperature compensateion in the average mode.
 * The sensor should has been readed in the fast mode. The interval should be between 20ms-25ms. In this mode the masss flow compensated diff pressure is calculated in the arthemtic mean mode. If the sensor is calculated more than this duration the average will be in the exponential mean, which give wrong values. The samples values will be stored in the array. Later any function uses this array to calculate the average.
 *
 * @param pvParameters
 */
void vSdp32TaskWithAveraging(void *pvParameters)
{
    sdp32_t dev;
    memset(&dev, 0, sizeof(sdp32_t));

    /* read buffer 0 initialized */
    uint8_t read_buff[9] = {0};

    /* variable to store the checksum  */
    uint8_t checksum;

    /*  various sdp32 commands */
    // uint8_t startContRead_cmd[2] = {0x36, 0x15}; // Start Read in continous Mode
    uint8_t stopContRead_cmd[2] = {0x3F, 0xF9}; // Stop Countinous measure command
    /* mass flow, temperature compensated diff pressure with average mode */
    uint8_t massFlowRead[2] = {0x36, 0x03};

    /* initializing spd32 sensor */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdp32_init_desc(&dev, SDP32_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));
    vTaskDelay(pdMS_TO_TICKS(20));

    /* sending stop continuous read command */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        sdp32_send_cmd_read(&dev, stopContRead_cmd);
        xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    ESP_LOGI(TAG, "Starting sdp32 sensor in massflow mode");
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        sdp32_send_cmd_read(&dev, massFlowRead);
        xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    /* Creating the task which does the averaging of the samples */
    xTaskCreate(vTaskAverageDiffPressure, "vTaskAverageDiffPressure", 2048, NULL, 5, NULL);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(SDP32_SENSOR_READ_DURATION_IN_MS));
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            sdp32_read_pressure(&dev, read_buff); // actually reading the mass flow
            xSemaphoreGive(xGuiSemaphore);
        }
        /* checksum is the third byte of the array after the first two bytes which ar actual readings */
        checksum = read_buff[2];

        /* checking the checksum, as the values we are getting are correct or not */
        bool check = CheckCrc(read_buff, 2, checksum);
        /* if checksum failed then continue the loop, else store the value in the array */
        if (!check)
        {
            continue;
        }
        else
        {
            int16_t massFlow = (read_buff[0] << 8) | read_buff[1];
            noOfSamplesSdp32[last_update_sensor_value_index] = ((float)massFlow) / SDP32_DIFF_PRESSURE_SCALE_FACTOR;
            if (last_update_sensor_value_index == NO_OF_SAMPLES_SDP32 - 1)
            {
                last_update_sensor_value_index = 0;
            }
            else
            {
                last_update_sensor_value_index++;
            }
            int16_t Temprature = (read_buff[3] << 8) | read_buff[4];
            fSpd32Temperatuer_average = ((float)Temprature) / SDP32_DIFF_TEMPERATURE_SCALE_FACTOR;
        }
    }
    vTaskDelete(NULL);
}

float fGetSdp32DiffPressureAverageValue()
{
    return fSdp32_diff_pressure_average;
}

float fGetSdp32TemperatuerAverageValue()
{
    return fSpd32Temperatuer_average;
}

void vInitiateSensorsOnBoard()
{
    vInitiateRTCSensor();
    vTaskDelay(1000);
    xTaskCreate(vExternalBME680SensorTask, "bme680", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);

    /* create the various tasks to read the sensor data raw values and avbgerage them */
    xTaskCreate(Internal_Seneor_bme280_task, "bme280", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(vSdp32TaskWithAveraging, "sdp32", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(vIna3221_Sensor_task, "ina3231", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
    xTaskCreate(vAverageBMPValue, "bmp_average_task", 4 * 1024, NULL, 5, NULL);
    vTaskDelay(1000);
}

void vAverageBMPValue(void *pvParameters)
{
    TickType_t last_wakeup = xTaskGetTickCount();

    float temp_bme280_temperature_average = 0;
    float temp_bme280_pressure_average = 0;
    float temp_bme280_humidity_average = 0;

    while (1)
    {

        /*  initializing the average values to 0 */
        temp_bme280_temperature_average = 0;
        temp_bme280_pressure_average = 0;
        temp_bme280_humidity_average = 0;

        external_sensor_data_t temp_external_sensor_data_average = {0};

        for (uint8_t i = 0; i < NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP; i++)
        {
            /*  calculating values for the bme280 */
            temp_bme280_temperature_average += bme280_temperature_array[i];
            temp_bme280_pressure_average += bme280_pressure_array[i];
            temp_bme280_humidity_average += bme280_humidity_array[i];

            /* calculating values for the bme680 */
            temp_external_sensor_data_average.fTemperature += external_sensor_data[i].fTemperature;
            temp_external_sensor_data_average.fPressure += external_sensor_data[i].fPressure;
            temp_external_sensor_data_average.fHumidity += external_sensor_data[i].fHumidity;
            temp_external_sensor_data_average.fGasResistance += external_sensor_data[i].fGasResistance;
        }

        temp_bme280_temperature_average = temp_bme280_temperature_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
        temp_bme280_pressure_average = temp_bme280_pressure_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
        temp_bme280_humidity_average = temp_bme280_humidity_average / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;

        bme280_temperature_average = temp_bme280_temperature_average;
        bme280_pressure_average = temp_bme280_pressure_average;
        bme280_humidity_average = temp_bme280_humidity_average;

        temp_external_sensor_data_average.fPressure = temp_external_sensor_data_average.fPressure / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
        temp_external_sensor_data_average.fTemperature = temp_external_sensor_data_average.fTemperature / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
        temp_external_sensor_data_average.fHumidity = temp_external_sensor_data_average.fHumidity / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;
        temp_external_sensor_data_average.fGasResistance = temp_external_sensor_data_average.fGasResistance / NUMBER_OF_SAMPLE_VALUES_FOR_AVERAGE_BMP;

        external_sensor_data_average.fPressure = temp_external_sensor_data_average.fPressure;
        external_sensor_data_average.fTemperature = temp_external_sensor_data_average.fTemperature;
        external_sensor_data_average.fHumidity = temp_external_sensor_data_average.fHumidity;
        external_sensor_data_average.fGasResistance = temp_external_sensor_data_average.fGasResistance;

        ESP_LOGD(TAG, "External sensor Temp: %0.2f, Humidity: %0.2f, Pressure: %0.2f", external_sensor_data_average.fTemperature, external_sensor_data_average.fHumidity, external_sensor_data_average.fPressure);
        ESP_LOGD(TAG, "Internal sensor Temp: %0.2f, Humidity: %0.2f, Pressure: %0.2f", bme280_temperature_average, bme280_humidity_average, bme280_pressure_average);
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(BME_280_AVERAGING_TIME_IN_MS));
    }
}
