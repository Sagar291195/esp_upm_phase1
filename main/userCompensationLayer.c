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
static float flow_calculation(float differential_pressure, float airdensity);
static float density_calculation(float temperature, float humidity, float pressure);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
*                           
********************************************************************************************/
static float density_calculation(float temperature, float humidity, float pressure)
{
    float exponential =  exp((17.5043 * temperature) / (241.2 + temperature));

    float density = (1.0 / (287.06 * (temperature + 273.15)) *
           ((pressure*100)- (230.617 * (humidity / 100.0) * exponential)));

    ESP_LOGD(TAG, "Density : %f", density);       
    return density;
}

/********************************************************************************************
*                           
********************************************************************************************/
static float flow_calculation(float differential_pressure, float airdensity)
{
    float fResult = 0.0;
    if (isnan(differential_pressure))
    {
        fResult = 0.0;
        return fResult;
    }

    // fResult = sqrt(fabs(differential_pressure)) / sqrt(fDensity) * 0.3758940103; // for test
    // fResult = (differential_pressure < 0.0) ? -fResult : fResult;                // check si négatif
    /* using the formula Q= F(dp)* den_char / den_real
     * Putting F=1 and den_char =1.2     
     * fResult = .118 * differential_pressure * 1.2/fDensity;
     * updating the flow rate to flow value =  0,759 * SDPvalue^05288 */
    
    fResult = (0.759 * (pow(differential_pressure, 0.5288)) * (1.2 / airdensity));
    ESP_LOGD(TAG, "Pressure  = %.2f, Density = %.02f, Flow = %.02f", differential_pressure, airdensity, fResult);
    return fResult;
}

/********************************************************************************************
*                           GLOBAL FUNCTIONS
********************************************************************************************/
 


/********************************************************************************************
*                           
********************************************************************************************/
float get_internal_pressure_calibrated(void)
{
    float result = get_internal_pressure_value();
    result = result - getcalibrationvalue_int_pressure();   
    ESP_LOGD(TAG, "Internal Pressure calibrated =  %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
float get_internal_humidity_calibrated(void)
{
    float result = get_internal_humidity_value();
    result = result - getcalibrationvalue_int_humidity();    
    ESP_LOGD(TAG, "Internal Humidity calibrated = %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
float get_internal_temperature_calibrated(void)
{
    float result = get_internal_temperature_value();
    result = result - getcalibrationvalue_int_temperature(); /* add some user compensation */
    ESP_LOGD(TAG, "Internal Temperature calibrated = %0.2f", result);
    return result;
}

/********************************************************************************************
*                           
********************************************************************************************/
void get_external_sensor_calibratedvalue(external_sensor_data_t *calibratedvalue)
{
    external_sensor_data_t sensorvalues;

    get_external_sensor_data_raw(&sensorvalues);
    float offset_temp = getcalibrationvalue_ext_temperature();
    float offset_pressure = getcalibrationvalue_ext_pressure();
    float offset_humidity = getcalibrationvalue_ext_humidity();
    calibratedvalue->temperature = (sensorvalues.temperature - offset_temp);
    calibratedvalue->pressure = (sensorvalues.pressure - offset_pressure);
    calibratedvalue->humidity = (sensorvalues.humidity - offset_humidity);
    ESP_LOGD(TAG, "Calibrated External sensor values are temperature %0.2f humidiy %0.2f pressure %0.2f", 
                        calibratedvalue->temperature, calibratedvalue->humidity, calibratedvalue->pressure);
}

/********************************************************************************************
*                           
********************************************************************************************/
float get_external_air_density_raw(void)
{
    float result =0;
    external_sensor_data_t raw_sensor_data;

    get_external_sensor_data_raw(&raw_sensor_data);    
    result = density_calculation(raw_sensor_data.temperature, raw_sensor_data.humidity, 
                    raw_sensor_data.pressure);
    return result;

}

/********************************************************************************************
*                           
********************************************************************************************/
float get_internal_air_density_raw()
{
    return density_calculation(get_internal_temperature_value(),get_internal_humidity_value(), 
                get_internal_pressure_value());
}

/********************************************************************************************
*                           
********************************************************************************************/
float get_external_air_density_calibrated(void)
{
    float result = 0;
    external_sensor_data_t calibratedsensordata;

    get_external_sensor_calibratedvalue(&calibratedsensordata);;    
    result = density_calculation(calibratedsensordata.temperature, calibratedsensordata.humidity, 
                    calibratedsensordata.pressure);
    return result;

}

 /********************************************************************************************
 *                           
 ********************************************************************************************/
float get_internal_air_density_calibrated(void)
{
    return density_calculation(get_internal_temperature_calibrated(), get_internal_humidity_calibrated(),
                    get_internal_pressure_calibrated());
}

 /********************************************************************************************
 *                           
 ********************************************************************************************/
float get_volumetric_flow(void)
{
    float result =0;
    result = flow_calculation(get_sdp32_massflow_value(), get_external_air_density_calibrated());
    return result;
}