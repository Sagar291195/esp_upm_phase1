
#ifndef SCREEN_PIDTUNE_H
#define SCREEN_PIDTUNE_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define DAY_ROLLER_DEF      9
#define MONTH_ROLLER_DEF    5
#define YEAR_ROLLER_DEF     121

#define HOUR_ROLLER_DEF     18
#define MIN_ROLLER_DEF      10
#define SEC_ROLLER_DEF      10

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern int day_Roller_int;
extern int month_Roller_int;
extern int year_Roller_int;

extern int hour_Roller_int;
extern int min_Roller_int;
extern int sec_Roller_int;

extern const int DD;
extern const int MM;
extern const int YYYY;

extern const int HH;
extern const int min;
extern const int sec;
/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
void xScreenPidTune(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_PIDTUNE_H*/
