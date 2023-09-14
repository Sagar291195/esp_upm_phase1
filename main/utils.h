
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include "esp_err.h"


typedef struct
{
    int hours;
    int minutes;
    int seconds;
}timedif;

int date_difference(int day1, int mon1, int year1, int day2, int mon2, int year2);
int time_difference(int strtHr, int strtMin, int strtSec, int stpHr, int stpMin, int stpSec);
int valid_date(int day, int mon, int year);


#endif  /*_UTILS_H_*/
