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


/********************************************function prototypes*****************************************************/

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
#endif // __FEATUREDATA_H__