
/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "espupm_tasks.h"
#include <inttypes.h>
#include <math.h>
#include "storage/parameters.h"
#include <sensorManagement.h>
#include <userCompensationLayer.h>

#include "esp_upm.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG                                 "ESPUPM_TASKS"

#define INCLUDE_vTaskPrioritySet            1
#define SDA_GPIO                            21
#define SCL_GPIO                            22
#define SDP32_SENSOR_READ_DURATION_IN_MS    20         /* This is the duration after which the sensor will update the data into the array. */
#define NO_OF_SAMPLES_SDP32                 (PID_COMPUT_TIME_AGGRESIVE_IN_MS / SDP32_SENSOR_READ_DURATION_IN_MS)    /* this is the number of samples to be averaged in a second */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR    240.0      /* scale factor for the sdp32 diff sensor */

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
sensor_present_t sensor =
{
    .bme280 = false,
    .bme680 = false,
    .ds3231 = false,
    .ina3231 = false,
    .mpu6050 = false,
    .sdp32 = false,
    .buzzer = true};

/********************************************************************************************
 *                            GLOBAL VARIABLES
 ********************************************************************************************/
extern SemaphoreHandle_t i2c_communication_semaphore;
struct tm navier_time;

/********************************************************************************************
 *                            STATIC VARIABLES
 ********************************************************************************************/
float noOfSamplesSdp32[NO_OF_SAMPLES_SDP32] = {0};      /* array which stores the number of samples in a second for the sdp32 sensor */
lv_task_t *infoWgtUpdtWaitToProgTask;                   /*Task to be called after wait time is over */
char today_Date_Msg[200];
char month_name[5] = "";
bool buzzer_on = false;
uint8_t cr, cg, cb;
int dashboardflg;
int global_DashbordBTNflag;
int day, month, year;
bool navier_set_time;
char guiTime[25];
char guiDate[40];
char guiHrDef[25];
char guiMinDef[32];
char GuiDateRollerStr[700];
char guiSeqDate0[25];
char guiSeqDate1[25];
char guiSeqDate2[25];
char guiSeqDate3[25];
char guiSeqDate4[25];
char guiSeqDate5[25];
char guiSeqDate6[25];
char guiSeqDate7[25];
char guiSeqDate8[25];
char guiSeqDate9[25];


/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void iLEDActive(void);
static void iLEDDeActive(void);

/********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/
static void iLEDActive(void)
{
    const uint8_t pixel_count = 1;
    uint8_t green = cg;
    uint8_t red = cr;
    uint8_t blue = cb;
    rgbVal color = makeRGBVal(green, red, blue);
    rgbVal *pixels;
    pixels = malloc(sizeof(rgbVal) * pixel_count);
    pixels[0] = color;
    ws2812_setColors(pixel_count, pixels);
    // vTaskDelay(300);
    free(pixels);
}

static void iLEDDeActive(void)
{
    const uint8_t pixel_count = 1;
    uint8_t green = 0x0;
    uint8_t red = 0x0;
    uint8_t blue = 0x0;
    rgbVal color = makeRGBVal(green, red, blue);
    rgbVal *pixels;
    pixels = malloc(sizeof(rgbVal) * pixel_count);
    pixels[0] = color;
    ws2812_setColors(pixel_count, pixels);
    free(pixels);
}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                          
 ********************************************************************************************/
void ds3231_task(void *pvParameters)
{
    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (pdTRUE == xSemaphoreTake(i2c_communication_semaphore, portMAX_DELAY))
        {
            if (navier_set_time)
            {
                struct tm new_set_time;
                get_new_set_time(&new_set_time);
                printf("Time Setting Execution started \n");
                if (ds3231_set_time(&dev, &new_set_time) != ESP_OK)
                {
                    printf("Could not set time \n");
                }
                printf("Time Setting Execution Done \n");
                navier_set_time = false;
            }

            else if (ds3231_get_time(&dev, &navier_time) != ESP_OK)
            {
                printf("Could not get time1\n");
                ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
                continue;
            }
            xSemaphoreGive(i2c_communication_semaphore);

            int hour1 = navier_time.tm_hour;
            int minute1 = navier_time.tm_min;
            int second1 = navier_time.tm_sec;

            int day1 = navier_time.tm_mday;
            int month1 = navier_time.tm_mon + 1;
            int year1 = navier_time.tm_year + 1900;

            sprintf(guiTime, "%02d:%02d:%02d\n", hour1, minute1, second1);
            sprintf(guiDate, "%04d/%02d/%02d", year1, month1, day1);

            sprintf(guiHrDef, "%d", navier_time.tm_hour);
            sprintf(guiMinDef, "%d", navier_time.tm_min);

            sprintf(today_Date_Msg, "Today is %2d %s %04d", navier_time.tm_mday, month_name, navier_time.tm_year + 1900);

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(0);
            sprintf(guiSeqDate0, "%04d/%02d/%02d", year, month, day);

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(1);
            sprintf(guiSeqDate1, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(2);
            sprintf(guiSeqDate2, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(3);
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(4);
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(5);
            sprintf(guiSeqDate4, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(6);
            sprintf(guiSeqDate5, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(7);
            sprintf(guiSeqDate6, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(8);
            sprintf(guiSeqDate7, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(9);
            sprintf(guiSeqDate8, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(10);
            sprintf(guiSeqDate9, "%04d/%02d/%02d", year, month, day); // day, month, year

            sprintf(GuiDateRollerStr, "%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n\n", guiSeqDate0, guiSeqDate1, guiSeqDate2, guiSeqDate3, guiSeqDate4, guiSeqDate5, guiSeqDate6, guiSeqDate7, guiSeqDate8, guiSeqDate9);
            if (navier_time.tm_mon + 1 == 1)
            {
                strcpy(month_name, "JAN");
            }
            if (navier_time.tm_mon + 1 == 2)
            {
                strcpy(month_name, "FEB");
            }
            if (navier_time.tm_mon + 1 == 3)
            {
                strcpy(month_name, "MAR");
            }
            if (navier_time.tm_mon + 1 == 4)
            {
                strcpy(month_name, "APR");
            }
            if (navier_time.tm_mon + 1 == 5)
            {
                strcpy(month_name, "MAY");
            }
            if (navier_time.tm_mon + 1 == 6)
            {
                strcpy(month_name, "JUNE");
            }
            if (navier_time.tm_mon + 1 == 7)
            {
                strcpy(month_name, "JULY");
            }
            if (navier_time.tm_mon + 1 == 8)
            {
                strcpy(month_name, "AUG");
            }
            if (navier_time.tm_mon + 1 == 9)
            {
                strcpy(month_name, "SEP");
            }
            if (navier_time.tm_mon + 1 == 10)
            {
                strcpy(month_name, "OCT");
            }
            if (navier_time.tm_mon + 1 == 11)
            {
                strcpy(month_name, "NOV");
            }
            if (navier_time.tm_mon + 1 == 12)
            {
                strcpy(month_name, "DEC");
            }
        }
    }

    vTaskDelete(NULL);
}

/********************************************************************************************
 *                          
 ********************************************************************************************/
struct tm get_current_time(void)
{
    return navier_time;
}
 

/********************************************************************************************
 *                          
 ********************************************************************************************/
void buzzer_task(void *pvParamters)
{
    while (1)
    {
        if (sensor.buzzer && buzzer_on)
        {
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 2000));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
            vTaskDelay(300);
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
            vTaskDelay(1000);
        }
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}

/********************************************************************************************
 *                          
 ********************************************************************************************/
bool get_buzzeron_stat(void)
{
    return buzzer_on;
}

/********************************************************************************************
 *                          
 ********************************************************************************************/
void set_buzzeron_stat(bool stat)
{
    buzzer_on = stat;
}

/********************************************************************************************
 *                          
 ********************************************************************************************/
void ws2812_task(void *pvParamters)
{
    if(devicesettings.led_enable == 1)
    {
        ws2812_init(13);
        cr = 0x5D;
        cg = 0xAF;
        cb = 0x48;
    }

    while (1)
    {
        if(devicesettings.led_enable == 1)
        {
            switch (dashboardflg)
            {
            case 0: // Ready
                cr = 0x5D;
                cg = 0xAF;
                cb = 0x48;
                iLEDActive();
                break;

            case 1: // Work in progress
                cr = 0x54;
                cg = 0x83;
                cb = 0xAF;
                iLEDActive();
                vTaskDelay(100);
                iLEDDeActive();
                vTaskDelay(100);
                iLEDActive();
                vTaskDelay(100);
                iLEDDeActive();
                vTaskDelay(2000);
                break;

            case 2: // Work Finished
                cr = 0x38;
                cg = 0x67;
                cb = 0xD6;
                iLEDActive();
                break;

            case 3: // Wait
                cr = 0xD5;
                cg = 0xDE;
                cb = 0x54;
                iLEDActive();
                vTaskDelay(300);
                iLEDDeActive();
                vTaskDelay(300);
                break;

            default:
                break;    
            }
        }
    }
}


/********************************************************************************************
 *                          
 ********************************************************************************************/
void set_navier_time_flag(bool value){
    navier_set_time = value;
}

/**********************
 *    ERROR ASSERT
 **********************/