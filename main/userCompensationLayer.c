/**
 * @file userCompensationLayer.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This contain the user compensation layer functions definations. All the data realted to the user compensation is caluelated and done here
 * @version 1.1
 * @date 2022-06-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <userCompensationLayer.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

#include "storage/calibration.h"
/**********************************************defines******************************/

#define TAG "user compensation"

/*************************************function prototypes******************************/

/****************************************function definations******************************/

float fGetVolumetricFlowUserCompensated()
{
    float result = fGetVolumetricFlow_featureData();
    /* apply the user compensation */
    ESP_LOGD(TAG, "user compensated Voulumetric flow is %0.2f", result);
    return result;
}

float fGetInternalPressureUserCompensated()
{
    float result = fGetBme280PressureAverages();
    /*  add some user compensation */
    result = result - getcalibrationvalue_int_pressure();
    ESP_LOGD(TAG, "Internal Pressure USER COMPENSATED is %0.2f", result);
    return result;
}

float fGetInternalHumidityUserCompesated()
{
    float result = fGetBme280HumidityAverages();

    /* add some user compensation */
    result = result - getcalibrationvalue_int_humidity();
    ESP_LOGD(TAG, "Internal Humidity USER COMPENSATED is %0.2f", result);
    return result;
}

float fGetInternalTemperatureUserCompesated()
{
    float result = fGetBme280TemperatureAverages();
    result = result - getcalibrationvalue_int_temperature(); /* add some user compensation */
    ESP_LOGD(TAG, "Internal Temperature user compensated is %0.2f", result);
    return result;
}

void vGetExternalSensorDataUserCompensated(external_sensor_data_t *xUserCompenstedValues)
{
    external_sensor_data_t xManuCompenstedValues;
    vGetExternalSensorData(&xManuCompenstedValues);

    xManuCompenstedValues.fTemperature = (xManuCompenstedValues.fTemperature - getcalibrationvalue_ext_temperature());
    xManuCompenstedValues.fPressure = (xManuCompenstedValues.fPressure - getcalibrationvalue_ext_pressure());
    xManuCompenstedValues.fTemperature = (xManuCompenstedValues.fHumidity - getcalibrationvalue_ext_humidity());
    /* do something to get the user compnsated values */
    memcpy(xUserCompenstedValues, &xManuCompenstedValues, sizeof(external_sensor_data_t));
    ESP_LOGD(TAG, "User compensated External sensor values are temp %0.2f humidiy %0.2f pressure %0.2f", xUserCompenstedValues->fTemperature, xUserCompenstedValues->fHumidity, xUserCompenstedValues->fPressure);
}


float fGetMassFlowUserCompensated(void)
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
    }else{
        coeffA = getcalibrationvalue_flow_coeffA3();
        coeffB = getcalibrationvalue_flow_coeffB3();
    }

    
    fResult = ((coeffA * fResult) + coeffB);
    return fResult;
}

/**
 * @brief calculates the air density
 *
 * @param fTemperature external temperature manufacture compensated
 * @param fHumidity externalt humidity manufacture compensated
 * @param fPressure external pressure manufacture compensated
 * @return float Air density
 */
static float _fDensityCalculation(float fTemperature, float fHumidity, float fPressure)
{
    float exponential =  exp((17.5043 * fTemperature) / (241.2 + fTemperature));

    float density = (1.0 / (287.06 * (fTemperature + 273.15)) *
           ((fPressure*100)- (230.617 * (fHumidity / 100.0) * exponential)));

    ESP_LOGD(TAG, "Density : %f", density);       
    return density;
}

/**
 * @brief Calutes the flow rate
 *
 * @param fDeltaPressure air mass flow
 * @param fDensity air density
 * @return float
 */
static float _fFlowCalculation(float fDeltaPressure, float fDensity)
{
    float fResult = 0.0;
    if (isnan(fDeltaPressure))
    {
        fResult = 0.0;
        return fResult;
    }

    // fResult = sqrt(fabs(fDeltaPressure)) / sqrt(fDensity) * 0.3758940103; // for test
    // fResult = (fDeltaPressure < 0.0) ? -fResult : fResult;                // check si négatif
    /* using the formula Q= F(dp)* den_char / den_real
     * Putting F=1 and den_char =1.2     
     * fResult = .118 * fDeltaPressure * 1.2/fDensity;
     * updating the flow rate to flow value =  0,759 * SDPvalue^05288 */
    
    fResult = (0.759 * (pow(fDeltaPressure, 0.5288)) * (1.2 / fDensity));
    ESP_LOGD(TAG, "Pressure  : %.2f, Density : %.02f, Flow : %.02f", fDeltaPressure, fDensity, fResult);
    return fResult;
}

float fGetExternal_AirDesity_Raw(void)
{
    float result =0;
    external_sensor_data_t xAverageData;

    vGetExternalSensorData(&xAverageData);    
    result =  _fDensityCalculation(xAverageData.fTemperature, xAverageData.fHumidity, xAverageData.fPressure);
    return result;

}

float fGetInternalAirDensity_Raw()
{
    return _fDensityCalculation(fGetBme280TemperatureAverages(),fGetBme280HumidityAverages(),fGetBme280PressureAverages());
}

float fGetExternal_AirDesity_Comp(void)
{
    float result =0;
    external_sensor_data_t comp_sensordata;

    vGetExternalSensorDataUserCompensated(&comp_sensordata);;    
    result =  _fDensityCalculation(comp_sensordata.fTemperature, comp_sensordata.fHumidity, comp_sensordata.fPressure);
    return result;

}

float fGetInternalAirDensity_Comp(void)
{
    return _fDensityCalculation(fGetInternalTemperatureUserCompesated(), fGetInternalHumidityUserCompesated(), fGetInternalPressureUserCompensated());
}

float fGetVolumetricFlow_featureData()
{
    float result =0;
    result = _fFlowCalculation(fGetMassFlowUserCompensated(), fGetExternal_AirDesity_Comp());
    return result;
}