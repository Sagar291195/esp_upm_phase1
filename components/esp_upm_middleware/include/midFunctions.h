

/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      20-07-2021
 */

/**
*  @file file
*  @brief Short description
*  @details Long description
*/

#ifndef MIDFUNCTIONS_H
#define MIDFUNCTIONS_H

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

typedef struct
{
    int hours;
    int minutes;
    int seconds;
}timedif;

/**********************
 *  GLOBAL MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

extern void readTotalLiters(void);
void Init_Buzzer(void);
void buzzer_init();



int valid_date(int date, int mon, int year);
//int getDays(int day1, int mon1, int year1, int day2, int mon2, int year2);
int date_difference(int day1, int mon1, int year1, int day2, int mon2, int year2);
int time_difference(int strtHr, int strtMnt, int strtSec, int stpHr, int stpMin, int stpSec);
int getWaitSecond(int sday1, int smon1, int syear1, int sday2, int smon2, int syear2, int sstrtHr, int sstrtMin, int sstrtSec, int sstpHr, int sstpMin, int sstpSec);
int RemoveSlashes(char inputStr[100]);

/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MIDFUNCTIONS_H*/


