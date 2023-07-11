
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include <stdint.h>
#include <stdbool.h>


typedef struct calibration_strct{
    float internal_temperature_calibration;
    float internal_humidity_calibration;
    float internal_pressure_calibration;
    float external_temperature_calibration;
    float external_humidity_calibration;
    float external_pressure_calibration;
}calibrationt_t;

void nvsread_calibrationdata(void);
float getcalibrationvalue_ext_temperature(void);
float getcalibrationvalue_ext_pressure(void);
float getcalibrationvalue_ext_humidity(void);
float getcalibrationvalue_int_temperature(void);
float getcalibrationvalue_int_pressure(void);
float getcalibrationvalue_int_humidity(void);
void setcalibrationvalue_ext_temperature(float value);
void setcalibrationvalue_ext_pressure(float value);
void setcalibrationvalue_ext_humidity(float value);
void setcalibrationvalue_int_temperature(float value);
void setcalibrationvalue_int_pressure(float value);
void setcalibrationvalue_int_humidity(float value);

#endif