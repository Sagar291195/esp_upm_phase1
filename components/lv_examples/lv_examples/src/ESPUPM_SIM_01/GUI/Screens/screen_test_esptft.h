
/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Name (mail or git)
 *  @date      Year
 */


/**
*  @file 
*  @brief Short description
*  @details Long description
*/

#ifndef SCREEN_TEST_ESPTFT_H
#define SCREEN_TEST_ESPTFT_H

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
lv_task_t *task1; 
lv_task_t *task2; 

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/
lv_obj_t *  vTestScreen1(void);
lv_obj_t *  vTestScreen2(void);

void cScrTransitCheck(void);
void vCallTask1(void); 
void task_cb1(lv_task_t *t);

/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_TEST_ESPTFT_H*/
