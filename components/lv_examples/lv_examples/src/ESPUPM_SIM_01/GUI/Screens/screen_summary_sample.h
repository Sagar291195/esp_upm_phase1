/**
*  @file   : screen_summary_sample.h
*  @brief  : This screen is to show the summary of entered preset values 
*  @details: This file is to show summary screen, this screen comes when Valid button is clicked from Preset  screen
*      
*/


#ifndef SCREEN_SUMMARY_SAMPLE_H
#define SCREEN_SUMMARY_SAMPLE_H
#include <stdint.h>
#include <controller.h>

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 *  GLOBAL MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/
void sssSummarySampleScreen(void);
void __sssTimeLabel_refr_func(lv_task_t *__sssrefresherTask);

/**
 * @brief Use this function before calling the sequence summary screen so that the sequence summary screen can be shown for the given sample number and sequnce nubmer
 * 
 * @param u8SampleNumber sample number to be shown in the sequence summary screen
 * @param u8SequenceNumber sequnce number to get the data from the database
 * @param xSequence 
 */

void vSetSampleNumberAndSequnceNumberSampleSummary(uint32_t u8SampleNumber, uint8_t u8SequenceNumber, sequenceSummary_t *xSequnce);


/**********************
 *    ERROR ASSERT
 **********************/



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_SUMMARY_SAMPLE_H*/