

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-04-01
 */

/**
*  @file screen_info.h
*  @brief This is header file for screen_info.c
*  @details This is header file for screen_info.c
*/

#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

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
void CallScreenInfo(void);
void infoTimeLabel_refr_func(lv_task_t inforefresherTask);
//void infoTimeLabel_refr_func(void* p);

/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_INFO_H*/


