#ifndef __FLASH_H__
#define __FLASH_H__


/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "vfs_fat_internal.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include <esp_spi_flash.h>
#include <esp_flash_spi_init.h>
#include <esp_spiffs.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <esp_littlefs.h>
#include <esp_err.h>

#include "sequenceManagement.h"


/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
typedef enum
{
    NORMAL_MODE,
    DO_FIRMWARE_UPDATE,
    FIRMWARE_UPDATE_ERROR,
}device_operating_mode;

typedef enum{
    NO_ERROR_FW_UDPATE,
    ERROR_SSID_PASSWORD_NOT_AVAILABLE,
    ERROR_FIRMWARE_DOWNLOAD,
    ERROR_WIFI_CONNECT,
    ERROR_MQTT_CONNECT,
    ERROR_MQTT_CONFIG_FETCH,
    ERROR_TIMEOUT,
    ERROR_LATEST_FIRMWARE_ALREADY,
    ERROR_UNKNOWN
}firmware_update_error_t;

typedef struct
{
    uint8_t startbyte;
    uint8_t device_operating_mode;
    uint8_t fw_update_state;
}device_state_t;

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
enum{
    ENGLISH,
    FRENCH,
    ITALIAN
}languages_t;

typedef struct{
    char startbyte;
    char screen_lock_password[5];
    char metrology_lock_password[5];
    char wifi_ssid[32];
    char wifi_password[32];
    uint8_t buzzer_enable;
    uint8_t led_enable;
    uint8_t wifi_enable;
    uint8_t external_fan_enable;
    uint8_t screen_sleepmode_enable;
    uint8_t selected_language;
    uint8_t screen_timeout_value;
    uint8_t luminosity_value;
    uint8_t contrast_value;
    char device_serial_number[16];
    char customer_name[32];
}device_settings_t;

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern device_settings_t devicesettings;

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
bool nvsread_value_parameter(char *storagename, char *key, void *value);
bool nvswrite_value_float(char *storagename, char *key, float value);
bool nvswrite_value_parameters(char *storagename, char *key, void *value, uint16_t length);
bool nvswrite_value_u32(char *storagename, char *key, uint32_t value);
bool nvsread_value_u32(char *storagename, char *key, uint32_t *value);
bool nvswrite_value_u8(char *storagename, char *key, uint8_t value);
bool nvsread_value_u8(char *storagename, char *key, uint8_t *value);


void nvs_storage_initialize(void);

bool nvswrite_device_mode_settings(device_state_t *devicestate);
bool nvsread_device_mode_settings(device_state_t *devicestate);
bool database_get_sequence_summary(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t *sequenceSummary);
void database_save_sequence_summary(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t sequenceSummary);

bool nvsread_device_settings(void);
bool nvswrite_device_settings(device_settings_t *settingsbuffer);
#endif  /*__FLASH_H__*/