/**********************************************************************
 *                              INCLUDES
***********************************************************************/

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include <calibration.h>
/**********************************************************************
 *                              DEFINES
***********************************************************************/
#define TAG "CALIBRATION"

#define CALIBRATION_STORGE_NAME         "calibration"



/**********************************************************************
 *                              TYPEDEF
***********************************************************************/

/**********************************************************************
 *                             VARIBALES
***********************************************************************/
char ext_temperaturekey[] = "ext-temp";
char ext_pressurekey[] = "ext-pressure";
char ext_humiditykey[] = "ext-humidity";
char int_temperaturekey[] = "int-temp";
char int_pressurekey[] = "int-pressure";
char int_huniditykey[] = "int-humidity";

calibrationt_t calibrationdata;

/**********************************************************************
 *                           STATIC PROTOTYPE
***********************************************************************/

/**********************************************************************
 *                           STATIC FUNCTIONS
***********************************************************************/
static bool nvsread_value_calibration(char *key, float *value){
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(CALIBRATION_STORGE_NAME, NVS_READWRITE, &my_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    size_t required_size = 0; 
    err = nvs_get_blob(my_handle, key, NULL, &required_size);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) getting size NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    if (required_size == 0)
        return false;

    err = nvs_get_blob(my_handle, key, value, &required_size);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) getting value NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
    
    return true;
}


static bool nvswrite_value_calibration(char *key, float value)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(CALIBRATION_STORGE_NAME, NVS_READWRITE, &my_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(my_handle, (char *)key, &value, sizeof(value));
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
  return true;
}

/**********************************************************************
 *                           GLOBAL FUNCTIONS
***********************************************************************/
void nvsread_calibrationdata(void)
{   
    bool ret = false;

    ret = nvsread_value_calibration(ext_temperaturekey, &calibrationdata.external_temperature_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "External Temperature value read error");
    }
    ret = nvsread_value_calibration(ext_humiditykey, &calibrationdata.external_humidity_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "External Humidity value read error");
    }
    ret = nvsread_value_calibration(ext_humiditykey, &calibrationdata.external_pressure_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "External Pressure value read error");
    }
    ret = nvsread_value_calibration(int_temperaturekey, &calibrationdata.internal_temperature_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "Internal Temperature value read error");
    }
    ret = nvsread_value_calibration(int_pressurekey, &calibrationdata.internal_pressure_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "Internal Pressure value read error");
    }
    ret = nvsread_value_calibration(int_huniditykey, &calibrationdata.internal_humidity_calibration);
    if(ret == false){
        ESP_LOGE(TAG, "Internal Humidity value read error");
    }
}

float getcalibrationvalue_ext_temperature(void){
    return calibrationdata.external_temperature_calibration;
}

float getcalibrationvalue_ext_pressure(void){
    return calibrationdata.external_pressure_calibration;
}

float getcalibrationvalue_ext_humidity(void){
    return calibrationdata.external_humidity_calibration;
}

float getcalibrationvalue_int_temperature(void){
    return calibrationdata.internal_temperature_calibration;    
}

float getcalibrationvalue_int_pressure(void){   
     return calibrationdata.internal_pressure_calibration; 
}

float getcalibrationvalue_int_humidity(void){
     return calibrationdata.internal_humidity_calibration; 
}

void setcalibrationvalue_ext_temperature(float value){
    calibrationdata.external_temperature_calibration = value;
    nvswrite_value_calibration(ext_temperaturekey, calibrationdata.external_temperature_calibration);
}

void setcalibrationvalue_ext_pressure(float value){
    calibrationdata.external_pressure_calibration = value;
    nvswrite_value_calibration(ext_humiditykey, calibrationdata.external_pressure_calibration);
}   

void setcalibrationvalue_ext_humidity(float value){
    calibrationdata.external_humidity_calibration = value;
    nvswrite_value_calibration(ext_humiditykey, calibrationdata.external_humidity_calibration);
}

void setcalibrationvalue_int_temperature(float value){
    calibrationdata.internal_temperature_calibration = value;
    nvswrite_value_calibration(int_temperaturekey, calibrationdata.internal_temperature_calibration);
}

void setcalibrationvalue_int_pressure(float value){
    calibrationdata.internal_pressure_calibration = value;
    nvswrite_value_calibration(int_pressurekey, calibrationdata.internal_pressure_calibration);
}

void setcalibrationvalue_int_humidity(float value){
    calibrationdata.internal_humidity_calibration = value;
    nvswrite_value_calibration(int_huniditykey, calibrationdata.internal_humidity_calibration);
}   