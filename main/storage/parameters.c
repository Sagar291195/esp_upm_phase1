/**********************************************************************
 *                              INCLUDES
 ***********************************************************************/

#include "flash.h"
#include "parameters.h"
/***********************************************************************
 *                              DEFINES
 **********************************************************************/
#define TAG                         "PARAMETERS"
#define PARAMETERS_STORGE_NAME      "parameters"

/***********************************************************************
 *                              TYPEDEF
 **********************************************************************/

/***********************************************************************
 *                             VARIBALES
 **********************************************************************/
float totalLitersCounter = 0;   /* Total number of liters in the system */
float totalHoursCounter = 0;    /* total hours in the system */
float fPercentageOfJobDone = 0; /* stores the percent of the job done */
uint64_t uTotalSecondPassesInGivenSequence = 0;     /* To measure how much time has been passed in the sequence */
float fTotalLitersHasBeenPassInGivenSequence = 0;   /* Tracks how much liters has been passed in the given sequence */

static char total_literskey[] = "total-liters";  // key to store data in flash
static char total_hourskey[] = "total-liters";  // key to store data in flash
static char pid_parameterskey[] = "pid-parameters";  // key to store data in flash

/***********************************************************************
 *                           STATIC PROTOTYP
 **********************************************************************/
void vSetTotalLitersValueToNvs();
void vGetTotalLitersFromNvs();
void vSetTotalHoursValueToNvs();
void vGetTotalHoursFromNvs();

/***********************************************************************
 *                           STATIC FUNCTIONS
 **********************************************************************/

/***********************************************************************
 *                           GLOBAL FUNCTIONS
 **********************************************************************/
float fGetTotalLiterCount(void)
{
  return totalLitersCounter;
}

void vSetTotalLiterCount(float uTotalLiters)
{
  totalLitersCounter = uTotalLiters;
}

float fGetTotalHoursCount()
{
  return totalHoursCounter;
}

void vSetTotalHoursCount(float uTotalHours)
{
  totalHoursCounter = uTotalHours;
}

uint32_t uGetTotalLiterIntegerPart()
{
  return (uint32_t)totalLitersCounter;
}

uint8_t uGetTotalLiterFloatPart()
{
  return (uint8_t)((totalLitersCounter - (uint32_t)totalLitersCounter) * 100);
}

uint32_t uGetTotalHoursIntegerPart()
{
  return (uint32_t)totalHoursCounter;
}

uint8_t uGetTotalHoursFloatPart()
{
  return (uint8_t)((totalHoursCounter - (uint32_t)totalHoursCounter) * 100);
}


float fGetPercentageOfJobDone()
{
  return fPercentageOfJobDone;
}

void fSetPercentageOfJobDone(float fPercentage)
{
  fPercentageOfJobDone = fPercentage;
}

uint64_t uGetTotalSecondPassesInGivenSequence()
{
  return uTotalSecondPassesInGivenSequence;
}

void vSetTotalSecondPassesInGivenSequence(uint64_t uSecondPasses)
{
  uTotalSecondPassesInGivenSequence = uSecondPasses;
}

void vIncrementTotalSecondPassesInGivenSequence()
{
  uTotalSecondPassesInGivenSequence++;
}

void vSetTotalLitersHasBeenPassInGivenSequence(float fLiters)
{
  fTotalLitersHasBeenPassInGivenSequence = fLiters;
}

float fGetTotalLitersHasBeenPassInGivenSequence()
{
  return fTotalLitersHasBeenPassInGivenSequence;
}

void vGetTheCounterValuesFromNvsFlash()
{
  vGetTotalLitersFromNvs();
  vGetTotalHoursFromNvs();  
}

void vSetTotalLitersValueToNvs()
{
    (void)nvswrite_value_float(PARAMETERS_STORGE_NAME, total_literskey, totalLitersCounter);
}

void vGetTotalLitersFromNvs()
{
    (void)nvsread_value_parameter(PARAMETERS_STORGE_NAME, total_literskey, &totalLitersCounter);
}

void vSetTotalHoursValueToNvs()
{
    (void)nvswrite_value_float(PARAMETERS_STORGE_NAME, total_hourskey, totalHoursCounter);
}

void vGetTotalHoursFromNvs()
{
    (void)nvsread_value_parameter(PARAMETERS_STORGE_NAME, total_hourskey, &totalHoursCounter);
}

void vSetPIDParametersToNvs(struct_PID_parameters_t *paramaters)
{
    (void)nvswrite_value_parameters(PARAMETERS_STORGE_NAME, pid_parameterskey, paramaters, sizeof(struct_PID_parameters_t));
}

void vGetPIDParametersFromNvs(struct_PID_parameters_t *paramaters)
{
    bool ret = nvsread_value_parameter(PARAMETERS_STORGE_NAME, pid_parameterskey, paramaters);
    if(ret == false){
        paramaters->fKp = motorPID_DEFAULT_KP;
        paramaters->fKi = motorPID_DEFAULT_KI;
        paramaters->fKd = motorPID_DEFAULT_KD;
        paramaters->fAkp = motorPID_DEFAULT_AKP;
        paramaters->fAki = motorPID_DEFAULT_AKI;
        paramaters->fAkd = motorPID_DEFAULT_AKD;
        paramaters->fACoff = 1;
        paramaters->fNcoff = 1;
    }
}