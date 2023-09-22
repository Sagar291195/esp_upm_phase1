#ifndef SCREEN_COMMON_H
#define SCREEN_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 #include "screen_includes.h"
 
 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
extern int stopHr;
extern int global_DashbordBTNflag;
extern bool startTimeLabelAdgFlg;

extern char guiTime[25];
extern char guiSeqDate1[25];
extern char GuiDateRollerStr[700];
extern char guiHrDef[25];
extern char guiMinDef[32];

extern bool defaultParaSelected;
extern int dashboardflg;
extern int totalSecond;
extern lv_obj_t *crnt_screen;
/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
void getSeqStopDT(int afterHr, int afterMin, int currHr, int currMin);
void getStopDate(int durHr1, int stopHr1);
void DashboardInfoWidget(void);

void set_archiv_or_summary_screen(bool val);
bool get_archiv_or_summary_screen_stat(void);
void set_rollermovck_flag(bool val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_COMMON_H*/
