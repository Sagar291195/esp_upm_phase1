/**
 * @file featureData.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the feature data layer This layer is responsible for all the data derivate from sensor
 * @version 1.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <featureData.h>
#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <math.h>
#include <sensorManagement.h>
#include "userCompensationLayer.h"


/***************************************defines***********************************************/
#define TAG "Feature Data"


/*****************************************function definations*************************************/
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
    result = _fFlowCalculation(fGetMassFlowManuCompensationLayer(), fGetExternal_AirDesity_Comp());
    return result;
}