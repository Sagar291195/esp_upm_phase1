
#ifndef __FLASH_H__
#define __FLASH_H__

#include <stdint.h>
#include <stdbool.h>

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>

/***************************************************************************************************
 *                              TYPEDEF
 ***************************************************************************************************/


/***************************************************************************************************
 *                         FUNCTION PROTOTYPE
 ***************************************************************************************************/
bool nvsread_value_float(char *storagename, char *key, float *value);
bool nvswrite_value_float(char *storagename, char *key, float value);

#endif  /*__FLASH_H__*/