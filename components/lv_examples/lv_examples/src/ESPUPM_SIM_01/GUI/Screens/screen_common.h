

/**
 * @file screen_common.h
 *
 */

//   SCREEN_COMMON_H

#ifndef SCREEN_COMMON_H
#define SCREEN_COMMON_H

#ifdef __cplusplus
extern "C"
{
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

extern int startHr;
extern int startMin;

extern int stopHr;
extern int stopMin;

extern int durHr;
extern int durMin;

extern int global_DashbordBTNflag;
extern int global_InfoWidgetStateflag;
extern bool startTimeLabelAdgFlg;


extern char guiTime[25];
extern struct tm navier_time;
extern char rtcTimeNow[25];

extern char guiDate[40];
extern char guiSeqDate1[25];
extern char guiDateDefault[25];
extern char guiDateNext1[200];
extern char GuiDateRollerStr[700];
extern char guiHrDef[25];
extern char guiMinDef[32];

extern char _xaDaterollerbuf[40];
extern char _xaHourMinrollerbuf[130];
extern char _xaHourrollerbuf[60];
extern char _xaMinutrollerbuf[32];

extern char _xsSProllerbuf[32];
extern char _xsDurHourrollerbuf[32];
extern char _xsDurMinutrollerbuf[32];

extern bool defaultParaSelected;

extern char WseqNum1[5];
extern bool rollerMovCkFlag;

extern char stopTime[25];
extern char stopTime1[25];
extern char startTime[100];
extern char stopDate[40];

extern int dashboardflg;
extern int totalSecond;

extern char startDateEnd[55];
extern char stopDateEnd[45];

extern char startTimeEnd[60];
extern char stopTimeEnd[60];

extern int revCountDefJobStart;

extern char * Seq_Number;

extern float SetPoint_Value;

extern float FlowPloyCoffA;
extern float FlowPloyCoffB;
extern float FlowPloyCoffC;
extern float FlowPloyCoffD;
extern float FlowPloyCoffE;
extern float FlowPloyCoffF;
extern float FlowPloyCoffG;
extern float FlowPloyCoffH;
extern float FlowPloyCoffI;
extern float FlowPloyCoffJ;

extern int FlowPolyDeg;

extern bool iArchORSummaryScrn;

   

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void delay(int number_of_seconds);
void getSeqStopDT(int afterHr, int afterMin, int currHr, int currMin);
void getStopDate(int durHr1, int stopHr1);
void DashboardInfoWidget(void);



/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SCREEN_COMMON_H*/
