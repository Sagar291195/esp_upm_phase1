/**
 * @file dataMangement.
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief this file contains the data management functions protottype declaration.
 * @version 1.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __DATAMANGEMENT_H__
#define __DATAMANGEMENT_H__

#include <stdio.h>
#include <esp_log.h>
#include <sequenceManagement.h>
#include <stdbool.h>

/****************************************function prototypes***************************************/

/**
 * @brief initializes the data management module
 *
 */
void vInitializeDataManagementApi();

/**
 * @brief writes the sequence summary to the data base
 *
 */
void vInsertSequenceSummaryIntoDataBase(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t sequenceSummary);

/**
 * @brief reads the sequence summary from the data base
 *
 * @param sampleNumber sample number to beread
 * @param sequenceNumber sequnce number to be read
 * @param sequenceSummary output sequnce summary
 * @return true if the sequence summary is read successfully else false
 */
bool vGetSequenceSummaryFromDataBase(uint32_t sampleNumber, uint32_t sequenceNumber, sequenceSummary_t *sequenceSummary);

#endif // __DATAMANGEMENT_H__