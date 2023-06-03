

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

#ifndef SCREEN_PIDTUNE_H
#define SCREEN_PIDTUNE_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

//     .tm_year = 121, //since 1900 (2020 - 1900)
//     .tm_mon  = 5,  // 0-based
//     .tm_mday = 9,
//     .tm_hour = 18,
//     .tm_min  = 39,
//     .tm_sec  = 10

#define DAY_ROLLER_DEF     9             
#define MONTH_ROLLER_DEF   5          
#define YEAR_ROLLER_DEF   121         

#define HOUR_ROLLER_DEF    18         
#define MIN_ROLLER_DEF     10          
#define SEC_ROLLER_DEF     10            

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 *  GLOBAL MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

extern int day_Roller_int  ;
extern int month_Roller_int;
extern int year_Roller_int ;

extern int hour_Roller_int;
extern int min_Roller_int ;
extern int sec_Roller_int ;

extern const int DD  ;
extern const int MM;
extern const int YYYY ;

extern const int HH;
extern const int min ;
extern const int sec ;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/
void xScreenPidTune(void);


/**********************
 *    ERROR ASSERT
 **********************/
 
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_PIDTUNE_H*/


