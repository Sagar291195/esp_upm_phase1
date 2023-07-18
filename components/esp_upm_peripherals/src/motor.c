/**
 * @file motor.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief This file contains the implementation of the motor and its realted functions
 * @version 1.1
 * @date 2022-05-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <motor.h>
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "driver/pcnt.h"
#include <PID.h>
#include <esp_log.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include <math.h>
#include <middleware.h>
#include <userCompensationLayer.h>
#include <sampleManagement.h>
#include <sequenceManagement.h>

/**********************************************define************************************************/
#define TAG     "MOTOR"

/***************************************function prototypes***********************************************/
static void motorTask(void *pvParameters);

/****************************************************variables************************************/

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

/* pid control structure for motor */
pid_ctrl_config_t pid_config;
/* handle for the control */
static pid_ctrl_block_handle_t pid_ctrl;
/* pid set target value flow rate */
static float pid_setTargetVaule = 0;
/* determine the state of motor */
bool isMotorRunning = false;
/* setting the pid parameters of the pid controller */
static float fKp;
static float fKi;
static float fKd;
static float fAkp;
static float fAki;
static float fAkd;
uint16_t PID_COMPUTE_TIME_IN_MS = PID_COMPUT_TIME_AGGRESIVE_IN_MS;

/*********************************functions*******************************/

static void settedPIDParameters()
{
    ESP_LOGI(TAG, "KP %0.2f, KI %0.2f, KD %0.2f, AKP %0.2f, AKI %0.2f, AKD %0.2f", fKp, fKi, fKd, fAkp, fAki, fAkd);
}
/**
 * @brief set motor pid parameters
 */
void setMotorPwmPidSetKpKiKd(float fKp, float fKi, float fKd);

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

void motorPWMSetFrequecy(unsigned short freq)
{
    ESP_ERROR_CHECK(ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, freq));
}

void MotorPWMStart(float duty)
{

    motorPWMSetDutyCycle(duty);
}

void MotorPWMStop()
{
    ESP_LOGD(TAG, "stopping the motor PWM");
    // mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    motorPWMSetDutyCycle(0);
    setStateOfMotor(false); // setting the state of motor to false
}

void motorPWMSetDutyCycle(float duty)
{ 
    /*converting % into the duty cycle */
    uint32_t duty_cycle = (((pow(2, DUTY_CYCLE_RESOLUTION)) - 1) * duty) / 100;
    ESP_LOGD(TAG, "duty cycle is %d", duty_cycle);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
}

void initializePIDController()
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

float getMotorPIDSetTargetValue()
{
    return pid_setTargetVaule;
}

void setMotorPIDSetTargetValue(float setValue)
{
    ESP_LOGD(TAG, "Target set value is %f", setValue);
    pid_setTargetVaule = setValue;
}

void setMotorPwmPidSetKpKiKd(float fKpv, float fKiv, float fKdv)
{

    pid_runtime_param.kp = fKpv;
    pid_runtime_param.ki = fKiv;
    pid_runtime_param.kd = fKdv;
    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_update_parameters(pid_ctrl, &pid_runtime_param));
}

void setMotorPidLimits(float fMin, float fMax)
{
    pid_runtime_param.min_output = fMin;
    pid_runtime_param.max_output = fMax;

    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_update_parameters(pid_ctrl, &pid_runtime_param));
}

bool getIsMotorRunning()
{
    return isMotorRunning;
}

void setStateOfMotor(bool state)
{
    ESP_LOGD(TAG, "state is %d", state);
    isMotorRunning = state;
     motorPWMSetDutyCycle(0); // setting the motor to stop
}

void motorPidComputeAndSetOutput(float input)
{
    ESP_LOGD(TAG, "input %f", input);
    ESP_LOGD(TAG, "setpoint %f", pid_setTargetVaule);
    float fOutputDuty = 0;
    
    /*computing the error percentage  */
    float error = (pid_setTargetVaule - input)/pid_setTargetVaule;
    ESP_LOGD(TAG, "ERROR %f", error);
    /* if error is less than the default error then set the normal pid pramaters */
    if (fabs(error) < motorPID_DEFAULT_ERROR)
    {
        setMotorPwmPidSetKpKiKd(fKp, fKi, fKd);
        PID_COMPUTE_TIME_IN_MS = PID_COMPUTE_TIME_STABLE_IN_MS;
    }
    else
    {
        setMotorPwmPidSetKpKiKd(fAkp, fAki, fAkd);
        PID_COMPUTE_TIME_IN_MS = PID_COMPUT_TIME_AGGRESIVE_IN_MS;
    }
    ESP_ERROR_CHECK_WITHOUT_ABORT(pid_compute(pid_ctrl, error, &fOutputDuty));
    ESP_LOGD(TAG, "output %f", fOutputDuty);

    /*setiing the motor duty cycle */
    motorPWMSetDutyCycle(fOutputDuty);
}

void setMotorPIDParameters(float fKpv, float fKiv, float fKdv, float fAkpv, float fAkiv, float fAkdv)
{
    fKp = fKpv;
    fKi = fKiv;
    fKd = fKdv;
    fAkp = fAkpv;
    fAki = fAkiv;
    fAkd = fAkdv;
}

float fGetPIDParameterKp()
{
    return fKp;
}

float fGetPIDParameterKi()
{
    return fKi;
}

float fGetPIDParameterKd()
{
    return fKd;
}

float fGetPIDParameterAkp()
{
    return fAkp;
}

float fGetPIDParameterAki()
{
    return fAki;
}

float fGetPIDParameterAkd()
{
    return fAkd;
}

uint16_t getMotorPIDSampleComputeTime()
{
    return PID_COMPUTE_TIME_IN_MS;
}


static void motorTask(void *pvParameters)
{
    float flowRate = 0;
    float fTempVariable = 0;
    /*  volume flow in the single iteration  */
    float fVoulumeFlowInLocalPeriodTime = 0;

    /* intiating the pwm motor */
    initiatePWMMotor();
    /* initialize pid controller */
    initializePIDController();

    while (1)
    {

        vTaskDelay(motorWAIT_ON / portTICK_PERIOD_MS);

        /* if motor is runnng then we need to calculate the duty cycle so that to make
         * the constant volume flow */
        while (getIsMotorRunning())
        {

            ESP_LOGD(TAG, "AVERAGE SDP VALUE IN CALUCULATION IS %0.2f", getSdp32SensorAverageValue());
            /* calulating the current flow rate */

            // flowRate = _fFlowCalculation(fGetSdp32DiffPressureAverageValue(), fGetAirDensityManuCompensationLayer());
            flowRate = fGetVolumetricFlowUserCompensated();

            if (isnan(flowRate))
            {
                ESP_LOGD(TAG, "flow rate is nan");
                flowRate = MINIMUN_FLOW_RATE;
            }
            /* compute the total volume flow in the system */
            else
            {
                fTempVariable = fGetTotalLiterCount();
                ESP_LOGV(TAG,"FLOW rate IS %0.2f",flowRate);
                /* total liters flow is flowRate in L/Min * time in ms /60*1000 */
                fTempVariable += ((flowRate * getMotorPIDSampleComputeTime())) / (60 * 1000);
                /* updating the total liters flow in the variable */
                vSetTotalLiterCount(fTempVariable);
            }
            /* computing the duty cycle and set it */
            motorPidComputeAndSetOutput(flowRate);
            flow_value = flowRate;
            vTaskDelay(pdMS_TO_TICKS(getMotorPIDSampleComputeTime()));
        }
    }
}


void vInitializeMotor()
{
    xTaskCreate(motorTask, "motorTask", 2*2048, NULL, 5, NULL);
}