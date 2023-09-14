/**********************************************************************
 *                              INCLUDES
 ***********************************************************************/



#include "flash.h"

/***********************************************************************
 *                              DEFINES
 **********************************************************************/
#define TAG                 "FLASH"

/***********************************************************************
 *                              TYPEDEF
 **********************************************************************/

/***********************************************************************
 *                             VARIBALES
 **********************************************************************/

/***********************************************************************
 *                           STATIC PROTOTYP
 **********************************************************************/

/***********************************************************************
 *                           STATIC FUNCTIONS
 **********************************************************************/

/***********************************************************************
 *                           GLOBAL FUNCTIONS
 **********************************************************************/
bool nvsread_value_parameter(char *storagename, char *key, void *value)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }

    size_t required_size = 0;
    err = nvs_get_blob(my_handle, key, NULL, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) getting size NVS handle!", esp_err_to_name(err));
        return false;
    }

    if (required_size == 0)
        return false;

    err = nvs_get_blob(my_handle, key, value, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) getting value NVS handle!", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);

    return true;
}

bool nvswrite_value_float(char *storagename, char *key, float value)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(my_handle, (char *)key, &value, sizeof(value));
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
    return true;
}


bool nvswrite_value_parameters(char *storagename, char *key, void *value, uint16_t length)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(storagename, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_blob(my_handle, (char *)key, value, length);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) setting NVS value!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    nvs_close(my_handle);
    return true;
}