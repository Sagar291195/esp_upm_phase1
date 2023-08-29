/**
 * @file counters.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief Create the implementation of the various conters involved in the system
 * @version 1.1
 * @date 2022-05-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <counters.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <sampleManagement.h>
#include <sequenceManagement.h>

/******************************************defines****************************************/
#define TAG "counters"

#define NVS_STORGE_NAME "storage"
#define TOTAL_LITER_STORAGE_KEY "Liters"
#define TOTAL_HOUR_STORAGE_KEY "Hours"

/*****************************variables****************************************************/

/* Total number of liters in the system */
float totalLitersCounter = 0;
/* total hours in the system */
float totalHoursCounter = 0;
/* stores the percent of the job done */
float fPercentageOfJobDone = 0;
/* To measure how much time has been passed in the sequence */
uint64_t uTotalSecondPassesInGivenSequence = 0;
/* Tracks how much liters has been passed in the given sequence */
float fTotalLitersHasBeenPassInGivenSequence = 0;

/****************************************function definations***************************************/
float fGetTotalLiterCount(void)
{
  return totalLitersCounter;
}

void vSetTotalLiterCount(float uTotalLiters)
{
  totalLitersCounter = uTotalLiters;
}

float fGetTotalHoursCount()
{
  return totalHoursCounter;
}

void vSetTotalHoursCount(float uTotalHours)
{
  totalHoursCounter = uTotalHours;
}

void vSetTotalLitersValueToNvs()
{

  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    return;
  }

  err = nvs_set_blob(my_handle, TOTAL_LITER_STORAGE_KEY, &totalLitersCounter, sizeof(totalLitersCounter));
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) setting NVS value!", esp_err_to_name(err));
    return;
  }

  // Commit
  err = nvs_commit(my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) committing NVS handle!", esp_err_to_name(err));
  }
  // Close
  nvs_close(my_handle);
}

void vGetTotalLitersFromNvs()
{
  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));

    return;
  }

  // Read run time blob
  size_t required_size = 0; // value will default to 0, if not set yet in NVS
                            // obtain required memory space to store blob being read from NVS

  err = nvs_get_blob(my_handle, TOTAL_LITER_STORAGE_KEY, NULL, &required_size);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) getting blob NVS handle!", esp_err_to_name(err));
  }

  if (required_size == 0)
    return;

  // Read blob
  err = nvs_get_blob(my_handle, TOTAL_LITER_STORAGE_KEY, &totalLitersCounter, &required_size);

  // Close
  nvs_close(my_handle);
}

void vSetTotalHoursValueToNvs()
{

  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
    return;
  }
  /**
   * @brief setting the value to the nvs flash
   *
   */
  err = nvs_set_blob(my_handle, TOTAL_HOUR_STORAGE_KEY, &totalHoursCounter, sizeof(totalHoursCounter));
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) setting NVS value!", esp_err_to_name(err));
    return;
  }

  // Commit
  err = nvs_commit(my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) committing NVS handle!", esp_err_to_name(err));
  }
  // Close
  nvs_close(my_handle);
}

void vGetTotalHoursFromNvs()
{
  nvs_handle_t my_handle;
  esp_err_t err;

  // Open
  err = nvs_open(NVS_STORGE_NAME, NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));

    return;
  }

  size_t required_size = 0; // value will default to 0, if not set yet in NVS
                            // obtain required memory space to store blob being read from NVS
  err = nvs_get_blob(my_handle, TOTAL_HOUR_STORAGE_KEY, NULL, &required_size);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Error (%s) getting blob NVS handle!", esp_err_to_name(err));
  }

  if (required_size == 0)
    return;

  // Read blob
  err = nvs_get_blob(my_handle, TOTAL_HOUR_STORAGE_KEY, &totalHoursCounter, &required_size);

  // Close
  nvs_close(my_handle);
}

uint32_t uGetTotalLiterIntegerPart()
{
  return (uint32_t)totalLitersCounter;
}

uint8_t uGetTotalLiterFloatPart()
{
  return (uint8_t)((totalLitersCounter - (uint32_t)totalLitersCounter) * 100);
}

uint32_t uGetTotalHoursIntegerPart()
{
  return (uint32_t)totalHoursCounter;
}

uint8_t uGetTotalHoursFloatPart()
{
  return (uint8_t)((totalHoursCounter - (uint32_t)totalHoursCounter) * 100);
}

void vGetTheCounterValuesFromNvsFlash()
{
  vGetTotalLitersFromNvs();
  vGetTotalHoursFromNvs();
}

float fGetPercentageOfJobDone()
{
  return fPercentageOfJobDone;
}

void fSetPercentageOfJobDone(float fPercentage)
{
  fPercentageOfJobDone = fPercentage;
}

uint64_t uGetTotalSecondPassesInGivenSequence()
{
  return uTotalSecondPassesInGivenSequence;
}

void vSetTotalSecondPassesInGivenSequence(uint64_t uSecondPasses)
{
  uTotalSecondPassesInGivenSequence = uSecondPasses;
}

void vIncrementTotalSecondPassesInGivenSequence()
{
  uTotalSecondPassesInGivenSequence++;
}

void vSetTotalLitersHasBeenPassInGivenSequence(float fLiters)
{
  fTotalLitersHasBeenPassInGivenSequence = fLiters;
}

float fGetTotalLitersHasBeenPassInGivenSequence()
{
  return fTotalLitersHasBeenPassInGivenSequence;
}
