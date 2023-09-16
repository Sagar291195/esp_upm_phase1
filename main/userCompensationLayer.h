/**
 * @file userCompensationLayer.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief header file for user compensation layer. all the funciton defination of the user compenstion will be available here
 * @version 1.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __USERCOMPENSATIONLAYER_H__
#define __USERCOMPENSATIONLAYER_H__

#include "esp_upm.h"
#include <sensorManagement.h>


/*****************************function prototype***************************************/


/**
 * @brief get the internal pressure user compensated
 * @return float 
 */
float fGetInternalPressureUserCompensated();

float fGetVolumetricFlowUserCompensated();
/**
 * @brief Get internal Humidity user compensated
 * @return float 
 */
float fGetInternalHumidityUserCompesated();

/**
 * @brief Get the Internal temperature user compensated
 * @return float 
 */
float fGetInternalTemperatureUserCompesated();

/**
 * @brief Get the external sensor data user compensaed 
 * @param[in] xUserCompenstedValues 
 */
void get_external_sensor_calibratedvalue(external_sensor_data_t *calibratedvalue);

float fGetMassFlowUserCompensated(void);

/**
 * @brief get the air density as the feature data
 * @return float
 */
float fGetExternal_AirDesity_Raw();


/**
 * @brief Get the internal air density as feature data
 * @return float
 */
float fGetInternalAirDensity_Raw();


/**
 * @brief get the volumentric flow as the feature data
 * @return float
 */
float fGetVolumetricFlow_featureData();
float fGetExternal_AirDesity_Comp(void);
float fGetInternalAirDensity_Comp(void);
#endif // __USERCOMPENSATIONLAYER_H__