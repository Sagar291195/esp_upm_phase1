

/**
 * @file abhai_test1.h
 *
 */

//   CODE_SCREEN_H

#ifndef CODE_SCREEN_H
#define CODE_SCREEN_H

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
 *      TYPEDEFS
 **********************/






/**********************
 * GLOBAL PROTOTYPES
 **********************/
void vChangeScrTest(void);
void vClearBTN(void);
void vTickMark(void);
void vPasswordLine(void);
void vPassword_scr1(void);
void vCallCodeScreen(void);


void vCallTask(void); 
void task_cb(lv_task_t *t);

void vCallTask_Code_Correct(void) ;
void task_cb_Code_Correct(lv_task_t *t); 





/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CODE_SCREEN_H*/
