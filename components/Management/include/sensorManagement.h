

/**
 * @file sensorManagement.h
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief this header file managed all the sensor present in the system.
 * @version 1.1
 * @date 2022-06-21
 *
 * @copyright Copyright (c) 2022
 *
 */


#ifndef __SENSORMANAGEMENT_H__
#define __SENSORMANAGEMENT_H__

#include <stdio.h>
#include <bme680.h>
#include <bmp280.h>
#include <esp_err.h>


/********************************defines*******************************************/

#define INA3221_CHANNEL 3   //!< Number of channels in INA3221
/*  lower range of the external temperature sensor */
#define EXTERNAL_SENSOR_TEMPERATURE_MIN_VALUE -10
/* higher range of the external temperature sensor */
#define EXTERNAL_SENSOR_TEMPERATURE_MAX_VALUE 45
/* Lower range of the external humidity sensor */
#define EXTERNAL_SENSOR_HUMIDITY_MIN_VALUE 0
/* higher range of External sensor humidity value */
#define EXTERNAL_SENSOR_HUMIDITY_MAX_VALUE 95
/* Lower range of External sensor pressure value */
#define EXTERNAL_SENSOR_PRESSURE_MIN_VALUE 650
/* higher range of the external sensor pressure value */
#define EXTERNAL_SENOSR_PRESSURE_MAX_VALUE 1100
 /* head loss is differece between the internal pressure and external pressure headloss mininum value*/
#define HEAD_LOSS_MIN_VALUE 70
/* head loss maximum value */
#define HEAD_LOSS_MAX_VALUE 145
/* Maximum tolerance of humidity in percentage */
#define MAX_TOLERACE_IN_HUMIDITY 5.0
/* Max tolerac in the temperature in degree celcius */
#define MAX_TOLERACE_IN_TEMPERATURE 5.0
/* Maximum tolerance in the pressure in hPa */
#define MAX_TOLERACE_IN_PRESSURE 5.0
 /* maximum tolerance in the head loss */
#define MAX_TOLERACE_IN_HEAD_LOSS 5.0

/**********************************************************struct***************************************************/
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

/**********************************************function prototypes*****************************************************/

/**
 * @brief Initializes all the sensor on the board
 */
void vInitiateSensorsOnBoard();

/**
 * @brief get the average bme 280 temperature average values
 * @return float
 */
float fGetBme280TemperatureAverages(void);
/**
 * @brief get the humidity average values of bme 280
 * @return float
 */
float fGetBme280HumidityAverages(void);

/**
 * @brief get the average pressure average values of bme 280
 * @return float
 */
float fGetBme280PressureAverages(void);

/**
 * @brief Give the ina3231 sensor data of all three channels
 * @snippet: INA3231_sensor_data_t INA3231_sensor_data[INA3221_CHANNEL];
 *        vGetINA3231Data(&INA3231_sensor_data[0]);
 * @param INA3221_sensor_data_des data array of all the channels
 */
void vGet_INA3221_sensor_data(INA3231_sensor_data_t *INA3221_sensor_data_des);

/**
 * @brief Get the bme 680, external sensor data.
 * @snippet external_sensor_data_t external_sensor_data;
 *          vGetExternalSensorData(&external_sensor_data);
 * @param external_sensor_data_des 
 */
void vGetExternalSensorData(external_sensor_data_t *external_sensor_data_des);

/**
 * @brief get the differential pressure average value 
 * @return float 
 */
float fGetSdp32DiffPressureAverageValue();

/**
 * @brief get the sdp32 
 * @return float 
 */
float fGetSdp32TemperatuerAverageValue();



#endif // __SENSORMANAGEMENT_H__