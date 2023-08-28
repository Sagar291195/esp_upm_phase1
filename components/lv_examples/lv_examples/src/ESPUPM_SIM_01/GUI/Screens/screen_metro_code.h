/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    ABHAI TIWARI
 *  @date      2021-03-05
 */



#ifndef SCREEN_METRO_CODE_H
#define SCREEN_METRO_CODE_H

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

/*******************************
 * EXTERN  Variables
 *******************************/
extern int iUserPass;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void metroCodeScreen(void);
void vClearBTN_metro(void);
void vPasswordLine_metro(void);
void task_cb_metro(lv_task_t *t);
void vCallTask_metro(void);
void _metroOKSymbol(void);
void _metroCrossSymbol(void);
void _metroPasswordStatus(void);




/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_METRO_CODE_H*/

