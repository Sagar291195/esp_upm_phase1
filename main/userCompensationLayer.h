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

float get_internal_pressure_calibrated(void);
float get_internal_humidity_calibrated(void);
float get_internal_temperature_calibrated(void);
void get_external_sensor_calibratedvalue(external_sensor_data_t *calibratedvalue);
float get_volumetric_flow(void);
float get_external_air_density_raw(void);
float get_internal_air_density_raw(void);
float get_external_air_density_calibrated(void);
float get_internal_air_density_calibrated(void);

#endif // __USERCOMPENSATIONLAYER_H__