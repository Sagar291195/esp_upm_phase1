/**
 * @file manuCompensationLayer.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This contain the manufacturing layer compensation. All the data realted to the manufacture compensation is caluelated and done here
 * @version 1.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sensorManagement.h>
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include <featureData.h>

#include <calibration.h>

/*************************************defines****************************************/

#define TAG "Manufacture Compensation"

/***********************************************function definations*********************/

void vGetMaufCompensatedExternalSensorData(external_sensor_data_t *xManuCompenstedValues)
{
    external_sensor_data_t xAverageData;
    vGetExternalSensorData(&xAverageData);
    /*  DO some calculation to get the manufacturing compensation values */
    memcpy(xManuCompenstedValues, &xAverageData, sizeof(external_sensor_data_t));

    ESP_LOGD(TAG, "Manufactrue compensated external sensor values are temp %0.2f humidiy %0.2f pressure %0.2f", xManuCompenstedValues->fTemperature, xManuCompenstedValues->fHumidity, xManuCompenstedValues->fPressure);
}

float fGetVolumeFlowFromManuCompensationLayer()
{
    float fResult = 0.0;
    fResult = fGetVolumetricFlow_featureData();
    /* do some calulation to get the volume flow */
    return fResult;
}

float fGetMassFlowManuCompensationLayer()
{
    float fResult = 0.0;
    float coeffA = 0.0;
    float coeffB = 0.0;

    fResult = fGetSdp32DiffPressureAverageValue();
    /*  do some calulation to get the mass flow  */
    if (fResult > 0 && fResult <= getcalibration_reference_sensorvalue1())
    {
        coeffA = getcalibrationvalue_flow_coeffA1();
        coeffB = getcalibrationvalue_flow_coeffB1();
    }
    else if (fResult <= getcalibration_reference_sensorvalue2())
    {
        coeffA = getcalibrationvalue_flow_coeffA2();
        coeffB = getcalibrationvalue_flow_coeffB2();
    }
    else if (fResult <= getcalibration_reference_sensorvalue3())
    {
        coeffA = getcalibrationvalue_flow_coeffA3();
        coeffB = getcalibrationvalue_flow_coeffB3();
    }

    
    fResult = ((coeffA * fResult) + coeffB);
    return fResult;
}

float fGetAirDensityManuCompensationLayer()
{
    float fResult = 0.0;
    fResult = fGetAirDesity_featureData();
    /* do some calulation to get the air density */
    return fResult;
}

float fGetInternalPressureManuCompensationLayer()
{
    float fResult = 0.0;
    fResult = fGetBme280PressureAverages();
    /* do some calulation to get the internal pressure */
    return fResult;
}

float fGetInternalTempretureManuCompensationLayer()
{
    float fResult = 0.0;
    fResult = fGetBme280TemperatureAverages();
    /* do some calulation to get the internal temperature */
    return fResult;
}

float fGetInternalHumidityManuCompensationLayer()
{
    float fResult = 0.0;
    fResult = fGetBme280HumidityAverages();
    /* do some calulation to get the internal humidity */
    return fResult;
}

float fGetInternalAirDensityManuCompensationLayer()
{
    float fResult = 0;
    fResult = fGetInternalAirDensity_featureData();

    return fResult;
}
