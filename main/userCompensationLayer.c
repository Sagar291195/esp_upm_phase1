/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <userCompensationLayer.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

#include "storage/calibration.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG         "User compensation"
/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static float _fFlowCalculation(float fDeltaPressure, float fDensity);
static float _fDensityCalculation(float fTemperature, float fHumidity, float fPressure);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
*                           
********************************************************************************************/
static float _fDensityCalculation(float fTemperature, float fHumidity, float fPressure)
{
    float exponential =  exp((17.5043 * fTemperature) / (241.2 + fTemperature));

    float density = (1.0 / (287.06 * (fTemperature + 273.15)) *
           ((fPressure*100)- (230.617 * (fHumidity / 100.0) * exponential)));

    ESP_LOGD(TAG, "Density : %f", density);       
    return density;
}

/********************************************************************************************
*                           
********************************************************************************************/
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

/********************************************************************************************
*                           GLOBAL FUNCTIONS
********************************************************************************************/
 
/********************************************************************************************
*                           
********************************************************************************************/
float fGetVolumetricFlowUserCompensated()
{
    float result = fGetVolumetricFlow_featureData();
    /* apply the user compensation */
    ESP_LOGD(TAG, "user compensated Voulumetric flow is %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetInternalPressureUserCompensated()
{
    float result = get_internal_pressure_value();
    /*  add some user compensation */
    result = result - getcalibrationvalue_int_pressure();
    ESP_LOGD(TAG, "Internal Pressure USER COMPENSATED is %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetInternalHumidityUserCompesated()
{
    float result = get_internal_humidity_value();

    /* add some user compensation */
    result = result - getcalibrationvalue_int_humidity();
    ESP_LOGD(TAG, "Internal Humidity USER COMPENSATED is %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetInternalTemperatureUserCompesated()
{
    float result = get_internal_temperature_value();
    result = result - getcalibrationvalue_int_temperature(); /* add some user compensation */
    ESP_LOGD(TAG, "Internal Temperature user compensated is %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
void get_external_sensor_calibratedvalue(external_sensor_data_t *calibratedvalue)
{
    external_sensor_data_t sensorvalues;

    get_external_sensor_data(&sensorvalues);
    float offset_temp = getcalibrationvalue_ext_temperature();
    float offset_pressure = getcalibrationvalue_ext_pressure();
    float offset_humidity = getcalibrationvalue_ext_humidity();
    calibratedvalue->fTemperature = (sensorvalues.fTemperature - offset_temp);
    calibratedvalue->fPressure = (sensorvalues.fPressure - offset_pressure);
    calibratedvalue->fHumidity = (sensorvalues.fHumidity - offset_humidity);
    ESP_LOGD(TAG, "Calibrated External sensor values are temp %0.2f humidiy %0.2f pressure %0.2f", calibratedvalue->fTemperature, calibratedvalue->fHumidity, calibratedvalue->fPressure);
}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetMassFlowUserCompensated(void)
{
    float fResult = 0.0;
    float coeffA = 0.0;
    float coeffB = 0.0;

    fResult = get_sdp32_pressure_value();
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



/********************************************************************************************
*                           
********************************************************************************************/
float fGetExternal_AirDesity_Raw(void)
{
    float result =0;
    external_sensor_data_t xAverageData;

    get_external_sensor_data(&xAverageData);    
    result =  _fDensityCalculation(xAverageData.fTemperature, xAverageData.fHumidity, xAverageData.fPressure);
    return result;

}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetInternalAirDensity_Raw()
{
    return _fDensityCalculation(get_internal_temperature_value(),get_internal_humidity_value(),get_internal_pressure_value());
}

/********************************************************************************************
*                           
********************************************************************************************/
float fGetExternal_AirDesity_Comp(void)
{
    float result =0;
    external_sensor_data_t calibratedsensordata;

    get_external_sensor_calibratedvalue(&calibratedsensordata);;    
    result =  _fDensityCalculation(calibratedsensordata.fTemperature, calibratedsensordata.fHumidity, calibratedsensordata.fPressure);
    return result;

}

 /********************************************************************************************
 *                           
 ********************************************************************************************/
float fGetInternalAirDensity_Comp(void)
{
    return _fDensityCalculation(fGetInternalTemperatureUserCompesated(), fGetInternalHumidityUserCompesated(), fGetInternalPressureUserCompensated());
}

 /********************************************************************************************
 *                           
 ********************************************************************************************/
float fGetVolumetricFlow_featureData()
{
    float result =0;
    result = _fFlowCalculation(fGetMassFlowUserCompensated(), fGetExternal_AirDesity_Comp());
    return result;
}