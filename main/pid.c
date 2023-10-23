#include <stdbool.h>
#include <sys/param.h>
#include "esp_check.h"
#include "esp_log.h"


#include "esp_upm.h"

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/
#define TAG            "PID"

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
typedef struct pid_ctrl_block_t pid_ctrl_block_t;
typedef float (*pid_cal_func_t)(pid_ctrl_block_t *pid, float error);

struct pid_ctrl_block_t {
    float Kp; // PID Kp value
    float Ki; // PID Ki value
    float Kd; // PID Kd value
    float previous_err1; // e(k-1)
    float previous_err2; // e(k-2)
    float integral_err;  // Sum of error
    float last_output;  // PID output in last control period
    float max_output;   // PID maximum output limitation
    float min_output;   // PID minimum output limitation
    float max_integral; // PID maximum integral value limitation
    float min_integral; // PID minimum integral value limitation
    pid_cal_func_t calculate_func; // calculation function, depends on actual PID type set by user
};

/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           STATIC PROTOTYPE
 ********************************************************************************************/
 static float pid_calc_incremental(pid_ctrl_block_t *pid, float error);
 static float pid_calc_positional(pid_ctrl_block_t *pid, float error);

 /********************************************************************************************
 *                           STATIC FUNCTIONS
 ********************************************************************************************/

 /********************************************************************************************
 *                         
 ********************************************************************************************/
static float pid_calc_positional(pid_ctrl_block_t *pid, float error)
{
    float output = 0;
    
    pid->integral_err += error;                                         /* Add current error to the integral error */
    pid->integral_err = MIN(pid->integral_err, pid->max_integral);      /* If the integral error is out of the range, it will be limited */
    pid->integral_err = MAX(pid->integral_err, pid->min_integral);

    output = error * pid->Kp +                          /* Calculate the pid control value by location formula */
             (error - pid->previous_err1) * pid->Kd +   /* u(k) = e(k)*Kp + (e(k)-e(k-1))*Kd + integral*Ki */
             pid->integral_err * pid->Ki;

    output = MIN(output, pid->max_output);              /* If the output is out of the range, it will be limited */
    output = MAX(output, pid->min_output);

    pid->previous_err1 = error;         /* Update previous error */
    return output;
}

 /********************************************************************************************
 *                         
 ********************************************************************************************/
static float pid_calc_incremental(pid_ctrl_block_t *pid, float error)
{
    float output = 0;

    output = (error - pid->previous_err1) * pid->Kp +                                /* Calculate the pid control value by increment formula */
             (error - 2 * pid->previous_err1 + pid->previous_err2) * pid->Kd +       /* du(k) = (e(k)-e(k-1))*Kp + (e(k)-2*e(k-1)+e(k-2))*Kd + e(k)*Ki */
             error * pid->Ki +                                                       /* u(k) = du(k) + u(k-1) */   
             pid->last_output;          
    
    output = MIN(output, pid->max_output);      /* If the output is beyond the range, it will be limited */
    output = MAX(output, pid->min_output);

    pid->previous_err2 = pid->previous_err1;    /* Update previous error */
    pid->previous_err1 = error;
    pid->last_output = output;                  /* Update last output */
    return output;
}
/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
 
 /********************************************************************************************
 *                         
 ********************************************************************************************/
esp_err_t pid_new_control_block(const pid_ctrl_config_t *config, pid_ctrl_block_handle_t *ret_pid)
{
    esp_err_t ret = ESP_OK;
    pid_ctrl_block_t *pid = NULL;
   
    ESP_GOTO_ON_FALSE(config && ret_pid, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");     /* Check the input pointer */

    pid = calloc(1, sizeof(pid_ctrl_block_t));
    ESP_GOTO_ON_FALSE(pid, ESP_ERR_NO_MEM, err, TAG, "no mem for PID control block");
    ESP_GOTO_ON_ERROR(pid_update_parameters(pid, &config->init_param), err, TAG, "init PID parameters failed");
    *ret_pid = pid;
    return ret;

err:
    if (pid) {
        free(pid);
    }
    return ret;
}

 /********************************************************************************************
 *                         
 ********************************************************************************************/
esp_err_t pid_compute(pid_ctrl_block_handle_t pid, float input_error, float *ret_result)
{
    ESP_RETURN_ON_FALSE(pid && ret_result, ESP_ERR_INVALID_ARG, TAG, "invalid argument");
    *ret_result = pid->calculate_func(pid, input_error);
    return ESP_OK;
}


 /********************************************************************************************
 *                         
 ********************************************************************************************/
esp_err_t pid_update_parameters(pid_ctrl_block_handle_t pid, const pid_ctrl_parameter_t *params)
{
    ESP_RETURN_ON_FALSE(pid && params, ESP_ERR_INVALID_ARG, TAG, "invalid argument");
    pid->Kp = params->kp;
    pid->Ki = params->ki;
    pid->Kd = params->kd;
    pid->max_output = params->max_output;
    pid->min_output = params->min_output;
    pid->max_integral = params->max_integral;
    pid->min_integral = params->min_integral;

    /* Set the calculate function according to the PID type */
    switch (params->cal_type) 
    {
    case PID_CAL_TYPE_INCREMENTAL:
        pid->calculate_func = pid_calc_incremental;
        break;
    case PID_CAL_TYPE_POSITIONAL:
        pid->calculate_func = pid_calc_positional;
        break;
    default:
        ESP_RETURN_ON_FALSE(false, ESP_ERR_INVALID_ARG, TAG, "invalid PID calculation type:%d", params->cal_type);
    }
    return ESP_OK;
}

 /********************************************************************************************
 *                         
 ********************************************************************************************/
void save_pid_parameters(float fKp, float fKi, float fKd, float fAkp, float fAki, float fAkd, float fNcoff, float fACoff)
{
    ESP_LOGI(TAG, "kp %0.2f, ki %0.2f, kd %0.2f, akp %0.2f, aki %0.2f, akd %0.2f, ncoff %0.2f, acoff %0.2f", fKp, fKi, fKd, fAkp, fAki, fAkd, fNcoff, fACoff);
    struct_PID_parameters_t pid_param;
    pid_param.fKp = fKp;
    pid_param.fKi = fKi;
    pid_param.fKd = fKd;
    pid_param.fAkp = fAkp;
    pid_param.fAki = fAki;
    pid_param.fAkd = fAkd;
    pid_param.fNcoff = fNcoff;
    pid_param.fACoff = fACoff;
    if (fNcoff == 0)
    {
        fNcoff = 1;
    }

    if (fACoff == 0)
    {
        fACoff = 1;
    }

    nvswrite_pid_parameters(&pid_param);    /* setting the pid parameters to the nvs flash */
    save_motor_pid_parameter(fKp / fNcoff, fKi / fNcoff, fKd / fNcoff, fAkp / fACoff, fAki / fACoff, fAkd / fACoff);   /* also setting the pid parameters to the pid control block */
}