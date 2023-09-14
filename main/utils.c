#include <stdbool.h>
#include "esp_log.h"
#include "utils.h"
#include "esp_upm.h"

int date_difference(int day1, int mon1, int year1, int day2, int mon2, int year2)
{
    int day_diff, mon_diff, year_diff;

    if (!valid_date(day1, mon1, year1))
    {
        printf("First date is invalid.\n");
    }

    if (!valid_date(day2, mon2, year2))
    {
        printf("Second date is invalid.\n");
        exit(0);
    }

    if (day2 < day1)
    {
        // borrow days from february
        if (mon2 == 3)
        {
            //  check whether year is a leap year
            if ((year2 % 4 == 0 && year2 % 100 != 0) || (year2 % 400 == 0))
            {
                day2 += 29;
            }
            else
            {
                day2 += 28;
            }
        }
        else if (mon2 == 5 || mon2 == 7 || mon2 == 10 || mon2 == 12)   // borrow days from April or June or September or November
        {
            day2 += 30;
        }
        else    // borrow days from Jan or Mar or May or July or Aug or Oct or Dec
        {
            
            day2 += 31;
        }
        mon2 = mon2 - 1;
    }

    if (mon2 < mon1)
    {
        mon2 += 12;
        year2 -= 1;
    }
    day_diff = day2 - day1;
    mon_diff = mon2 - mon1;
    year_diff = year2 - year1;
    printf("Difference: %d years %02d months and %02d days. \n", year_diff, mon_diff, day_diff);
    int daysfromnow = year_diff * 360 + mon_diff * 30 + day_diff;
    printf("days from now = %d \n", daysfromnow);
    return daysfromnow; // return 0 to operating system
}

// function to check whether a date is valid or not
int valid_date(int day, int mon, int year)
{
    int is_valid = 1, is_leap = 0;

    if (year >= 1800 && year <= 9999)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))  //  check whether year is a leap year
        {
            is_leap = 1;
        }

        if (mon >= 1 && mon <= 12)  // check whether mon is between 1 and 12
        {
            // check for days in feb
            if (mon == 2)
            {
                if (is_leap && day == 29)
                {
                    is_valid = 1;
                }
                else if (day > 28)
                {
                    is_valid = 0;
                }
            }
            else if (mon == 4 || mon == 6 || mon == 9 || mon == 11)   // check for days in April, June, September and November
            {
                if (day > 30)
                {
                    is_valid = 0;
                }
            }
            else if (day > 31)  // check for days in rest of the months i.e Jan, Mar, May, July, Aug, Oct, Dec
            {
                is_valid = 0;
            }
        }
        else
        {
            is_valid = 0;
        }
    }
    else
    {
        is_valid = 0;
    }
    return is_valid;
}

int time_difference(int strtHr, int strtMin, int strtSec, int stpHr, int stpMin, int stpSec)
{
    /* Creating structure variables */
    timedif start, stop, diff;
    start.hours = strtHr;
    start.minutes = strtMin;
    start.seconds = strtSec;

    stop.hours = stpHr;
    stop.minutes = stpMin;
    stop.seconds = stpSec;

    if (start.seconds > stop.seconds)
    {
        stop.seconds += 60;
        --stop.minutes;
    }

    if (start.minutes > stop.minutes)
    {
        stop.minutes += 60;
        --stop.hours;
    }

    diff.seconds = stop.seconds - start.seconds;
    diff.minutes = stop.minutes - start.minutes;
    diff.hours = stop.hours - start.hours;

    printf("Difference = %d : %d : %d \n", diff.hours, diff.minutes, diff.seconds);

    int secVal = diff.hours * 60 * 60 + diff.minutes * 60 + diff.seconds;
    printf(" Wait Second is : %d \n", secVal);
    return secVal;
}
