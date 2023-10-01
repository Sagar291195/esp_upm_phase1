/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "flash.h"
#include "calibration.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                         "CALIBRATION"
#define CALIBRATION_STORGE_NAME     "calibration"

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

 /********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
static char ext_temperaturekey[] = "ext-temp";  // key to store data in flash
static char ext_pressurekey[] = "ext-pressure"; // key to store data in flash
static char ext_humiditykey[] = "ext-humidity"; // key to store data in flash
static char int_temperaturekey[] = "int-temp";  // key to store data in flash
static char int_pressurekey[] = "int-pressure"; // key to store data in flash
static char int_huniditykey[] = "int-humidity"; // key to store data in flash
static char flow_coeffA1[] = "flow_coeffA1";    // key to store data in flash
static char flow_coeffB1[] = "flow_coeffB1";    // key to store data in flash
static char flow_coeffA2[] = "flow_coeffA2";    // key to store data in flash
static char flow_coeffB2[] = "flow_coeffB2";    // key to store data in flash
static char flow_coeffA3[] = "flow_coeffA3";    // key to store data in flash
static char flow_coeffB3[] = "flow_coeffB3";    // key to store data in flash
static char sensorvalue1[] = "sensorvalue1";
static char sensorvalue2[] = "sensorvalue2";
static char sensorvalue3[] = "sensorvalue3";

calibration_t calibrationdata; // variable to store calibration data

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                
 ********************************************************************************************/
void nvsread_calibrationdata(void)
{
    bool ret = false;

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, ext_temperaturekey, &calibrationdata.external_temperature_calibration);
    if (ret == false)
    {
        calibrationdata.external_temperature_calibration = 0;
        ESP_LOGE(TAG, "External Temperature value read error");
    }
    else
    {
        ESP_LOGI(TAG, "External Temperature Calibration = %.02f", calibrationdata.external_temperature_calibration);
    }
    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, ext_humiditykey, &calibrationdata.external_humidity_calibration);
    if (ret == false)
    {
        calibrationdata.external_humidity_calibration = 0;
        ESP_LOGE(TAG, "External Humidity value read error");
    }
    else
    {
        ESP_LOGI(TAG, "External Humidity Calibration = %.02f", calibrationdata.external_humidity_calibration);
    }
    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, ext_pressurekey, &calibrationdata.external_pressure_calibration);
    if (ret == false)
    {
        calibrationdata.external_pressure_calibration = 0;
        ESP_LOGE(TAG, "External pressure value read error");
    }
    else
    {
        ESP_LOGI(TAG, "External pressure Calibration = %.02f", calibrationdata.external_pressure_calibration);
    }
    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, int_temperaturekey, &calibrationdata.internal_temperature_calibration);
    if (ret == false)
    {
        calibrationdata.internal_temperature_calibration = 0;
        ESP_LOGE(TAG, "Internal Temperature value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Internal Temperature Calibration = %.02f", calibrationdata.internal_temperature_calibration);
    }
    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, int_pressurekey, &calibrationdata.internal_pressure_calibration);
    if (ret == false)
    {
        calibrationdata.internal_pressure_calibration = 0;
        ESP_LOGE(TAG, "Internal Pressure value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Internal Pressure Calibration = %.02f", calibrationdata.internal_pressure_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, int_huniditykey, &calibrationdata.internal_humidity_calibration);
    if (ret == false)
    {
        calibrationdata.internal_humidity_calibration = 0;
        ESP_LOGE(TAG, "Internal Humidity value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Internal humidity Calibration = %.02f", calibrationdata.internal_humidity_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffA1, &calibrationdata.flow_coeffA1_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffA1_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff A1 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff A1 Calibration = %.02f", calibrationdata.flow_coeffA1_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffA2, &calibrationdata.flow_coeffA2_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffA2_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff A2 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff A2 Calibration = %.02f", calibrationdata.flow_coeffA2_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffA3, &calibrationdata.flow_coeffA3_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffA3_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff A3 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff A3 Calibration = %.02f", calibrationdata.flow_coeffA3_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffB1, &calibrationdata.flow_coeffB1_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffB1_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff B1 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff B1 Calibration = %.02f", calibrationdata.flow_coeffB1_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffB2, &calibrationdata.flow_coeffB2_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffB2_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff B2 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff B2 Calibration = %.02f", calibrationdata.flow_coeffB2_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, flow_coeffB3, &calibrationdata.flow_coeffB3_calibration);
    if (ret == false)
    {
        calibrationdata.flow_coeffB3_calibration = 1;
        ESP_LOGE(TAG, "Flow coeff B3 value read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow coeff B3 Calibration = %.02f", calibrationdata.flow_coeffB3_calibration);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, sensorvalue1, &calibrationdata.flow_reference_sensorvalue1);
    if (ret == false)
    {
        ESP_LOGE(TAG, "Flow sensor value1 read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow sensor value 1 = %.02f", calibrationdata.flow_reference_sensorvalue1);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, sensorvalue2, &calibrationdata.flow_reference_sensorvalue2);
    if (ret == false)
    {
        ESP_LOGE(TAG, "Flow sensor value2 read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow sensor value 2 = %.02f", calibrationdata.flow_reference_sensorvalue2);
    }

    ret = nvsread_value_parameter(CALIBRATION_STORGE_NAME, sensorvalue3, &calibrationdata.flow_reference_sensorvalue3);
    if (ret == false)
    {
        ESP_LOGE(TAG, "Flow sensor value3 read error");
    }
    else
    {
        ESP_LOGI(TAG, "Flow sensor value 2 = %.02f", calibrationdata.flow_reference_sensorvalue3);
    }
}


/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_ext_temperature(void)
{
    return calibrationdata.external_temperature_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_ext_pressure(void)
{
    return calibrationdata.external_pressure_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_ext_humidity(void)
{
    return calibrationdata.external_humidity_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_int_temperature(void)
{
    return calibrationdata.internal_temperature_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_int_pressure(void)
{
    return calibrationdata.internal_pressure_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_int_humidity(void)
{
    return calibrationdata.internal_humidity_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_flow_coeffA1(void)
{
    return calibrationdata.flow_coeffA1_calibration;
}

float getcalibrationvalue_flow_coeffA2(void)
{
    return calibrationdata.flow_coeffA2_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_flow_coeffA3(void)
{
    return calibrationdata.flow_coeffA3_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_flow_coeffB1(void)
{
    return calibrationdata.flow_coeffB1_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_flow_coeffB2(void)
{
    return calibrationdata.flow_coeffB2_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibrationvalue_flow_coeffB3(void)
{
    return calibrationdata.flow_coeffB3_calibration;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibration_reference_sensorvalue1(void)
{
    return calibrationdata.flow_reference_sensorvalue1;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibration_reference_sensorvalue2(void)
{
    return calibrationdata.flow_reference_sensorvalue2;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
float getcalibration_reference_sensorvalue3(void)
{
    return calibrationdata.flow_reference_sensorvalue3;
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_ext_temperature(float value)
{
    calibrationdata.external_temperature_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, ext_temperaturekey, calibrationdata.external_temperature_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_ext_pressure(float value)
{
    calibrationdata.external_pressure_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, ext_pressurekey, calibrationdata.external_pressure_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_ext_humidity(float value)
{
    calibrationdata.external_humidity_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, ext_humiditykey, calibrationdata.external_humidity_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_int_temperature(float value)
{
    calibrationdata.internal_temperature_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, int_temperaturekey, calibrationdata.internal_temperature_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_int_pressure(float value)
{
    calibrationdata.internal_pressure_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, int_pressurekey, calibrationdata.internal_pressure_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_int_humidity(float value)
{
    calibrationdata.internal_humidity_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, int_huniditykey, calibrationdata.internal_humidity_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffA1(float value)
{
    calibrationdata.flow_coeffA1_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffA1, calibrationdata.flow_coeffA1_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffA2(float value)
{
    calibrationdata.flow_coeffA2_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffA2, calibrationdata.flow_coeffA2_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffA3(float value)
{
    calibrationdata.flow_coeffA3_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffA3, calibrationdata.flow_coeffA3_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffB1(float value)
{
    calibrationdata.flow_coeffB1_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffB1, calibrationdata.flow_coeffB1_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffB2(float value)
{
    calibrationdata.flow_coeffB2_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffB2, calibrationdata.flow_coeffB2_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibrationvalue_flow_coeffB3(float value)
{
    calibrationdata.flow_coeffB3_calibration = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, flow_coeffB3, calibrationdata.flow_coeffB3_calibration);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibration_flow_reference_sensorvalue1(float value)
{
    calibrationdata.flow_reference_sensorvalue1 = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, sensorvalue1, calibrationdata.flow_reference_sensorvalue1);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibration_flow_reference_sensorvalue2(float value)
{
    calibrationdata.flow_reference_sensorvalue2 = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, sensorvalue2, calibrationdata.flow_reference_sensorvalue2);
}

/********************************************************************************************
 *                
 ********************************************************************************************/
void setcalibration_flow_reference_sensorvalue3(float value)
{
    calibrationdata.flow_reference_sensorvalue3 = value;
    nvswrite_value_float(CALIBRATION_STORGE_NAME, sensorvalue3, calibrationdata.flow_reference_sensorvalue3);
}