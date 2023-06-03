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
#include <motor.h>
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

/**
 * @brief This is the duration after which the sensor will update the data into the array.
 *
 */
#define SDP32_SENSOR_READ_DURATION_IN_MS 20

/**
 * @brief this is the number of samples to be averaged in a second
 *
 */
#define NO_OF_SAMPLES_SDP32 (PID_COMPUT_TIME_AGGRESIVE_IN_MS / SDP32_SENSOR_READ_DURATION_IN_MS)
/**
 * @brief scale factor for the sdp32 diff sensor
 *
 */
#define SDP32_DIFF_PRESSURE_SCALE_FACTOR 240.0

/****************************variables*****************************/

/**
 * @brief array which stores the number of samples in a second for the sdp32 sensor
 *
 */
float noOfSamplesSdp32[NO_OF_SAMPLES_SDP32] = {0};

/**
 * @brief contains the index of the array which is being updated in the sdp32 sensor value array
 *
 */
uint16_t last_update_sensor_value_index = 0;

float sdp32Temprature = 0;

unsigned short day_counter;

bool run_once = true;

int dutyCycleVal = 0;
bool metroFlowCalStarted = false;
int _fasDutyCycle;
float _airDensityIn = 0;
float _RealFloW;
float *pAirDensityIn = NULL;
float *pRealFlowRate = NULL;

// extern SemaphoreHandle_t main_mutex;
extern SemaphoreHandle_t xGuiSemaphore;

TaskHandle_t receiverHandler_test = NULL;

xQueueHandle queueDate;
xQueueHandle queueTime;
xQueueHandle queueSDP;
xQueueHandle queueBME280;
xQueueHandle queueIntBME280;
xQueueHandle queueINA3221;

// receiverHandler = NULL;

//======PID Variables============

// float kp = 1/*2.5*/, ki = 0/*1.0*/, kd = 0/*1.0*/;
// float kp = 1, ki = 0.8, kd = 0.4;
// pidc_t *pid;
// struct pid_controller ctrldata;
float count_flow = 1, output = 0;
float setpoint_val;

float *flowrateptr;

float percentError;

uint16_t rpm = 0;
int abh = 0;
int chk = 0;
float totalhourVal1;

float flowcoeff = 1.3;

int16_t dp_ticks = 0;
int16_t dp_scale;
int16_t temperature_ticks;
float t_scale = 200.0;
bool enRead = true;

float flow_comapre = 0.5;
float literTm_Frac = 60.0;
float hundread = 100.0;
// float diff_press;
float flowPoint;
int FlowPolyDeg;

float Avg_flow_value1;

//================================================

// lv_task_t *refresherTask;

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

int workProgress = 0;
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

// setup datetime: 2020-05-08 12:57:10
// struct tm navier_time_set =
// {
//     .tm_year = 121, //since 1900 (2020 - 1900)
//     .tm_mon  = 5,  // 0-based
//     .tm_mday = 9,
//     .tm_hour = 18,
//     .tm_min  = 39,
//     .tm_sec  = 10
// };

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

// struct tm navier_time_set =
// {
//     .tm_year = DAY_ROLLER_DEF,
//     .tm_mon  = MONTH_ROLLER_DEF,
//     .tm_mday = (YEAR_ROLLER_DEF - 1900),
//     .tm_hour = HOUR_ROLLER_DEF,
//     .tm_min  = MIN_ROLLER_DEF,
//     .tm_sec  = SEC_ROLLER_DEF
// };

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
char rtcTimeNow[25];
char guiDate[40];
char guiDateDefault[25];
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

static float _fDensityCalculation(float fTemperature, float fHumidity, float fPressure);
static float _fFlowCalculation(float fDeltaPressure, float fDensity);

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

    // ESP_ERROR_CHECK(ds3231_clear_oscillator_stop_flag(&dev));
    // ESP_ERROR_CHECK(ds3231_enable_32khz(&dev));
    // ds3231_set_flag(&dev, DS3231_ADDR_STATUS, DS3231_STAT_OSCILLATOR, DS3231_SET);

    while (1)
    {
        // vTaskDelay(1000);
        vTaskDelay(pdMS_TO_TICKS(1000));

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            if (navier_set_time)
            {

                struct tm navier_time_set =
                    {
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
                // ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
                ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
                continue;
            }
            // else if (ds3231_get_time(&dev, &navier_time) != ESP_OK)
            // {
            //     printf("Could not get time2\n");
            //     //ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     //continue;
            // }
            // else if (ds3231_get_time(&dev, &navier_time) != ESP_OK)
            // {
            //     printf("Could not get time3\n");
            //     //ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     //continue;
            // }
            // else if (ds3231_get_time(&dev, &navier_time) != ESP_OK)
            // {
            //     printf("Could not get time4\n");
            //     //ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     //ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));
            //     continue;
            // }

            xSemaphoreGive(xGuiSemaphore);
            // vTaskDelay(pdMS_TO_TICKS(250));

            /* float is used in printf(). you need non-default configuration in
             * sdkconfig for ESP8266, which is enabled by default for this
             * example. see sdkconfig.defaults.esp8266
             */

            // printf("%04d-%02d-%02d %02d:%02d:%02d\n", navier_time.tm_year + 1900 /*Add 1900 for better readability*/, navier_time.tm_mon + 1,
            //         navier_time.tm_mday, navier_time.tm_hour, navier_time.tm_min, navier_time.tm_sec);

            // if(ds3231_set_alarm(&dev, DS3231_ALARM1_MATCH_SECMINHOURDATE, &alarmtest_time, DS3231_ALARM1_MATCH_SECMINHOURDATE, 0, 0) == ESP_OK)
            // {
            //     printf("Alarm Fired \n");
            // }
            int hour1 = navier_time.tm_hour;
            int minute1 = navier_time.tm_min;
            int second1 = navier_time.tm_sec;

            int day1 = navier_time.tm_mday;
            int month1 = navier_time.tm_mon + 1;
            int year1 = navier_time.tm_year + 1900;

            sprintf(guiTime, "%02d:%02d:%02d\n", hour1, minute1, second1);
            sprintf(rtcTimeNow, "%02d:%02d:%02d", hour1, minute1, second1);
            // printf(guiTime);
            // guiDateDefault
            sprintf(guiDateDefault, "%02dH %02dM\n", hour1, minute1);
            sprintf(guiDate, "%04d/%02d/%02d", year1, month1, day1);
            log_guiDate_t queueGUIDate;
            log_guiTime_t queueGUITime;

            memset(queueGUIDate.QguiDate, 0, 10);
            memcpy(queueGUIDate.QguiDate, guiDate, strlen(guiDate));

            memset(queueGUITime.QguiTime, 0, 10);
            memcpy(queueGUITime.QguiTime, rtcTimeNow, strlen(guiTime));

            if (xQueueSend(queueDate, &queueGUIDate, 100 / portTICK_PERIOD_MS))
            {
            }
            if (xQueueSend(queueTime, &queueGUITime, 100 / portTICK_PERIOD_MS))
            {
            }

            // printf(guiDate);
            // extern char guiHrDef[5];
            // extern char guiMinDef[5];
            sprintf(guiHrDef, "%d", navier_time.tm_hour);
            sprintf(guiMinDef, "%d", navier_time.tm_min);
            char now[5] = "Now";
            sprintf(today_Date_Msg, "Today is %2d %s %04d", navier_time.tm_mday, month_name, navier_time.tm_year + 1900);

            sprintf(guiDateNext1, " %s \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n %04d-%02d-%02d   \n"

                    ,
                    now, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 0, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 1, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 2, navier_time.tm_year + 1900, navier_time.tm_mon + 1, navier_time.tm_mday + 3);
            // printf(guiDateNext1);

            //-------------------------------
            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(0);
            day_counter = 0;
            // sprintf ( guiSeqDate0, "%04d/%02d/%02d" ,navier_time.tm_year + 1900 , navier_time.tm_mon + 1,navier_time.tm_mday);
            sprintf(guiSeqDate0, "%04d/%02d/%02d", year, month, day);
            // printf(guiSeqDate0);

            //-------------------------------
            //-------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(1);
            day_counter = 0;
            sprintf(guiSeqDate1, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate1);
            /* after this call next_day each day */
            // print_date();

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(2);
            day_counter = 0;
            sprintf(guiSeqDate2, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate2);

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(3);
            day_counter = 0;
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate3);

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(4);
            day_counter = 0;
            sprintf(guiSeqDate3, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate3);s

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(5);
            day_counter = 0;
            sprintf(guiSeqDate4, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate4);

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(6);
            day_counter = 0;
            sprintf(guiSeqDate5, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate5);

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(7);
            day_counter = 0;
            sprintf(guiSeqDate6, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate6);

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(8);
            day_counter = 0;
            sprintf(guiSeqDate7, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate7);

            //-------------------------------------------------

            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(9);
            day_counter = 0;
            sprintf(guiSeqDate8, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate8);

            //-------------------------------------------------
            //-------------------------------------------------

            set_date(navier_time.tm_mday, navier_time.tm_mon + 1, navier_time.tm_year + 1900);
            skip_days(10);
            day_counter = 0;
            sprintf(guiSeqDate9, "%04d/%02d/%02d", year, month, day); // day, month, year
            // printf(guiSeqDate8);

            //-------------------------------------------------

            //-------------------------------------------------

            // char now1[5] = "Now";
            //  sprintf ( GuiDateRollerStr,  "%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n"
            //                          ,now1 ,guiSeqDate0, guiSeqDate1, guiSeqDate2, guiSeqDate3, guiSeqDate4, guiSeqDate5
            //                          ,guiSeqDate6, guiSeqDate7,guiSeqDate8, guiSeqDate9 );

            sprintf(GuiDateRollerStr, "%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n%s     \n", guiSeqDate0, guiSeqDate1, guiSeqDate2, guiSeqDate3, guiSeqDate4, guiSeqDate5, guiSeqDate6, guiSeqDate7, guiSeqDate8, guiSeqDate9);

            // printf(GuiDateRollerStr);

            //-------------------------------------------------

            //-------------------------------------------------

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

void bme280_task(void *pvParamters)
{
    float pressure, temperature, humidity;

    bmp280_params_t params;
    bmp280_init_default_params(&params);
    bmp280_t dev;
    memset(&dev, 0, sizeof(bmp280_t));

    vTaskDelay(1000);
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

        ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_1, 0, SDA_GPIO, SCL_GPIO));
        esp_err_t err = bmp280_init(&dev, &params);
        if (err == ESP_OK)
            sensor.bme280 = true;
        xSemaphoreGive(xGuiSemaphore);
    }

    while (1)
    {
        // vTaskDelay(900);

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            if (sensor.bme280)
            {
                if (bmp280_read_float(&dev, &temperature, &pressure, &humidity) != ESP_OK)
                {
                    printf("Temperature/pressure reading failed\n");
                    continue;
                }
                // printf("External BME280 Pressure: %.2f Pa, Temperature: %.2f C ", pressure/100, temperature);
                //  printf(", Humidity: %.2f\n", humidity);
                float p1 = pressure;
                float t1 = temperature + 273.15;
                _airDensityIn = (p1) / (287.05 * t1);
                _airDensityIn = _fDensityCalculation(temperature, humidity, pressure);
                pAirDensityIn = &_airDensityIn;
                // printf("Inside Air Density is = %0.4f\n", *pAirDensityIn);
                // fflush(NULL);
                log_BME280sensor_t Q_BME280;
                Q_BME280.Qpressure_BME = pressure / 100;
                Q_BME280.Qtemp_BME = temperature;
                Q_BME280.Qhumidity_BME = humidity;
                if (xQueueSend(queueBME280, &Q_BME280, 100 / portTICK_PERIOD_MS))
                {
                    // printf("BME 280 added message to log queue  \n");
                }

                sensor_data.bme280_temp = temperature;
                sensor_data.bme280_press = pressure;
                sensor_data.bme280_humid = humidity;
            }

            xSemaphoreGive(xGuiSemaphore);
            vTaskDelay(900);
        }
    }

    vTaskDelete(NULL);
}

void InternalBME280_task(void *pvParamters)
{
    float in_pressure, in_temperature, in_humidity;

    bmp280_params_t in_params;
    bmp280_init_default_params(&in_params);
    bmp280_t dev;
    memset(&dev, 0, sizeof(bmp280_t));

    vTaskDelay(900);
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, SDA_GPIO, SCL_GPIO));
        esp_err_t err = bmp280_init(&dev, &in_params);
        if (err == ESP_OK)
            sensor.bme280 = true;
        xSemaphoreGive(xGuiSemaphore);
    }

    while (1)
    {
        vTaskDelay(900);

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            if (sensor.bme280)
            {
                if (bmp280_read_float(&dev, &in_temperature, &in_pressure, &in_humidity) != ESP_OK)
                {
                    printf("Temperature/pressure reading failed\n");
                    continue;
                }
                // printf("Internal BME280 Pressure: %.2f Pa, Temperature: %.2f C ", in_pressure/100, in_temperature);
                // printf(", Humidity: %.2f\n", in_humidity);
                log_IntBME280sensor_t Q_IntBME280;
                Q_IntBME280.Qpressure_IntBME = in_pressure / 100;
                Q_IntBME280.Qtemp_IntBME = in_temperature;
                Q_IntBME280.Qhumidity_IntBME = in_humidity;
                if (xQueueSend(queueIntBME280, &Q_IntBME280, 100 / portTICK_PERIOD_MS))
                {
                    // printf("BME 280 added message to log queue  \n");
                }

                sensor_data.bme280_temp = in_temperature;
                sensor_data.bme280_press = in_pressure;
                sensor_data.bme280_humid = in_humidity;
            }

            xSemaphoreGive(xGuiSemaphore);
            vTaskDelay(100);
        }
    }

    vTaskDelete(NULL);
}

// void sdp32_task(void *pvParamters)
// {

//     ESP_LOGV(TAG_sdp32_task, "start sdp32_task");

//     // xGuiSemaphore = xSemaphoreCreateMutex();
//     sdp32_t dev;
//     memset(&dev, 0, sizeof(sdp32_t));
//     float out1 = 0.0;
//     float alpha = 0.30; // 0.50
//     // uint8_t cmd[2] = {0x36, 0x2f}; // Read data command in triggered mode
//     // uint8_t cmd[2] = {0x36, 0x1E}; // Start Read in continous Mode
//     uint8_t startContRead_cmd[2] = {0x36, 0x15}; // Start Read in continous Mode
//     uint8_t stopContRead_cmd[2] = {0x3F, 0xF9};  // Stop Countinous measure command
//     uint8_t softReset_cmd[2] = {0x00, 0x06};
//     uint8_t read_buff[9] = {0};
//     uint8_t massFlowRead[2] = {0x36, 0x03};

//     ESP_ERROR_CHECK(sdp32_init_desc(&dev, SDP32_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));
//     vTaskDelay(pdMS_TO_TICKS(100));

//     vTaskDelay(pdMS_TO_TICKS(900)); // 900
//     if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
//         sdp32_send_cmd_read(&dev, stopContRead_cmd);
//     printf("stopContRead Command Send: %x %x\n", stopContRead_cmd[0], stopContRead_cmd[1]);
//     xSemaphoreGive(xGuiSemaphore);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     vTaskDelay(pdMS_TO_TICKS(900)); // 900
//     if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
//         sdp32_send_cmd_read(&dev, softReset_cmd);
//     printf("soft Rest Command Send: %x %x\n", softReset_cmd[0], softReset_cmd[1]);
//     xSemaphoreGive(xGuiSemaphore);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     ESP_LOGI(TAG_sdp32_task, "Starting sensor in massflow mode");
//     if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
//     {

//         sdp32_send_cmd_read(&dev, massFlowRead);
//     }
//     xSemaphoreGive(xGuiSemaphore);
//     vTaskDelay(pdMS_TO_TICKS(30)); // sensor needs time to start

//     while (1)
//     {
//         ESP_LOGV(TAG_sdp32_task, "start loop sdp32_task");

//         vTaskDelay(pdMS_TO_TICKS(400)); // 900

//         if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
//         {
//             sdp32_read_pressure(&dev, read_buff); // actually reading the mass flow
//             // printf("read_buff Command Send: %x %x\n", read_buff[0], read_buff[1]);
//             xSemaphoreGive(xGuiSemaphore);
//             vTaskDelay(pdMS_TO_TICKS(100));
//         }

//         uint8_t checksum = read_buff[2];
//         bool check = CheckCrc(read_buff, 2, checksum);
//         int16_t massFlow = 0;
//         if (check)
//         {
//             massFlow = (read_buff[0] << 8) | read_buff[1];
//             ESP_LOGI(TAG, "massFlow: %d", massFlow);
//             // uint16_t sf = read_buff[6]<<8|read_buff[7];
//             // ESP_LOGI(TAG,"scaing factor: %d", sf);
//         }

//         else
//         {
//             ESP_LOGE(TAG, "CRC check failed, wrong value");
//         }

//         dp_ticks = BIU16(read_buff, 0);
//         // printf("Direct Sensor Avg Value = %d ,", dp_ticks); //======================>
//         // printf("Direct Sensor Avg Value = %d ,", 0);
//         // dp_scale = BIU16(read_buff, 6);
//         float dp_scale1 = 240.0;
//         // dp_ticks = dp_ticks /dp_scale1;
//         diff_press = (float)BIU16(read_buff, 0);
//         diff_press = diff_press / dp_scale1;
//         // sensor_data.sdp32_diff_press = dp_ticks;
//         // sensor_data.sdp32_diff_press = diff_press;
//         temperature_ticks = BIU16(read_buff, 3);
//         temperature_ticks = temperature_ticks / t_scale;
//         // sensor_data.sdp32_temp = temperature_ticks;
//         // printf("Differential preesure ticks =%d\n", dp_ticks);
//         int one = 1;
//         // out = out*(one - alpha) + alpha * (float)dp_ticks;  // filter //
//         float max_dp_tick = 136.5291;
//         if (diff_press <= max_dp_tick)
//         {
//             out1 = diff_press;
//         }
//         else
//         {
//             out1 = 0;
//         } /*else{printf("Presure reading invalid\n");}*/
//         // if(diff_press <= max_dp_tick){out1 =  out1*(1 - alpha) + alpha * diff_press;}else{out1 =0;}
//         // out = dp_ticks;
//         // flow_value = 1.3 * sqrt(out);
//         // out1 = diff_press;

// flow_value = flowcoeff * (float)sqrt(out1);
//         flowrateptr = &flow_value;

//         // _RealFloW = 17.97 * sqrt(out1 / *pAirDensityIn);

//         //_RealFloW = sqrt(fabs(out1)) / sqrt(*pAirDensityIn) * 0.3758940103;
//         _RealFloW = _fFlowCalculation(out1, _airDensityIn);
//         _RealFloW = _RealFloW * 2.7;

//         printf("Real_Flow: , %0.4f\n", _RealFloW);
//         fflush(NULL);

//         //==================
//         //==================
//         //==================

//         // int x = 100;
//         // int i;
//         // float avg = 0, y;
//         // y = _RealFloW;
//         // for(i = 0; i < x; i++)
//         // {
//         //     avg = avg + y;
//         // }
//         // avg = avg / x;
//         // printf("Custom avg Flow is = %0.4f\n", avg*2.7);

//         //====================
//         //====================
//         //====================

//         pRealFlowRate = &_RealFloW;
//         PolyFlowOut = iCalibXdegPoly(FlowPolyDeg, flow_value);
//         // printf("SDP32 Diff Press %0.2f Pa, Temperature %d C, Flow Value = %0.4f, ", out1, temperature_ticks, flow_value);////======================>
//         // printf("Flow Value = %0.2f, ", flow_value);
//         log_SDPsensor_t Q_SDP;
//         Q_SDP.dp_ticks_SDP = dp_ticks;
//         Q_SDP.out_SDP = out1;
//         Q_SDP.temperature_ticks_SDP = temperature_ticks;
//         Q_SDP.flow_value_SDP = flow_value;
//         Q_SDP.polyFlow = PolyFlowOut;
//         if (xQueueSend(queueSDP, &Q_SDP, 100 / portTICK_PERIOD_MS))
//         {
//             // printf("SDP data added message to log queue  \n");
//         }

//         //====================
//         //====================
//         if (isMotor == true)
//         {
//             setpoint_val = SetPoint_Value; // Set point from preset screen
//             // printf("Set point = %f\n", setpoint_val);
//             // pid = pid_create(&ctrldata, &flow_value, &output, &setpoint_val);
//             if (run_once == true)
//             {
//                 output = LOWER_PID_LIMIT + setpoint_val * 1000;
//             }
//             run_once = false;
//             pid = pid_create(&ctrldata, &_RealFloW, &output, &setpoint_val);
//             // ESP_LOGI(TAG,"Value of kp ki kd  is %0.2f  %0.2f %0.2f",kp,kd,ki);
//             pid_tune(pid, kp, ki, kd, false);                   // agressively tune
//             pid_limits(pid, LOWER_PID_LIMIT, HIGHER_PID_LIMIT); // Set the LEDC duty, the range of duty setting is [0, (2**duty_resolution) - 1]
//             pid_auto(pid);
//             pid_compute(pid); // Compute new PID output value
//             rpm = (uint16_t)output;
//             ESP_LOGI(TAG, "PWM = %d Massflow is %d", rpm, massFlow);

//             ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, rpm));
//             ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2));
//         }
//         else
//         {
//             rpm = 0;
//             run_once = true;
//         }
//         // float out_scl = 10.0;
//         // printf("setPoint = %0.2f, ", setpoint_val);////======================>
//         // printf("PWM duty = %d\n", rpm);////======================>
//         // printf("PWM duty = %d\n", rpm/100);

//         //====================
//         //====================

//         /****Commenting this part of code because this part is implemented in flowCounterTimer_Task */

//         // if(flow_value > flow_comapre)
//         // {

//         //     literMeasureNum = 1;
//         //     flow_read_count++;
//         //             // Avg_flow_value  = (Avg_flow_value + flow_value);
//         //             // float AvgFloWVal= Avg_flow_value / (float) flow_read_count;
//         //     printf("Flow Read Count = %d\n", flow_read_count);
//         //     Avg_flow_value  = (Avg_flow_value + flow_value)/ (float) flow_read_count;
//         //     TotalLit        =  AvgFloWVal *( (float)flow_read_count / literTm_Frac); //60
//         //     //TotalLit        =  Avg_flow_value * (float)flow_read_count * 0.01666667* (2.6 * 60); //60
//         //     total_liters    =  TotalLit + total_liters1 ;
//         //     totalliterPtr   =  &total_liters;
//         //     TotalliterInt   =  (int)total_liters;
//         //     TotalLiterFloat =  (total_liters - (float)TotalliterInt)*hundread;

//         // }
//         // else
//         // {

//         //     if(literMeasureNum == 1)
//         //     {
//         //         /****Upload data on NVS */
//         //         write_TotalLiter(total_liters);
//         //         StopLTRCountVal = total_liters;
//         //         targetLiters = (totalSecond/60.0) * atoi(_xsSProllerbuf);
//         //         effectiveLiters = StopLTRCountVal - StartLTRCountVal;
//         //         variationLiters = hundread - ((effectiveLiters*hundread)/targetLiters);
//         //         flow_read_count = 0;
//         //         /*Upload data on NVS Done */
//         //         literMeasureNum = 0; // Reset flag

//         //     }
//         // }
//         ESP_LOGV(TAG_sdp32_task, "end loop sdp32_task");
//     }

//     vTaskDelete(NULL);
// }

void ina3221_task(void *pvParameters)
{
    ina3221_t dev = {
        .shunt = {100, 100, 100}, // shunt values are 100 mOhm for each channel
        .config.config_register = INA3221_DEFAULT_CONFIG,
        .mask.mask_register = INA3221_DEFAULT_MASK};
    memset(&dev.i2c_dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(ina3221_init_desc(&dev, INA3221_I2C_ADDR, 0, SDA_GPIO, SCL_GPIO));
    vTaskDelay(pdMS_TO_TICKS(1000));
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

        ESP_ERROR_CHECK(ina3221_set_options(&dev, MODE, true, true));              // Mode selection, bus and shunt activated
        ESP_ERROR_CHECK(ina3221_enable_channel(&dev, true, true, true));           // Enable all channels
        ESP_ERROR_CHECK(ina3221_set_average(&dev, INA3221_AVG_64));                // 64 samples average
        ESP_ERROR_CHECK(ina3221_set_bus_conversion_time(&dev, INA3221_CT_2116));   // 2ms by channel
        ESP_ERROR_CHECK(ina3221_set_shunt_conversion_time(&dev, INA3221_CT_2116)); // 2ms by channel

        ESP_ERROR_CHECK(ina3221_set_warning_alert(&dev, WARNING_CHANNEL - 1, WARNING_CURRENT)); // Set overcurrent security flag

        xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));

    log_INAsensor_t INASensor1;
    uint32_t measure_number = 0;
    bool warning = false;
    float bus_voltage;
    float shunt_voltage;
    float shunt_current;

    while (1)
    {
        measure_number++;

        vTaskDelay(pdMS_TO_TICKS(900)); // 100

#if !MODE
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            ESP_ERROR_CHECK(ina3221_trigger(&dev)); // Start a measure
            xSemaphoreGive(xGuiSemaphore);
        }
        // printf("trig done, wait: ");
        do
        {
            if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
            {
                ESP_ERROR_CHECK(ina3221_get_status(&dev)); // get mask

                xSemaphoreGive(xGuiSemaphore);
            }

            if (dev.mask.wf & (1 << (3 - WARNING_CHANNEL)))
                warning = true;

            vTaskDelay(200 / portTICK_PERIOD_MS);

        } while (!(dev.mask.cvrf)); // check if measure done
#else
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            ESP_ERROR_CHECK(ina3221_get_status(&dev)); // get mask
            xSemaphoreGive(xGuiSemaphore);
        }

        if (dev.mask.wf & (1 << (3 - WARNING_CHANNEL)))
            warning = true;
#endif
        for (uint8_t i = 0; i < INA3221_BUS_NUMBER; i++)
        {

            if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
            {
                // Get voltage in volts
                ESP_ERROR_CHECK(ina3221_get_bus_voltage(&dev, i, &bus_voltage));
                // Get voltage in millivolts and current in milliamperes
                ESP_ERROR_CHECK(ina3221_get_shunt_value(&dev, i, &shunt_voltage, &shunt_current));
                xSemaphoreGive(xGuiSemaphore);
            }

            // printf("\nC%u:Measure number %u\n", i + 1, measure_number);
            INA_measNumPtr = &measure_number;
            if (warning && (i + 1) == WARNING_CHANNEL)
            {
            }
            // printf("C%u:Warning Current > %.2f mA !!\n", i + 1, WARNING_CURRENT);
            // printf("C%u:Bus voltage: %.02f V\n", i + 1, bus_voltage);
            // printf("C%u:Shunt voltage: %.02f mV\n", i + 1, shunt_voltage);
            // printf("C%u:Shunt current: %.02f mA\n\n", i + 1, shunt_current);

            uint8_t a_ina = 1;
            uint8_t b_ina = 2;
            uint8_t c_ina = 3;

            // xSemaphoreGive(xGuiSemaphore);

            if ((i + 1) == a_ina)
            {
                C1BusVoltPtr = &bus_voltage;
                C1ShuntVoltPtr = &shunt_voltage;
                C1ShuntCurrentPtr = &shunt_current;

                INASensor1.qC1BusVoltPtr = bus_voltage;
                INASensor1.qC1ShuntVoltPtr = shunt_voltage;
                INASensor1.qC1ShuntCurrentPtr = shunt_current;
            }
            if ((i + 1) == b_ina)
            {
                C2BusVoltPtr = &bus_voltage;
                C2ShuntVoltPtr = &shunt_voltage;
                C2ShuntCurrentPtr = &shunt_current;

                INASensor1.qC2BusVoltPtr = bus_voltage;
                INASensor1.qC2ShuntVoltPtr = shunt_voltage;
                INASensor1.qC2ShuntCurrentPtr = shunt_current;
            }
            if ((i + 1) == c_ina)
            {
                C3BusVoltPtr = &bus_voltage;
                C3ShuntVoltPtr = &shunt_voltage;
                C3ShuntCurrentPtr = &shunt_current;

                INASensor1.qC3BusVoltPtr = bus_voltage;
                INASensor1.qC3ShuntVoltPtr = shunt_voltage;
                INASensor1.qC3ShuntCurrentPtr = shunt_current;
            }
        }

        if (xQueueSend(queueINA3221, &INASensor1, 100 / portTICK_PERIOD_MS))
        {
            // printf("SDP data added message to log queue  \n");
        }
        warning = false;

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

int HighResCounter = 0;
esp_timer_handle_t JTCesp_timer_handle; // JTC = Job Time Counter

void JTCtimer_callback(void *args)
{

    if (isMotor)
    {
        HighResCounter++;
        // printf("Tick Counter High resolution Counter = %d\n", HighResCounter);
        // printf("FlowRate is: %0.2f\n", flow_value);
        totalhourVal1 = totalhourVal + (float)HighResCounter / 3600.0;
        StopHrEND = totalhourVal1;
        totalhourPtr = &totalhourVal1;

        if (HighResCounter > totalSecond)
        {
            isMotor = !isMotor;
            //============
            //============
            if (1)
            {
                printf("save hour counter \n");
                setSensorTasksTOzeroPriority();
                StopHrEND = totalhourVal1;
                // printf("Current Hour counter value : %f \n", totalhourVal);
                writeTotalhour(totalhourVal1); //
                targetHr = (float)totalSecond / 3600.0;
                effectiveHr = ((float)HighResCounter - 1.0) / 3600.0;
                variationHr = 100.0 - ((effectiveHr * 100.0) / targetHr);
                totalHourInt = (int)totalhourVal1;
                totalHourFloat = (totalhourVal1 - (float)totalHourInt) * 100;
                // printf("PumpStopForcefully: %d \n", PumpStopForcefully);
                if (PumpStopForcefully != true)
                {
                    // printf("Change InfoWidget \n");
                    lv_task_del(ResInfoPerChange_task);
                    sprintf(stopDateEnd, "%s", guiDate);
                    sprintf(stopTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
                    // setSensorTasksTOzeroPriority();
                    vUpdateInfoWidgetTask();
                }

                if (!isMotor)
                {
                    setSensorTasksTOzeroPriority();
                }

                PumpStopForcefully = false;
            }

            //=============
            //=============

            HighResCounter = 0;
        }
    }
}

int w = 0;
volatile int df;

void JobTimeCounter_task(void *pvParameters)
{

    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = JTCtimer_callback,
        .name = "Job Time Counter"};
    esp_timer_handle_t JTCesp_timer_handle; // JTC = Job Time Counter
    esp_timer_create(&esp_timer_create_args, &JTCesp_timer_handle);
    esp_timer_start_periodic(JTCesp_timer_handle, 1000000);
    printf("Timer Start time: %s \n", guiTime);
    int notificationValue;

    while (true)
    {
        // vTaskDelay(pdMS_TO_TICKS(1000));
        notificationValue = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        // printf("Notification received %d\n", notificationValue);
        // HourCount ++;
        if (isMotor == false)
        {
            // printf("Job completer pump off \n");
            //  esp_timer_stop(JTCesp_timer_handle);
            //  esp_timer_delete(JTCesp_timer_handle);
        }
    }

    vTaskDelete(NULL);
}

void flowCounttimer_callback(void *args)
{

    if (flow_value > flow_comapre)
    {
        literMeasureNum = 1;
        flow_read_count++;
        // Avg_flow_value  = (Avg_flow_value + flow_value);
        Avg_flow_value = (Avg_flow_value + *pRealFlowRate);
        // pRealFlowRate
        //  float AvgFloWVal= Avg_flow_value / (float) flow_read_count;
        // printf("Flow Read Count = %d\n", flow_read_count);
        // printf("Flow Value is: %0.2f\n", flow_value);
        Avg_flow_value1 = (Avg_flow_value) / (float)flow_read_count;
        // TotalLit        =  Avg_flow_value *( (float)flow_read_count / literTm_Frac); //60
        TotalLit = TotalLit + Avg_flow_value1 * (1 / literTm_Frac); // 60
        // TotalLit        =  Avg_flow_value * (float)flow_read_count * 0.01666667* (2.6 * 60); //60
        total_liters = TotalLit + total_liters1;
        totalliterPtr = &total_liters;
        TotalliterInt = (int)total_liters;
        TotalLiterFloat = (total_liters - (float)TotalliterInt) * hundread;
    }
    else
    {
        if (literMeasureNum == 1)
        {
            /****Upload data on NVS */
            // write_TotalLiter(total_liters);
            // readTotalLiters();                      //===============Added
            // StartLTRCountVal = total_liters1;       //===============Added
            StopLTRCountVal = total_liters;
            targetLiters = (totalSecond / 60.0) * atoi(_xsSProllerbuf);
            effectiveLiters = StopLTRCountVal - StartLTRCountVal;
            variationLiters = hundread - ((effectiveLiters * hundread) / targetLiters);
            flow_read_count = 0;
            /**Upload data on NVS Done **/
            literMeasureNum = 0; // Reset flag

            //====================
            Avg_flow_value = 0;
            TotalLit = 0;
            //========================
        }
    }
}

void flowCounter_task(void *pvParameters)
{

    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = flowCounttimer_callback,
        .name = "flow Time Counter"};
    esp_timer_handle_t flowCoun_handle;
    esp_timer_create(&esp_timer_create_args, &flowCoun_handle);
    esp_timer_start_periodic(flowCoun_handle, 1000000);
    // printf("Timer Start time: %s \n", guiTime);

    // while(true)
    // {
    // }

    vTaskDelete(NULL);
}
/**
 * @brief This task functin will calculate the volume while motor is running
 *
 * @param pvParamters
 */
void pump_task(void *pvParamters)
{
    // vTaskPrioritySet( ina3221_task, 0 );
    //  setSensorTasksTOzeroPriority();
    int setzero = 0;
    tickCoutptr = &setzero;
    totalliterPtr = &setzero;
    totalhourPtr = &setzero;

    while (1)
    {

        // gpio_pad_select_gpio(34);
        // gpio_set_direction(34, GPIO_MODE_INPUT); // WakeMode
        // int gpio34 = gpio_get_level(34);
        // printf("GPIO Interrrupt Value is: %d\n", gpio34);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // printf("value of check flag w = %d\n", w);

        //==============================test code===========================

        if (chk == 0)
        {
            if (dashboardflg == 3)
            {
                abh = strcmp(guiTime, _xaHourMinrollerbuf);
                // printf("Test value is: %d\n" , abh);
                if (abh > 0)
                {
                    isMotor = true;
                    chk = 1;
                    vinfoWgtUpdtWaitToProgTask();
                }
            }
        }

        //===================================================================

        if (isMotor == true)
        {
            // vTaskPrioritySet( ina3221_task, 1 );
            setSensorTasksTOonePriority();
            // xTaskNotifyGive(receiverHandler_test);
            timerOn = true;
            // dutyCycleVal = 2000;
            HourCount++;
            tickCoutptr = &HourCount;

            //=============================================================================

            // setpoint_val = SetPoint_Value; // Set point from preset screen
            // //printf("Set point = %f\n", setpoint_val);
            // pid = pid_create(&ctrldata, flowrateptr, &output, &setpoint_val);
            // pid_limits(pid, 80, 1000);//80-1000
            // pid_auto(pid);
            // pid_compute(pid);                   // Compute new PID output value
            // rpm = (uint16_t)output;
            // //printf("PWM = %d\n", rpm);

            // ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, rpm));
            // ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2));

            //========================================================================

            // totalhourVal1 = totalhourVal +  (float) HourCount / 3600.0 ;
            // StopHrEND = totalhourVal1;
            // totalhourPtr = &totalhourVal1;
            // //printf("Current Hour counter value : %f \n", totalhourVal);
            // targetHr = (float)totalSecond / 3600.0;
            // effectiveHr = ((float)HourCount )/ 3600.0;
            // variationHr = 100.0 - ((effectiveHr * 100.0) / targetHr);
            // totalHourInt = (int)totalhourVal1;
            // totalHourFloat = (totalhourVal1 - (float)totalHourInt)*100;

            //=============================================================================

            // printf("Hour Count Value: %d \n",HourCount );
            //  if(HourCount >= totalSecond )
            //  {
            //      printf("save hour counter \n");
            //      setSensorTasksTOzeroPriority();

            //     isMotor = false;
            //     HourCount =0;
            //     //totalhourVal = totalhourVal +  (float) HourCount / 3600.0 ;
            //     StopHrEND = totalhourVal;
            //     //printf("Current Hour counter value : %f \n", totalhourVal);
            //     writeTotalhour(totalhourVal1); //
            //     targetHr = (float)totalSecond / 3600.0;
            //     effectiveHr = ((float)HourCount )/ 3600.0;
            //     variationHr = 100.0 - ((effectiveHr * 100.0) / targetHr);
            //     totalHourInt = (int)totalhourVal1;
            //     totalHourFloat = (totalhourVal1 - (float)totalHourInt)*100;
            //     //printf("PumpStopForcefully: %d \n", PumpStopForcefully);
            //     if( PumpStopForcefully != true )
            //     {
            //         //printf("Change InfoWidget \n");
            //         lv_task_del(ResInfoPerChange_task);
            //         sprintf(stopDateEnd, "%s", guiDate);
            //         sprintf(stopTimeEnd, "%sH%sM", guiHrDef, guiMinDef);
            //         //setSensorTasksTOzeroPriority();
            //         vUpdateInfoWidgetTask();
            //     }

            //     if(!isMotor){setSensorTasksTOzeroPriority();}

            //     PumpStopForcefully = false;
            // }
        }
        else
        {
            // ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, 0));
            // ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2));
            /**
             * @brief stopping the motor after the time is over
             *
             */
            MotorPWMStop();

            dutyCycleVal = 0;
            timerOn = false;
        }
    }

    vTaskDelete(NULL);
}

void ResInfoPerCount_task(void *pvParamters)
{

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));

        if (isMotor == true)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            workProgressCount++;
            workProgress = (workProgressCount * 100) / totalSecond;
            // printf("Work Progress percentage = %d \n", workProgress);

            Job_Rem_Hr = (totalSecond - workProgressCount) / 3600;
            Job_Rem_Min = ((totalSecond - workProgressCount) % 3600) / 60;
        }
        else
        {
            workProgressCount = 0;
        }
    }

    vTaskDelete(NULL);
}

// buzzer_on = true;

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

void terminalPrint_task(void *pvParamters)
{

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        log_guiDate_t queueGUIDate;
        log_guiTime_t queueGUITime;
        log_SDPsensor_t Q_SDP;
        log_BME280sensor_t Q_BME280;
        log_IntBME280sensor_t Q_IntBME280;
        log_INAsensor_t INASensor1;
        // xQueueReceive(queueDate,  &queueGUIDate, 2000 / portTICK_PERIOD_MS);
        // xQueueReceive(queueTime,  &queueGUITime, 2000 / portTICK_PERIOD_MS);
        // xQueueReceive(queueSDP,  &Q_SDP, 2000 / portTICK_PERIOD_MS);
        // xQueueReceive(queueBME280,  &Q_BME280, 2000 / portTICK_PERIOD_MS);
        // xQueueReceive(queueIntBME280,  &Q_IntBME280, 2000 / portTICK_PERIOD_MS);
        // xQueueReceive(queueINA3221,  &INASensor1, 2000 / portTICK_PERIOD_MS);

        if (isMotor == true)
        {

            if (xQueueReceive(queueDate, &queueGUIDate, 200 / portTICK_PERIOD_MS))
            {
                sprintf(Rec_Date, "%s", queueGUIDate.QguiDate);
                printdata = false;
            }
            else
            {
                printdata = true;
            }
            if (xQueueReceive(queueTime, &queueGUITime, 200 / portTICK_PERIOD_MS))
            {
                sprintf(Rec_time, "%s", queueGUITime.QguiTime);
                printdata = false;
            }
            else
            {
                printdata = true;
            }
            if (xQueueReceive(queueSDP, &Q_SDP, 200 / portTICK_PERIOD_MS))
            {
                Rec_dp_ticks = Q_SDP.dp_ticks_SDP;
                Rec_Out = Q_SDP.out_SDP;
                Rec_temperature_ticks = Q_SDP.temperature_ticks_SDP;
                Rec_flow_value = Q_SDP.flow_value_SDP;
                Rec_Polyflow_value = Q_SDP.polyFlow;
                printdata = false;
            }
            else
            {
                printdata = true;
            }
            if (xQueueReceive(queueBME280, &Q_BME280, 200 / portTICK_PERIOD_MS))
            {
                Rec_pressureBME = Q_BME280.Qpressure_BME;
                Rec_tempBME = Q_BME280.Qtemp_BME;
                Rec_humidityBME = Q_BME280.Qhumidity_BME;
                printdata = false;
            }
            else
            {
                printdata = true;
            }
            if (xQueueReceive(queueIntBME280, &Q_IntBME280, 200 / portTICK_PERIOD_MS))
            {
                Rec_pressureBME_Int = Q_IntBME280.Qpressure_IntBME;
                Rec_tempBME_Int = Q_IntBME280.Qtemp_IntBME;
                Rec_humidityBME_Int = Q_IntBME280.Qhumidity_IntBME;
                printdata = false;
            }
            else
            {
                printdata = true;
            }
            if (xQueueReceive(queueINA3221, &INASensor1, 200 / portTICK_PERIOD_MS))
            {
                recC1BusVoltPtr = INASensor1.qC1BusVoltPtr;
                recC1ShuntVoltPtr = INASensor1.qC1ShuntVoltPtr;
                recC1ShuntCurrentPtr = INASensor1.qC1ShuntCurrentPtr;

                recC2BusVoltPtr = INASensor1.qC2BusVoltPtr;
                recC2ShuntVoltPtr = INASensor1.qC2ShuntVoltPtr;
                recC2ShuntCurrentPtr = INASensor1.qC2ShuntCurrentPtr;

                recC3BusVoltPtr = INASensor1.qC3BusVoltPtr;
                recC3ShuntVoltPtr = INASensor1.qC3ShuntVoltPtr;
                recC3ShuntCurrentPtr = INASensor1.qC3ShuntCurrentPtr;

                printdata = false;
            }
            else
            {
                printdata = true;
            }

            if (isMotor == true)
            {

                if (printHeader)
                {
                    printf("------------------Settings-----------------------\n");
                    printf(" kp is: %0.2f || Ki is: %0.2f || kd is: %0.2f || Akp is :%0.2f || Aki is: %0.2f||Akd is: %0.2f \n", fGetPIDParameterKp(), fGetPIDParameterKi(), fGetPIDParameterKd(), fGetPIDParameterAkp(), fGetPIDParameterAki(), fGetPIDParameterAkd());
                    printf("------------------Settings-----------------------\n");
                    // printf("Date,Time, dp_ticks_SDP,out_SDP, temperature_ticks_SDP, flow_value_SDP, polyFlowOut, Qpressure_BME, Qtemp_BME, Qhumidity_BME, totalliter, ");
                    // printf("TotalHour,tickCount,C1BusVolt,C1ShuntVolt,C1ShuntCurr,C2BusVolt,C2ShuntVolt,C2ShuntCurr,RealFlowRate, AirDensity,");
                    // printf("C3BusVolt,C3ShuntVolt,C3ShuntCurr\n");
                    // printf("Qpressure_IntBME, Qtemp_IntBME, Qhumidity_IntBME \n");
                    printf("Date,Time, dp_ticks_SDP,out_SDP, temperature_ticks_SDP, flow_value_SDP, polyFlowOut, Qpressure_BME, Qtemp_BME, Qhumidity_BME, totalliter, TotalHour,tickCount,C1BusVolt,C1ShuntVolt,C1ShuntCurr,C2BusVolt,C2ShuntVolt,C2ShuntCurr,C3BusVolt,C3ShuntVolt,C3ShuntCurr,RealFlowRate, AirDensity\n");
                    printHeader = !printHeader;
                }

                printf("%s", Rec_Date);
                printf(",");
                printf("%s", Rec_time);
                printf(",");
                printf("%d", Rec_dp_ticks);
                printf(",");
                printf("%0.2f", getSdp32SensorAverageValue());
                printf(",");
                printf("%f", sdp32Temprature);
                printf(",");
                printf("%0.2f", Rec_flow_value);
                printf(",");
                printf("%0.2f", Rec_Polyflow_value);
                printf(",");
                printf("%0.2f", Rec_pressureBME);
                printf(",");
                printf("%0.2f", Rec_tempBME);
                printf(",");
                printf("%0.2f", Rec_humidityBME);
                printf(",");
                printf("%0.2lf", *totalliterPtr);
                printf(",");
                printf("%0.2f", *totalhourPtr);
                printf(",");
                printf("%d", *tickCoutptr);
                printf(",");

                printf("%0.2f", recC1BusVoltPtr);
                printf(",");
                printf("%0.2f", recC1ShuntVoltPtr);
                printf(",");
                printf("%0.2f", recC1ShuntCurrentPtr);
                printf(",");

                printf("%0.2f", recC2BusVoltPtr);
                printf(",");
                printf("%0.2f", recC2ShuntVoltPtr);
                printf(",");
                printf("%0.2f", recC2ShuntCurrentPtr);
                printf(",");

                printf("%0.2f", recC3BusVoltPtr);
                printf(",");
                printf("%0.2f", recC3ShuntVoltPtr);
                printf(",");
                printf("%0.2f", recC3ShuntCurrentPtr);
                printf(",");
                printf("%0.4f", *pRealFlowRate);
                printf(",");
                printf("%0.4f", *pAirDensityIn);
                printf(",");

                printf("\n");
            }

            //==================================
            //==================================

            // xSemaphoreGive(xGuiSemaphore);
        }
    }
    vTaskDelete(NULL);
}

void ws2812_task(void *pvParamters)
{

    ws2812_init(13);
    // const uint8_t pixel_count = 1; // Number of your "pixels"
    // Ready 0x5D,0xAF,0x48 (r g b)
    // problem 0xC3,0x34,0x38
    // Metrology in progress 0x7A,0xD0,0xB7
    // Work in Progress 0x54,0x83,0xAF
    // Wait 0xD5,0xDE,0x54
    // Job Finished 0x38, 0x67, 0xD6
    // Alert Service 0xEb, 0x3B, 0x5A
    // Metrology needed 0xFa, 0x82, 0x31
    // Export Data 0xA5, 0x5E, 0xea
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

/**
 * @brief this task control the motor when the metrological task is enable
 *
 */
void MetroFlowCalPumpControl_task(void)
{

    int mfcpm = 0;
    while (1)
    {
        // vTaskDelay(1);
        vTaskDelay(pdMS_TO_TICKS(1));
        if (metroFlowCalStarted)
        {
            mfcpm++;

            // setSensorTasksTOonePriority();
            /**
             * @brief starting the motor from the value obtained from the front end
             */
            MotorPWMStart(_fasDutyCycle);

            // ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, _fasDutyCycle));
            // ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2));

            if (mfcpm >= 1000)
            {
                printf("%s", guiDate);
                printf(",");
                printf("%s", rtcTimeNow);
                printf(",");
                printf("%0.2f", *flowrateptr);
                printf("\n");

                percentError = ((flow_value - flowPoint) / flowPoint) * 100.0;

                printf("Percentage error is = %0.2f\n", percentError);

                mfcpm = 0;
            }
            // printf("%s", guiDate);
            // printf(",");
            // printf("%s", rtcTimeNow);
            // printf(",");
            // printf("%0.2f", *flowrateptr);
            // printf("\n");
        }
        else
        {

            setSensorTasksTOzeroPriority();
        }
    }
    vTaskDelete(NULL);
}

void infoWgtUpdtWaitToProgTask_cb(lv_task_t infoWgtUpdtWaitToProgTask)
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

void setSensorTasksTOzeroPriority(void)
{
    // vTaskPrioritySet( ina3221_task, 0 );
    // vTaskPrioritySet( bme280_task, 0 );
    // vTaskPrioritySet( sdp32_task, 0 );
    // vTaskPrioritySet( terminalPrint_task, 0 );

    // vTaskSuspend( ina3221_task );
    // vTaskSuspend( bme280_task );
    // vTaskSuspend( sdp32_task );
    // vTaskSuspend( terminalPrint_task );
}

void setSensorTasksTOonePriority(void)
{
    // vTaskPrioritySet( ina3221_task, 1 );
    // vTaskPrioritySet( bme280_task, 1 );
    // vTaskPrioritySet( sdp32_task, 1 );
    // vTaskPrioritySet( terminalPrint_task, 1 );

    // vTaskResume( ina3221_task );
    // vTaskResume( bme280_task );
    // vTaskResume( sdp32_task );
    // vTaskResume( terminalPrint_task );
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static float _fDensityCalculation(float fTemperature, float fHumidity, float fPressure)
{
    return 1.0 / (287.06 * (fTemperature + 273.15)) *
           ((fPressure)-230.617 * fHumidity / 100.0 * exp((17.5043 * fTemperature) / (241.2 + fTemperature)));
}

static float _fFlowCalculation(float fDeltaPressure, float fDensity)
{
    float fResult = 0.0;
    if (isnan(fDeltaPressure))
    {
        fResult = 0.0;
        return fResult;
    }

    // fResult = sqrt(fabs(fDeltaPressure)) / sqrt(fDensity) * 0.3758940103; // for test
    // fResult = (fDeltaPressure < 0.0) ? -fResult : fResult;                // check si négatif

    /**
     * @brief using the formula Q= F(dp)* den_char / den_real
     *
     * Putting F=1 and den_char =1.2
     *
     */
    // fResult = .118 * fDeltaPressure * 1.2/fDensity;
    /**
     * @brief updating the flow rate to flow value =  0,759 * SDPvalue^05288
     *
     */

    fResult = 0.759 * pow(fDeltaPressure, 0.5288) * (1.2 / fDensity);

    return fResult;
}

/**
 * @brief This task will calculate the diff pressure which is mass flaw compensation, temperature compensateion in the average mode.
 * The sensor should has been readed in the fast mode. The interval should be between 20ms-25ms. In this mode the masss flow compensated diff pressure is calculated in the arthemtic mean mode. If the sensor is calculated more than this duration the average will be in the exponential mean, which give wrong values. The samples values will be stored in the array. Later any function uses this array to calculate the average.
 *
 * @param pvParameters
 */
void sdp32Task(void *pvParameters)
{

    sdp32_t dev;
    memset(&dev, 0, sizeof(sdp32_t));

    /**
     * @brief read buffer 0 initialized
     *
     */
    uint8_t read_buff[9] = {0};

    /**
     * @brief variable to store the checksum
     *
     */
    uint8_t checksum;

    /**
     * @brief various sdp32 commands
     *
     */
    // uint8_t startContRead_cmd[2] = {0x36, 0x15}; // Start Read in continous Mode
    uint8_t stopContRead_cmd[2] = {0x3F, 0xF9}; // Stop Countinous measure command
    uint8_t softReset_cmd[2] = {0x00, 0x06};
    /**
     * @brief mass flow, temperature compensated diff pressure with average mode
     *
     */
    uint8_t massFlowRead[2] = {0x36, 0x03};

    /**
     * @brief initializing spd32 sensor
     *
     */
    ESP_ERROR_CHECK(sdp32_init_desc(&dev, SDP32_I2C_ADDRESS, 0, SDA_GPIO, SCL_GPIO));
    vTaskDelay(pdMS_TO_TICKS(20));

    /**
     * @brief sending stop continuous read command
     *
     */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
        sdp32_send_cmd_read(&dev, stopContRead_cmd);
        xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    ESP_LOGI(TAG_sdp32_task, "Starting sensor in massflow mode");
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

        sdp32_send_cmd_read(&dev, massFlowRead);
        xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    while (true)
    {

        vTaskDelay(pdMS_TO_TICKS(SDP32_SENSOR_READ_DURATION_IN_MS));
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {

            sdp32_read_pressure(&dev, read_buff); // actually reading the mass flow
            xSemaphoreGive(xGuiSemaphore);
        }
        /**
         * @brief checksum is the third byte of the array after the first two bytes which ar actual readings
         *
         */
        checksum = read_buff[2];

        /**
         * @brief checking the checksum, as the values we are getting are correct or not
         *
         */
        bool check = CheckCrc(read_buff, 2, checksum);
        /**
         * @brief if checksum failed then continue the loop, else store the value in the array
         *
         */
        if (!check)
        {
            continue;
        }
        else
        {
            int16_t massFlow = (read_buff[0] << 8) | read_buff[1];
            noOfSamplesSdp32[last_update_sensor_value_index] = ((float)massFlow) / SDP32_DIFF_PRESSURE_SCALE_FACTOR;
            if (last_update_sensor_value_index == NO_OF_SAMPLES_SDP32 - 1)
            {
                last_update_sensor_value_index = 0;
            }
            else
            {
                last_update_sensor_value_index++;
            }
            int16_t Temprature = (read_buff[3] << 8) | read_buff[4];
            sdp32Temprature = ((float)Temprature) / t_scale;
        }
    }
    vTaskDelete(NULL);
}

void motorPWMTask(void *pvParameters)
{

    float flowRate = 0;
    pRealFlowRate = &flowRate; // copying  so that we can print on task

    float fTempVariable = 0;

    /**
     * @brief volume flow in the single iteration
     *
     */
    float fVoulumeFlowInLocalPeriodTime = 0;

    /**
     * @brief intiating the pwm motor
     *
     */
    initiatePWMMotor();

    /**
     * @brief initialize pid controller
     *
     */
    initializePIDController();

    while (1)
    {

        vTaskDelay(motorWAIT_ON / portTICK_PERIOD_MS);

        /**
         * @brief if motor is runnng then we need to calculate the duty cycle so that to make the constant volume flow
         *
         */
        while (getIsMotorRunning())
        {

            ESP_LOGD(TAG, "AVERAGE SDP VALUE IN CALUCULATION IS %0.2f", getSdp32SensorAverageValue());
            /**
             * @brief calulating the current flow rate
             *
             */

            // flowRate = _fFlowCalculation(fGetSdp32DiffPressureAverageValue(), fGetAirDensityManuCompensationLayer());
            flowRate = fGetVolumetricFlowUserCompensated();

            if (isnan(flowRate))
            {
                ESP_LOGE(TAG, "flow rate is nan");
                flowRate = MINIMUN_FLOW_RATE;
            }
            /**
             * @brief compute the total volume flow in the system
             *
             */
            else
            {

                fTempVariable = fGetTotalLiterCount();
                ESP_LOGV(TAG,"FLOW rate IS %0.2f",flowRate);
                /**
                 * @brief total liters flow is flowRate in L/Min * time in ms /60*1000
                 *
                 */

                fTempVariable += ((flowRate * getMotorPIDSampleComputeTime())) / (60 * 1000);

                /**
                 * @brief updating the total liters flow in the variable
                 *
                 */
                vSetTotalLiterCount(fTempVariable);
            }

            // ESP_LOGD(TAG, "Flow Rate: %f and air densit is %f, average diff pressure is %f", flowRate, _airDensityIn, average_diff_pressure);

            /**
             * @brief computing the duty cycle and set it
             *
             */
            motorPidComputeAndSetOutput(flowRate);
            flow_value = flowRate;
            vTaskDelay(pdMS_TO_TICKS(getMotorPIDSampleComputeTime()));
        }
    }
}

float getSdp32SensorAverageValue()
{
    float sum = 0.0;
    for (uint8_t i = 0; i < NO_OF_SAMPLES_SDP32; i++)
    {
        sum += noOfSamplesSdp32[i];
    }

    return (sum / NO_OF_SAMPLES_SDP32);
}
/**********************
 *    ERROR ASSERT
 **********************/