
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
 #include <stdint.h>
#include <PID.h>
#include <stdbool.h>
#include <driver/ledc.h>


 /********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define DUTY_CYCLE_RESOLUTION   LEDC_TIMER_16_BIT
#define MOTOR_FREQ              1000         //Running frequency of motor
#ifdef CONFIG_NEW_HW
#define GPIO_MOTOR              25   //Gpio of the motor
#else
#define GPIO_MOTOR              4    //Gpio of the motor
#endif
#define motorWAIT_ON            (100)   /** Loop delay to wait motor on when interupt is already on (in ms) */
#define motorPULSE_BY_CYCLE     (3.0)   /** Number of pulse for a complete rotation */
#define motorMIN_SET            (0.0)   /** Motor minimum command possible */
#define motorMAX_SET            (100.0) /** Motor maximum command possible */
#define MINIMUN_FLOW_RATE       (1.0)
#define motorPID_DEFAULT_SETPOINT       (3.00)   /** Default PID setpoint config*/
#define motorPID_DEFAULT_ERROR          (0.05)   /** Default Error Config to trig aggressive PID in percentage*/
#define motorPID_SAMPLE_MIN             (100)    /** Minimum Sample for PID to process output */
#define motorPID_DEFAULT_SAMPLE         (403)    /** Default PID Sample */
#define motorPID_DEFAULT_ENTRY_POINT    (18.0)     /* set the entry point for the pid controller */
#define PID_COMPUT_TIME_AGGRESIVE_IN_MS (200)     /* pid compute time in ms in aggresive mode */
#define PID_COMPUTE_TIME_STABLE_IN_MS   (200)       /*  pid compute time in ms in stable mode */


/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
 
 /********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/

/**
 * @brief This function will initiate the pwm motor
 *
 */
void initiatePWMMotor();

/**
 * @brief this function will set the frequency of the motor
 *
 * @param freq frequency of the motor
 */
void motorPWMSetFrequecy(unsigned short freq);

/**
 * @brief Start the pwm motor with the given duty cycle
 *
 * @param fduty
 */
void MotorPWMStart(float fduty);

/**
 * @brief stop the ongoing pwm motor
 *
 */
void MotorPWMStop();

/**
 * @brief set the pwm duty cycle of the motor
 *
 * @param duty duty cycle of the motor in %
 */
void motorPWMSetDutyCycle(float duty);
/**
 * @brief initialixe the pid controller for the motor
 *
 */
void initializePIDController();

/**
 * @brief Get the Motor P I D Set Value
 *
 * @return float
 */
float getMotorPIDSetTargetValue();
/**
 * @brief Set the Motor P I D target flow value which user give from the screen . pid will compute the flow value and set the duty cycle
 *
 * @param setValue
 */
void setMotorPIDSetTargetValue(float setValue);

/**
 * @brief Set the Motor P I D Parameters to the pid controller
 *
 * @param fKp
 * @param fKi
 * @param fKd
 * @param fAkp
 * @param fAki
 * @param fAkd
 */
void setMotorPIDParameters(float fKp, float fKi, float fKd, float fAkp, float fAki, float fAkd);

/**
 * @brief Set the Motor Pid Limits object
 *
 * @param fMin min pid value
 * @param fMax max pid value
 */
void setMotorPidLimits(float fMin, float fMax);

/**
 * @brief Get the Is Motor Running
 *
 * @return true running else false
 */
bool getIsMotorRunning();

/**
 * @brief Set the State Of Motor
 *
 * @param state true if running else false
 */
void setStateOfMotor(bool state);

/**
 * @brief compute the motor pid output
 *
 * @param finput input flow rate
 */
void motorPidComputeAndSetOutput(float finput);

/**
 * @brief get the kp value of the motor pid controller
 *
 */
float fGetPIDParameterKp();
/**
 * @brief get the ki value of the motor pid controller
 *
 */
float fGetPIDParameterKi();

/**
 * @brief get the kd value of the motor pid controller
 *
 * @return float
 */
float fGetPIDParameterKd();

/**
 * @brief get teh akp value of the motor pid controller
 *
 * @return float
 */
float fGetPIDParameterAkp();

/**
 * @brief get the aki value of the motor pid controller
 *
 * @return float
 */
float fGetPIDParameterAki();

/**
 * @brief get the akd value of the motor pid controller
 * */

float fGetPIDParameterAkd();

/**
 * @brief Get the Motor P I D Sample Compute Time
 *
 * @return uint16_t retuns the pid compute time in ms
 */
uint16_t getMotorPIDSampleComputeTime();

/**
 * @brief Initailize the motor and pid controller
 *
 */
void vInitializeMotor();

#ifdef __cplusplus
}
#endif

#endif // __MOTOR_H__