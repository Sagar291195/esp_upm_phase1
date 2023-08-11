/**
 * @file screen_common.c
 * this file is containing all the common function 
 * which can be used in all other files
 */


/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h" 

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
extern char guiTime[25] ;
extern char guiDate[40] ;
extern char guiDateNext1[200];
extern char GuiDateRollerStr[700];
char stopTime[25];
char stopTime1[25];
char stopDate[40];

int totalSecond;


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
    // Storing start time 
    clock_t start_time = clock(); 
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) ;
         
}


// Calculate Date & Time after x hrs & y Min.
void getSeqStopDT(int afterHr, int afterMin, int currHr, int currMin)
{
    int getHr=0;        //
    int getMin=0;       //
    int totalHr=0;      //
    int totalMin=0;     //

    totalHr = currHr + afterHr;
    totalMin  = currMin + afterMin;

    if(totalHr >= 24)
    {
        getHr = totalHr - 24;
    } 
    else 
    {
        getHr = totalHr;
    }

    if(totalMin >= 60)
    {
        getMin = totalMin - 60;
        getHr++;
    }
    else
    {
        getMin = totalMin;
    }
    stopHr = getHr;
    stopMin = getMin;
    //printf("Time after X Hr &  Y Min: %02d:%02d \n", getHr, getMin);
    //stopTime
    char min2[] = "00";
    sprintf(stopTime, "%02dH%02dM\n", getHr, getMin);//stopTime1
    sprintf(stopTime1, "%02d:%02d:%s", getHr, getMin, min2);//stopTime1

}

void getStopDate(int xdurHr, int xstopHr)
{
    int a = 24 - xdurHr;
    if(a == 0)
    {
        sprintf(stopDate, "%s", guiSeqDate1);
    }
    else if(a == xstopHr)
    {
        sprintf(stopDate, "%s", guiSeqDate1);
    }
    else if(a > xstopHr)
    {
        sprintf(stopDate, "%s", guiDate);
    }
    else if(a < xstopHr)
    {
        sprintf(stopDate, "%s", guiSeqDate1);
    }
    else
    {
        sprintf(stopDate, "%s", guiSeqDate1);
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
