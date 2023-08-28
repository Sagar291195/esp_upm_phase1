/**
 * @file featureData.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the header file for the featre data layer
 * @version 1.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */



#ifndef __FEATUREDATA_H__
#define __FEATUREDATA_H__


#include <stdio.h>
#include <sensorManagement.h>
#include <manuCompensationLayer.h>


/********************************************function prototypes*****************************************************/

/**
 * @brief get the air density as the feature data
 *
 * @return float
 */
float fGetAirDesity_featureData();


/**
 * @brief Get the internal air density as feature data
 *
 * @return float
 */
float fGetInternalAirDensity_featureData();


/**
 * @brief get the volumentric flow as the feature data
 *
 * @return float
 */
float fGetVolumetricFlow_featureData();

#endif // __FEATUREDATA_H__