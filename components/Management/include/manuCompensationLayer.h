

/**
 * @file manuCompensationLayer.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief header files for the manufacturer compensation layer
 * @version 1.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */




#ifndef __MANUCOMPENSATIONLAYER_H__
#define __MANUCOMPENSATIONLAYER_H__

#include <stdio.h>
#include <sensorManagement.h>
#include <featureData.h>

/*******************************function prototype***************************************/
/**
 * @brief get the external temperature from the sensor after manufacture compensation values
 * @param xManuCompenstedValues
 */
void vGetMaufCompensatedExternalSensorData(external_sensor_data_t *xManuCompenstedValues);


/**
 * @brief Get the volume flow manufacture compensated
 * @return float
 */
float fGetVolumeFlowFromManuCompensationLayer();


/**
 * @brief get the manufacture compensated average mass flow rate
 * @return float
 */
float fGetMassFlowManuCompensationLayer();

/**
 * @brief get the manufacture compensated average External air density.
 * @return float
 */
float fGetAirDensityManuCompensationLayer();

/**
 * @brief get the manufacture compensated internal pressure
 * @return float
 */
float fGetInternalPressureManuCompensationLayer();

/**
 * @brief get the manufacture compensated internal temperature
 * @return float
 */
float fGetInternalTempretureManuCompensationLayer();

/**
 * @brief get the internal humidity from the sensor after manufacture compensation values
 * @return float
 */
float fGetInternalHumidityManuCompensationLayer();

 #endif // __MANUCOMPENSATIONLAYER_H__