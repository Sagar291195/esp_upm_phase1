/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-22
 */

/**
*  @file 
*  @brief 
*  @details 
*/


/*********************
 *      INCLUDES
 *********************/

#include "middleware.h"

/*********************
 *      DEFINES
 *********************/

#define BUZZER 15

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

double total_liters1;
double *totalliterPtr;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
extern void readTotalLiters(void)
{
    totalliterPtr = &total_liters1;
    TotalliterInt = (int)total_liters1;
    TotalLiterFloat = (total_liters1 - (float)TotalliterInt)*100;
}

void Init_Buzzer(void)
{
    gpio_pad_select_gpio(BUZZER);
    gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER, 0);

    buzzer_init();
    
}


void buzzer_init()
{
    ledc_channel_config_t ledc_channel_left = {0};
    ledc_channel_left.gpio_num = BUZZER;
    ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel_left.channel = LEDC_CHANNEL_1;
    ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_left.timer_sel = LEDC_TIMER_1;
    ledc_channel_left.duty = 0;

    ledc_timer_config_t ledc_timer = {0};
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.timer_num = LEDC_TIMER_1;
    ledc_timer.freq_hz = 1000;

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_left));
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}



int date_difference(int day1, int mon1, int year1, int day2, int mon2, int year2)
{
    // int day1, mon1, year1,
    //     day2, mon2, year2;

    int day_diff, mon_diff, year_diff;

    // int lpy =0;
    // int mnt =0;         

    // printf("Enter start date (MM/DD/YYYY): ");
    // scanf("%d/%d/%d", &mon1, &day1, &year1);

    // printf("Enter end date (MM/DD/YYYY): ");
    // scanf("%d/%d/%d", &mon2, &day2, &year2);

    if(!valid_date(day1, mon1, year1))
    {
        printf("First date is invalid.\n");        
    }

    if(!valid_date(day2, mon2, year2))
    {
        printf("Second date is invalid.\n");
        exit(0);
    }       

    if(day2 < day1)
    {      
        // borrow days from february
        if (mon2 == 3)
        {
            //  check whether year is a leap year
            if ((year2 % 4 == 0 && year2 % 100 != 0) || (year2 % 400 == 0)) 
            {
                day2 += 29;
                // lpy = 1;
            }

            else
            {
                day2 += 28;
                // lpy = 2;
            }                        
        }

        // borrow days from April or June or September or November
        else if (mon2 == 5 || mon2 == 7 || mon2 == 10 || mon2 == 12) 
        {
           day2 += 30; 
        }

        // borrow days from Jan or Mar or May or July or Aug or Oct or Dec
        else
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

    int daysfromnow = year_diff*360 + mon_diff * 30 + day_diff;

    printf("days from now = %d \n", daysfromnow);

    return daysfromnow; // return 0 to operating system
}

// function to check whether a date is valid or not

int valid_date(int day, int mon, int year)    
{
    int is_valid = 1, is_leap = 0;

    if (year >= 1800 && year <= 9999) 
    {

        //  check whether year is a leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        {
            is_leap = 1;
        }

        // check whether mon is between 1 and 12
        if(mon >= 1 && mon <= 12)
        {
            // check for days in feb
            if (mon == 2)
            {
                if (is_leap && day == 29) 
                {
                    is_valid = 1;
                }
                else if(day > 28) 
                {
                    is_valid = 0;
                }
            }

            // check for days in April, June, September and November
            else if (mon == 4 || mon == 6 || mon == 9 || mon == 11) 
            {
                if (day > 30)
                {
                    is_valid = 0;
                }
            }

            // check for days in rest of the months 
            // i.e Jan, Mar, May, July, Aug, Oct, Dec
            else if(day > 31)
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

    /* Reading first time period */
    // printf("Enter hours, minutes and seconds of start time: ");
    // scanf("%d%d%d", &start.hours,&start.minutes, &start.seconds);

    start.hours = strtHr;
    start.minutes = strtMin;
    start.seconds = strtSec;

    /* Reading second time period */
    // printf("Enter hours, minutes and seconds of stop time: ");
    // scanf("%d%d%d", &stop.hours,&stop.minutes, &stop.seconds);

    stop.hours   = stpHr;
    stop.minutes = stpMin;
    stop.seconds = stpSec;

    if(start.seconds > stop.seconds)
    {
        stop.seconds += 60;
        --stop.minutes;
    }

    if(start.minutes > stop.minutes)
    {
        stop.minutes += 60;
        --stop.hours;
    }

    diff.seconds = stop.seconds - start.seconds;
    diff.minutes = stop.minutes - start.minutes;
    diff.hours   = stop.hours   - start.hours;

    printf("Difference = %d : %d : %d \n", diff.hours, diff.minutes, diff.seconds);

    int secVal = diff.hours * 60 * 60 + diff.minutes * 60 + diff.seconds;
    printf(" Wait Second is : %d \n", secVal);
    return secVal;
}

int getWaitSecond(int sday1, int smon1, int syear1, int sday2, int smon2, int syear2, int sstrtHr, int sstrtMin, int sstrtSec, int sstpHr, int sstpMin, int sstpSec){

    int waitdays1 = date_difference(sday1, smon1, syear1, sday2,  smon2,  syear2);
    int waitSec1 = time_difference(sstrtHr, sstrtMin, sstrtSec, sstpHr, sstpMin, sstpSec);

    int totalWaitSec = waitdays1 * 86400 + waitSec1;

    printf("Total Wait Sec = %d \n", totalWaitSec);

    return 0;
}


int RemoveSlashes(char inputStr[100]) // Remove "/" from date & time buffer
{
  
    int i;
    int j = 0;

    for (i = 0; inputStr[i]; i++)
    {
        //5
        if (inputStr[i] >= '0' && inputStr[i] <= '9')
        {
            inputStr[j] = inputStr[i];
            j++;
        }
    }

    inputStr[j] = '\0';
    printf("String after modification : ");
    printf("%s\n", inputStr);
    int comninedDate = atoi(inputStr);
    return comninedDate;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**********************
 *    ERROR ASSERT
 **********************/
