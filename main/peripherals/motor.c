/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 #include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "driver/pcnt.h"
#include <pid.h>
#include <esp_log.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include <math.h>
#include <userCompensationLayer.h>
#include <sampleManagement.h>
#include <sequenceManagement.h>
#include "esp_upm.h"

 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG         "MOTOR"
/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
 /* pid paramters for motor */
static pid_ctrl_parameter_t pid_runtime_param = {
    .kp = motorPID_DEFAULT_AKP,
    .ki = motorPID_DEFAULT_AKI,
    .kd = motorPID_DEFAULT_AKD,
    .cal_type = PID_CAL_TYPE_INCREMENTAL,
    .max_output = motorMAX_SET,
    .min_output = motorMIN_SET,
    .max_integral = 1000,
    .min_integral = -1000,
};

pid_ctrl_config_t pid_config;   /* pid control structure for motor */
static pid_ctrl_block_handle_t pid_ctrl;    /* handle for the control */
static float pid_setTargetVaule = 0;    /* pid set target value flow rate */
bool isMotorRunning = false;    /* determine the state of motor */
static float fKp;   /* setting the pid parameters of the pid controller */
static float fKi;   
static float fKd;
static float fAkp;
static float fAki;
static float fAkd;
uint16_t PID_COMPUTE_TIME_IN_MS = PID_COMPUT_TIME_AGGRESIVE_IN_MS;

 /********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
static void motorTask(void *pvParameters);
static void setMotorPwmPidSetKpKiKd(float fKp, float fKi, float fKd);
static void settedPIDParameters(void);

 /********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/
/********************************************************************************************
 *  
 ********************************************************************************************/
static void settedPIDParameters(void)
{
    ESP_LOGI(TAG, "KP %0.2f, KI %0.2f, KD %0.2f, AKP %0.2f, AKI %0.2f, AKD %0.2f", fKp, fKi, fKd, fAkp, fAki, fAkd);
}

/********************************************************************************************
 *  
 ********************************************************************************************/
static void motorTask(void *pvParameters)
{
    float flowRate = 0;
    float fTempVariable = 0;

    initiatePWMMotor(); /* intiating the pwm motor */
    initializePIDController();  /* initialize pid controller */

    while (1)
    {
        vTaskDelay(motorWAIT_ON / portTICK_PERIOD_MS);

        /* if motor is runnng then we need to calculate the duty cycle so that to make the constant volume flow */
        while (getIsMotorRunning())
        {
            ESP_LOGD(TAG, "AVERAGE SDP VALUE IN CALUCULATION IS %0.2f", get_sdp32_pressure_value());
           
            flowRate = fGetVolumetricFlowUserCompensated();  /* calulating the current flow rate */
            if (isnan(flowRate))
            {
                ESP_LOGD(TAG, "flow rate is nan");
                flowRate = MINIMUN_FLOW_RATE;
            }
            else
            {
                fTempVariable = fGetTotalLiterCount();
                fTempVariable += ((flowRate * getMotorPIDSampleComputeTime())) / (60 * 1000);   /* total liters flow is flowRate in L/Min * time in ms /60*1000 */
                ESP_LOGD(TAG, "FLOW rate IS from motor %0.2f, Total liter : %.2f", flowRate, fTempVariable);
                vSetTotalLiterCount(fTempVariable); /* updating the total liters flow in the variable */
            }
            motorPidComputeAndSetOutput(flowRate);  /* computing the duty cycle and set it */
            vTaskDelay(pdMS_TO_TICKS(getMotorPIDSampleComputeTime()));
        }
    }
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void setMotorPwmPidSetKpKiKd(float fKpv, float fKiv, float fKdv)
{
    pid_runtime_param.kp = fKpv;
    pid_runtime_param.ki = fKiv;
    pid_runtime_param.kd = fKdv;
    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_update_parameters(pid_ctrl, &pid_runtime_param));
}

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *  
 ********************************************************************************************/
void initiatePWMMotor()
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = DUTY_CYCLE_RESOLUTION,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = MOTOR_FREQ,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = GPIO_MOTOR,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0, // initial duty to zero
        .hpoint = 0};
    ledc_channel_config(&channel);
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void motorPWMSetFrequecy(unsigned short freq)
{
    ESP_ERROR_CHECK(ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, freq));
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void MotorPWMStart(float duty)
{
    motorPWMSetDutyCycle(duty);
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void MotorPWMStop()
{
    ESP_LOGD(TAG, "stopping the motor PWM");
    // mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    motorPWMSetDutyCycle(0);
    setStateOfMotor(false); // setting the state of motor to false
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void motorPWMSetDutyCycle(float duty)
{
    /*converting % into the duty cycle */
    uint32_t duty_cycle = (((pow(2, DUTY_CYCLE_RESOLUTION)) - 1) * duty) / 100;
    ESP_LOGD(TAG, "duty cycle is %d", duty_cycle);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void initializePIDController(void)
{
    pid_config.init_param = pid_runtime_param;
    ESP_ERROR_CHECK(pid_new_control_block(&pid_config, &pid_ctrl));

    struct_PID_parameters_t pid_parameters;
    /* getting the parameters of the pid controller from nvs flash, if not found, the below function
     * will set the default values */
    vGetPIDParametersFromNvs(&pid_parameters);

    /* configuring  the parameters of the pid controller */
    setMotorPIDParameters(pid_parameters.fKp / pid_parameters.fNcoff,
                          pid_parameters.fKi / pid_parameters.fNcoff,
                          pid_parameters.fKd / pid_parameters.fNcoff,
                          pid_parameters.fAkp / pid_parameters.fACoff,
                          pid_parameters.fAki / pid_parameters.fACoff,
                          pid_parameters.fAkd / pid_parameters.fACoff);
    settedPIDParameters();
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float getMotorPIDSetTargetValue()
{
    return pid_setTargetVaule;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void setMotorPIDSetTargetValue(float setValue)
{
    ESP_LOGD(TAG, "Target set value is %f", setValue);
    pid_setTargetVaule = setValue;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void setMotorPidLimits(float fMin, float fMax)
{
    pid_runtime_param.min_output = fMin;
    pid_runtime_param.max_output = fMax;

    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_update_parameters(pid_ctrl, &pid_runtime_param));
}

/********************************************************************************************
 *  
 ********************************************************************************************/
bool getIsMotorRunning()
{
    return isMotorRunning;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void setStateOfMotor(bool state)
{
    ESP_LOGD(TAG, "state is %d", state);
    isMotorRunning = state;
    motorPWMSetDutyCycle(0); // setting the motor to stop
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void motorPidComputeAndSetOutput(float input)
{
    
    float fOutputDuty = 0;

    
    ESP_LOGD(TAG, "input %f, setpoint %f", input, pid_setTargetVaule);
    float error = (pid_setTargetVaule - input) / pid_setTargetVaule;    /*computing the error percentage  */
    /* if error is less than the default error then set the normal pid pramaters */
    if (fabs(error) < motorPID_DEFAULT_ERROR)
    {
        
        setMotorPwmPidSetKpKiKd(fKp, fKi, fKd);
        PID_COMPUTE_TIME_IN_MS = PID_COMPUTE_TIME_STABLE_IN_MS;
    }
    else
    {
        ESP_LOGD(TAG, "ERROR %f", error);
        setMotorPwmPidSetKpKiKd(fAkp, fAki, fAkd);
        PID_COMPUTE_TIME_IN_MS = PID_COMPUT_TIME_AGGRESIVE_IN_MS;
    }
    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_compute(pid_ctrl, error, &fOutputDuty));
    motorPWMSetDutyCycle(fOutputDuty);      /*setiing the motor duty cycle */
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void setMotorPIDParameters(float fKpv, float fKiv, float fKdv, float fAkpv, float fAkiv, float fAkdv)
{
    fKp = fKpv;
    fKi = fKiv;
    fKd = fKdv;
    fAkp = fAkpv;
    fAki = fAkiv;
    fAkd = fAkdv;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterKp()
{
    return fKp;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterKi()
{
    return fKi;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterKd()
{
    return fKd;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterAkp()
{
    return fAkp;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterAki()
{
    return fAki;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
float fGetPIDParameterAkd()
{
    return fAkd;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
uint16_t getMotorPIDSampleComputeTime()
{
    return PID_COMPUTE_TIME_IN_MS;
}

/********************************************************************************************
 *  
 ********************************************************************************************/
void motor_initialization()
{
    xTaskCreate(motorTask, "motorTask", 2 * 2048, NULL, 5, NULL);
}