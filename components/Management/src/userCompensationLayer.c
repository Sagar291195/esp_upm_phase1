/**
 * @file userCompensationLayer.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This contain the user compensation layer functions definations. All the data realted to the user compensation is caluelated and done here
 * @version 1.1
 * @date 2022-06-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <userCompensationLayer.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

/**********************************************defines******************************/

#define TAG "user compensation"

/*************************************function prototypes******************************/

/****************************************function definations******************************/

float fGetVolumetricFlowUserCompensated()
{
    float result = fGetVolumeFlowFromManuCompensationLayer();

    /**
     * @brief apply the user compensation
     *
     */
    ESP_LOGD(TAG,"user compensated voulmetric flow is %0.2f",result);
    return result;
}

float fGetExternalAirDensityUserCompensated()
{
    float result = fGetAirDensityManuCompensationLayer();

    /**
     * @brief add some user compensation
     *
     */
    ESP_LOGD(TAG,"user compensated extenal air density is %0.2f",result);
    return result;
}

float fGetInternalAirDensityUserCompensated()
{
    float result = fGetInternalAirDensityManuCompensationLayer();

    /**
     * @brief add some user compensation
     *
     */
    ESP_LOGD(TAG,"Internal air density user compensated is %0.2f",result);
    return result;
}

float fGetInternalPressureUserCompensated()
{
    float result = fGetInternalPressureManuCompensationLayer();

    /**
     * @brief add some user compensation
     *
     */
    ESP_LOGD(TAG,"Internal pressure USER COMPENSATED is %0.2f",result);
    return result;
}

float fGetInternalHumidityUserCompesated()
{
    float result = fGetInternalHumidityManuCompensationLayer();

    /**
     * @brief add some user compensation
     *
     */
    ESP_LOGD(TAG,"Internal humidity USER COMPENSATED is %0.2f",result);
    return result;
}

float fGetInternalTemperatureUserCompesated()
{
    float result = fGetInternalTempretureManuCompensationLayer();

    /**
     * @brief add some user compensation
     *
     */
    ESP_LOGD(TAG,"Internal temperature user compensated is %0.2f",result);
    return result;
}

void vGetExternalSensorDataUserCompensated(external_sensor_data_t *xUserCompenstedValues)
{
    external_sensor_data_t xManuCompenstedValues;

    vGetMaufCompensatedExternalSensorData(&xManuCompenstedValues);

    /**
     * @brief do something to get the user compnsated values
     *
     */

    memcpy(xUserCompenstedValues,&xManuCompenstedValues,sizeof(external_sensor_data_t));


    ESP_LOGD(TAG,"User compensated external sensor values are temp %0.2f humidiy %0.2f pressure %0.2f",xUserCompenstedValues->fTemperature,xUserCompenstedValues->fHumidity,xUserCompenstedValues->fPressure);

}