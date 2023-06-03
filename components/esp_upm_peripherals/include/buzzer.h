

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Name (mail or git)
 *  @date      Year
 */

/**
*  @file file
*  @brief Short description
*  @details Long description
*/

#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "esp_upm_periferal.h"
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

void readyModeBuzzBeep(void);
void workInProgressBuzzBeep(void);
void jobFinishedModeBuzzBeep(void);
void waitModeBuzzBeep(void);
/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*BUZZER_H*/


