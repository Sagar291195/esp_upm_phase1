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

#include <sensorManagement.h>
#include <manuCompensationLayer.h>



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
void vGetExternalSensorDataUserCompensated(external_sensor_data_t *xUserCompenstedValues);


#endif // __USERCOMPENSATIONLAYER_H__