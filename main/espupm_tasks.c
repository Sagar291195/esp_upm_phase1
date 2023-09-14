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
#include "espupm_tasks.h"
#include <inttypes.h>
#include "external/motor.h"
#include <math.h>
#include <counters.h>
#include <sensorManagement.h>
#include <manuCompensationLayer.h>
#include <userCompensationLayer.h>

/*********************
 *      DEFINES
 *********************/

#define INCLUDE_vTaskPrioritySet 1

#define SDA_GPIO 21
#define SCL_GPIO 22

#define TAG_sdp32_task "TAG_sdp32_task"
#define TAG "ESPUPM_TASKS"

/* This is the duration after which the sensor will update the data into the array. */
#define SDP32_SENSOR_READ_DURATION_IN_MS 20
/* this is the number of samples to be averaged in a second */
#define NO_OF_SAMPLES_SDP32 (PID_COMPUT_TIME_AGGRESIVE_IN_MS / SDP32_SENSOR_READ_DURATION_IN_MS)
/* scale factor for the sdp32 diff sensor */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR 240.0

/****************************variables*****************************/
/* array which stores the number of samples in a second for the sdp32 sensor */
float noOfSamplesSdp32[NO_OF_SAMPLES_SDP32] = {0};
/* contains the index of the array which is being updated in the sdp32 sensor value array */
uint16_t last_update_sensor_value_index = 0;
float sdp32Temprature = 0;
unsigned short day_counter;
bool run_once = true;
int dutyCycleVal = 0;
float _airDensityIn = 0;
float _RealFloW;
float *pAirDensityIn = NULL;
float *pRealFlowRate = NULL;
// extern SemaphoreHandle_t main_mutex;
extern SemaphoreHandle_t xGuiSemaphore;

//======PID Variables============
// float kp = 1/*2.5*/, ki = 0/*1.0*/, kd = 0/*1.0*/;
// float kp = 1, ki = 0.8, kd = 0.4;
// pidc_t *pid;
// struct pid_controller ctrldata;
float count_flow = 1, output = 0;
float setpoint_val;
float *flowrateptr;
uint16_t rpm = 0;
int abh = 0;
int chk = 0;
float totalhourVal1;
int16_t dp_ticks = 0;
int16_t dp_scale;
int16_t temperature_ticks;
float t_scale = 200.0;
bool enRead = true;
float flow_comapre = 0.5;
float literTm_Frac = 60.0;
float hundread = 100.0;
int FlowPolyDeg;
float Avg_flow_value1;

//================================================
lv_task_t *infoWgtUpdtWaitToProgTask; // Task to be called after wait time is over
sensor_data_t sensor_data;
float hours_value_sum, flow_value_sum = 0, flow_begin = 0, flow_stop = 0, flow_total_vol = 0, hours_begin = 0, hours_stop = 0, hour_duration;
float diff_press;
float flow_value;
// float dp_ticks_rw = 240.0
volatile int JTCCountSec = 0;
int64_t JTCC21 = 0;
int revCountDefJobStart;
bool ismotor;
bool timerOn = false;
int flow_read_count = 0;
float Avg_flow_value = 0;
float TotalLit;
int TotalliterInt;
int TotalLiterFloat;
volatile int Job_Rem_Hr;
volatile int Job_Rem_Min;
int literMeasureNum = 0; // This is 0 at start when there is Flow Rate 1, Flow Rate Stopped then 2
char today_Date_Msg[200];
char month_name[5] = "";
double total_liters;
double StopLTRCountVal;
double targetLiters;
double effectiveLiters;
float variationLiters;
int HourCount = 0;
float totalhourVal;
bool buzzer_on = false;
float PolyFlowOut;
uint8_t cr, cg, cb;
int workProgressCount = 0;
int dashboardflg;
int global_DashbordBTNflag;
char startDateEnd[55];
char startTimeEnd[60];
double StartLTRCountVal;
double total_liters1;

double *totalliterPtr;
float *totalhourPtr;

uint32_t *INA_measNumPtr = NULL;

float *C1BusVoltPtr = NULL;
float *C1ShuntVoltPtr = NULL;
float *C1ShuntCurrentPtr = NULL;
float *C2BusVoltPtr = NULL;
float *C2ShuntVoltPtr = NULL;
float *C2ShuntCurrentPtr = NULL;
float *C3BusVoltPtr = NULL;
float *C3ShuntVoltPtr = NULL;
float *C3ShuntCurrentPtr = NULL;
int *tickCoutptr = NULL;
bool printHeader = true;
bool printdata = true;

char Rec_Date[60];
char Rec_time[60];
int16_t Rec_dp_ticks;
float Rec_Out;
int16_t Rec_temperature_ticks;
float Rec_flow_value;
float Rec_Polyflow_value;
float Rec_pressureBME;
float Rec_tempBME;
float Rec_humidityBME;
float Rec_pressureBME_Int;
float Rec_tempBME_Int;
float Rec_humidityBME_Int;

float recC1BusVoltPtr;
float recC1ShuntVoltPtr;
float recC1ShuntCurrentPtr;

float recC2BusVoltPtr;
float recC2ShuntVoltPtr;
float recC2ShuntCurrentPtr;

float recC3BusVoltPtr;
float recC3ShuntVoltPtr;
float recC3ShuntCurrentPtr;

int day_Roller_int;
int month_Roller_int;
int year_Roller_int;

int hour_Roller_int;
int min_Roller_int;
int sec_Roller_int;

const int DD = 1;
const int MM = 3;
const int YYYY = 5;

const int HH = 0;
const int min = 9;
const int sec = 0;

/**********************
 *      TYPEDEFS
 **********************/
struct tm navier_time_set =
    {
        .tm_year = YYYY - 1900, // since 1900 (2020 - 1900)
        .tm_mon = MM,           // 0-based
        .tm_mday = DD,
        .tm_hour = HH,
        .tm_min = min,
        .tm_sec = sec};

struct tm navier_time =
    {
        .tm_year = 121, // since 1900 (2020 - 1900)
        .tm_mon = 5,    // 0-based
        .tm_mday = 9,
        .tm_hour = 18,
        .tm_min = 39,
        .tm_sec = 10};

struct tm alarmtest_time =
    {
        .tm_year = 121 + 1990, // since 1900 (2020 - 1900)
        .tm_mon = 8 + 1,       // 0-based
        .tm_mday = 17,
        .tm_hour = 14,
        .tm_min = 52,
        .tm_sec = 10};

sensor_present_t sensor =
    {
        .bme280 = false,
        .bme680 = false,
        .ds3231 = false,
        .ina3231 = false,
        .mpu6050 = false,
        .sdp32 = false,
        .buzzer = true};

int day, month, year;
int totalSecond;

bool navier_set_time;

char guiTime[25];

char guiDate[40];
char guiHrDef[25];
char guiMinDef[32];
char guiS;

char guiDateNext1[200];
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
char guiSeqDate10[25];
char guiSeqDate11[25];
char guiSeqDate12[25];
char guiSeqDate13[25];
char guiSeqDate14[25];

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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ds3231_task(void *pvParameters)
{
    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            if (navier_set_time)
            {
                struct tm navier_time_set = {
                    .tm_year = year_Roller_int - 1900, // since 1900 (2020 - 1900)
                    .tm_mon = month_Roller_int - 1,    // 0-based
                    .tm_mday = day_Roller_int,
                    .tm_hour = hour_Roller_int,
                    .tm_min = min_Roller_int,
                    .tm_sec = sec_Roller_int};

                printf("Time Setting Execution started \n");
                if (ds3231_set_time(&dev, &navier_time_set) != ESP_OK)
                {
                    printf("Could not net time \n");
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
            xSemaphoreGive(xGuiSemaphore);

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

            char now[5] = "Now";
            sprintf(today_Date_Msg, "Today is %2d %s %04d", navier_time.tm_mday, month_name, navier_time.tm_year + 1900);

            sprintf(guiDateNext1, " %s \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n",
                    now, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 0, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 1, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 2, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 3);

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(0);
            day_counter = 0;
            sprintf(guiSeqDate0, "%04d/%02d/%02d", year, month, day);

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(1);
            day_counter = 0;
            sprintf(guiSeqDate1, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(2);
            day_counter = 0;
            sprintf(guiSeqDate2, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(3);
            day_counter = 0;
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(4);
            day_counter = 0;
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(5);
            day_counter = 0;
            sprintf(guiSeqDate4, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(6);
            day_counter = 0;
            sprintf(guiSeqDate5, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(7);
            day_counter = 0;
            sprintf(guiSeqDate6, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(8);
            day_counter = 0;
            sprintf(guiSeqDate7, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(9);
            day_counter = 0;
            sprintf(guiSeqDate8, "%04d/%02d/%02d", year, month, day); // day, month, year

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(10);
            day_counter = 0;
            sprintf(guiSeqDate9, "%04d/%02d/%02d", year, month, day); // day, month, year

            sprintf(GuiDateRollerStr, "%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n", guiSeqDate0, guiSeqDate1, guiSeqDate2, guiSeqDate3, guiSeqDate4, guiSeqDate5, guiSeqDate6, guiSeqDate7, guiSeqDate8, guiSeqDate9);
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

            // xSemaphoreGive(xGuiSemaphore);
        }
    }

    vTaskDelete(NULL);
}

int HighResCounter = 0;
esp_timer_handle_t JTCesp_timer_handle; // JTC = Job Time Counter

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
            // buzzer_on = false;
        }
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}

void ws2812_task(void *pvParamters)
{

    ws2812_init(13);
    cr = 0x5D;
    cg = 0xAF;
    cb = 0x48;

    while (1)
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
        }
    }
}

void iLEDActive(void)
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

void iLEDDeActive(void)
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
    // vTaskDelay(300);
    free(pixels);
}

void infoWgtUpdtWaitToProgTask_cb(lv_task_t *infoWgtUpdtWaitToProgTask)
{
    dashboardflg = 1;

    sprintf(startDateEnd, "%s", guiDate);
    sprintf(startTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
    StartLTRCountVal = total_liters1;

    DashboardInfoWidget();
    lv_label_set_text(xStopButtonLabel, dashboardBTNTxt);
    lv_obj_set_style_local_bg_color(_xStopBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
}

void vinfoWgtUpdtWaitToProgTask(void)
{
    infoWgtUpdtWaitToProgTask = lv_task_create(infoWgtUpdtWaitToProgTask_cb, 1000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(infoWgtUpdtWaitToProgTask);
}

void motorPWMTask(void *pvParameters)
{
    float flowRate = 0;
    float fTempVariable = 0;

    pRealFlowRate = &flowRate; // copying  so that we can print on task
    initiatePWMMotor();        /* intiating the pwm motor */
    initializePIDController(); /* initialize pid controller */

    while (1)
    {
        vTaskDelay(motorWAIT_ON / portTICK_PERIOD_MS);
        /* if motor is runnng then we need to calculate the duty cycle so that to make the constant volume flow */
        while (getIsMotorRunning())
        {
            ESP_LOGD(TAG, "AVERAGE SDP VALUE IN CALUCULATION IS %0.2f", fGetSdp32DiffPressureAverageValue());

            flowRate = fGetVolumetricFlowUserCompensated(); /* calulating the current flow rate */
            if (isnan(flowRate))
            {
                ESP_LOGE(TAG, "flow rate is nan");
                flowRate = MINIMUN_FLOW_RATE;
            }
            else
            {
                fTempVariable = fGetTotalLiterCount(); /* compute the total volume flow in the system */
                ESP_LOGD(TAG, "FLOW rate IS from upm %0.2f", flowRate);
                fTempVariable += ((flowRate * getMotorPIDSampleComputeTime())) / (60 * 1000); /* total liters flow is flowRate in L/Min * time in ms /60*1000 */
                vSetTotalLiterCount(fTempVariable);                                           /* updating the total liters flow in the variable */
            }

            motorPidComputeAndSetOutput(flowRate); /* computing the duty cycle and set it */
            flow_value = flowRate;
            vTaskDelay(pdMS_TO_TICKS(getMotorPIDSampleComputeTime()));
        }
    }
}


/**********************
 *    ERROR ASSERT
 **********************/