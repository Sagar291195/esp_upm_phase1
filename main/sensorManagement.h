#ifndef __SENSORMANAGEMENT_H__
#define __SENSORMANAGEMENT_H__

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <stdio.h>
#include <esp_err.h>

#include "esp_upm.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define INA3221_CHANNEL                         3       //!< Number of channels in INA3221
#define EXTERNAL_SENSOR_TEMPERATURE_MIN_VALUE   -10     /* lower range of the external temperature sensor */
#define EXTERNAL_SENSOR_TEMPERATURE_MAX_VALUE   45      /* higher range of the external temperature sensor */
#define EXTERNAL_SENSOR_HUMIDITY_MIN_VALUE      0       /* Lower range of the external humidity sensor */
#define EXTERNAL_SENSOR_HUMIDITY_MAX_VALUE      95      /* higher range of External sensor humidity value */
#define EXTERNAL_SENSOR_PRESSURE_MIN_VALUE      650     /* Lower range of External sensor pressure value */
#define EXTERNAL_SENOSR_PRESSURE_MAX_VALUE      1100    /* higher range of the external sensor pressure value */
#define HEAD_LOSS_MIN_VALUE                     70      /* head loss is differece between the internal pressure and external pressure headloss mininum value*/
#define HEAD_LOSS_MAX_VALUE                     145     /* head loss maximum value */
#define MAX_TOLERACE_IN_HUMIDITY                5.0     /* Maximum tolerance of humidity in percentage */
#define MAX_TOLERACE_IN_TEMPERATURE             5.0     /* Max tolerac in the temperature in degree celcius */
#define MAX_TOLERACE_IN_PRESSURE                5.0     /* Maximum tolerance in the pressure in hPa */
#define MAX_TOLERACE_IN_HEAD_LOSS               5.0     /* maximum tolerance in the head loss */

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
/* INA sensor data */
typedef struct INA3231_sensor_data
{
    float fBusVoltage;
    float fShuntVoltage;
    float fShuntCurrent;
} INA3231_sensor_data_t;


/* data structure for the external sensor */
typedef struct external_sensor_data
{
    float fTemperature;
    float fPressure;
    float fHumidity;
    float fGasResistance;
} external_sensor_data_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
/**
 * @brief Initializes all the sensor on the board
 */
void sensor_initialization();

/**
 * @brief get the average bme 280 temperature average values
 * @return float
 */
float get_internal_temperature_value(void);
/**
 * @brief get the humidity average values of bme 280
 * @return float
 */
float get_internal_humidity_value(void);

/**
 * @brief get the average pressure average values of bme 280
 * @return float
 */
float get_internal_pressure_value(void);

/**
 * @brief Give the ina3231 sensor data of all three channels
 * @snippet: INA3231_sensor_data_t ina3221_sensor_data[INA3221_CHANNEL];
 *        vGetINA3231Data(&ina3221_sensor_data[0]);
 * @param INA3221_sensor_data_des data array of all the channels
 */
void get_ina3221_sensor_data(INA3231_sensor_data_t *INA3221_sensor_data_des);

/**
 * @brief Get the bme 680, external sensor data.
 * @snippet external_sensor_data_t external_sensor_data;
 *          get_external_sensor_data_raw(&external_sensor_data);
 * @param external_sensor_data_des 
 */
void get_external_sensor_data_raw(external_sensor_data_t *external_sensor_data_des);

/**
 * @brief get the differential pressure average value 
 * @return float 
 */
float get_sdp32_pressure_value();

/**
 * @brief get the sdp32 
 * @return float 
 */
float get_sdp32_temperature_value();

#endif // __SENSORMANAGEMENT_H__