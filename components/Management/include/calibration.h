
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include <stdint.h>
#include <stdbool.h>

/***************************************************************************************************
 *                              TYPEDEF
 ***************************************************************************************************/
/* structure to store calibration values */
typedef struct calibration_strct{
    float internal_temperature_calibration;     //internal temperature calibration offset
    float internal_humidity_calibration;        //internal humidity calibration offset
    float internal_pressure_calibration;        //internal pressure calibration offset
    float external_temperature_calibration;     //external temperature calibration offset
    float external_humidity_calibration;        //external humidity calibration offset
    float external_pressure_calibration;        //external_pressure_calibration offset
}calibrationt_t;

/***************************************************************************************************
 *                         FUNCTION PROTOTYPE
 ***************************************************************************************************/
/**
 * @brief Read calibration data from nvs flash
 * @param None
 * @return None
 *   
*/
void nvsread_calibrationdata(void);

/**
 * @brief Get external temperature calibration data 
 * @param None
 * @return float external temperature offset
 *   
*/
float getcalibrationvalue_ext_temperature(void);

/**
 * @brief Get external pressure calibration data 
 * @param None
 * @return float external pressure offset
 *   
*/
float getcalibrationvalue_ext_pressure(void);

/**
 * @brief Get external humidity calibration data 
 * @param None
 * @return float external hunidity offset
 *   
*/
float getcalibrationvalue_ext_humidity(void);

/**
 * @brief Get internal temperature calibration data 
 * @param None
 * @return float internal temperature offset
 *   
*/
float getcalibrationvalue_int_temperature(void);

/**
 * @brief Get internal pressure calibration data 
 * @param None
 * @return float internal pressure offset
 *   
*/
float getcalibrationvalue_int_pressure(void);

/**
 * @brief Get internal humidity calibration data 
 * @param None
 * @return float internal humidity offset
 *   
*/
float getcalibrationvalue_int_humidity(void);


/**
 * @brief Get external temperature calibration data 
 * @param value external temperature offset value
 * @return None
 *   
*/
void setcalibrationvalue_ext_temperature(float value);

/**
 * @brief Get external pressure calibration data 
 * @param value external pressure offset value
 * @return None
 *   
*/
void setcalibrationvalue_ext_pressure(float value);

/**
 * @brief Get external humidity calibration data 
 * @param value external humidity offset value
 * @return None
 *   
*/
void setcalibrationvalue_ext_humidity(float value);

/**
 * @brief Get internal temperature calibration data 
 * @param value internal temperature offset value
 * @return None
 *   
*/
void setcalibrationvalue_int_temperature(float value);

/**
 * @brief Get internal pressure calibration data 
 * @param value internal pressure offset value
 * @return None
 *   
*/
void setcalibrationvalue_int_pressure(float value);

/**
 * @brief Get internal humidity calibration data 
 * @param value internal humidity offset value
 * @return None
 *   
*/
void setcalibrationvalue_int_humidity(float value);

#endif