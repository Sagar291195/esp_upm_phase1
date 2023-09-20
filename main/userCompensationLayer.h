#ifndef __USERCOMPENSATIONLAYER_H__
#define __USERCOMPENSATIONLAYER_H__


/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_upm.h"
#include <sensorManagement.h>

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

float fGetInternalPressureUserCompensated();
float fGetVolumetricFlowUserCompensated();
float fGetInternalHumidityUserCompesated();
float fGetInternalTemperatureUserCompesated();
void get_external_sensor_calibratedvalue(external_sensor_data_t *calibratedvalue);
float fGetMassFlowUserCompensated(void);
float fGetExternal_AirDesity_Raw();
float fGetInternalAirDensity_Raw();
float fGetVolumetricFlow_featureData();
float fGetExternal_AirDesity_Comp(void);
float fGetInternalAirDensity_Comp(void);

#endif // __USERCOMPENSATIONLAYER_H__