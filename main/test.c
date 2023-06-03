/**
 * @file test.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This is the implementation for the test code for the project. This is unity testing. we can safely delete the file after testing.
 * @version 1.1
 * @date 2022-06-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <test.h>
#include <dataMangement.h>
#include <sensorManagement.h>
#include <sequenceManagement.h>
#include <string.h>

#define TAG "TEST"


void vTestStart()
{
    vInitializeDataManagementApi();

    sequenceSummary_t sequenceSummary;
    memset(&sequenceSummary, 0, sizeof(sequenceSummary_t));
    // vInsertSequenceSummaryIntoDataBase(1, 0,sequenceSummary);

    vGetSequenceSummaryFromDataBase(2, 1, &sequenceSummary);
    ESP_LOGI(TAG,"Summay verified data is %f",sequenceSummary.summary.xVolumeCounter.fEndVolume);

    ESP_LOGW("tests","starting the test code");
    // vInitiateSensorsOnBoard();
}