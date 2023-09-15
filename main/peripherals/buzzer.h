#ifndef BUZZER_H
#define BUZZER_H

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
void Init_Buzzer(void);
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


