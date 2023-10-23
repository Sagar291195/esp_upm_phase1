#ifndef _PID_H
#define _PID_H

/********************************************************************************************
 *                              INCLUDES
 ********************************************************************************************/
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************************
 *                              DEFINES
 ********************************************************************************************/

/********************************************************************************************
 *                              TYPEDEFS
 ********************************************************************************************/
/* PID calculation type */
typedef enum {
    PID_CAL_TYPE_INCREMENTAL, /*!< Incremental PID control */
    PID_CAL_TYPE_POSITIONAL,  /*!< Positional PID control */
} pid_calculate_type_t;

/* PID control parameters */
typedef struct {
    float kp;                      // PID Kp parameter
    float ki;                      // PID Ki parameter
    float kd;                      // PID Kd parameter
    float max_output;              // PID maximum output limitation
    float min_output;              // PID minimum output limitation
    float max_integral;            // PID maximum integral value limitation
    float min_integral;            // PID minimum integral value limitation
    pid_calculate_type_t cal_type; // PID calculation type
} pid_ctrl_parameter_t;

/* PID control configuration */
typedef struct {
    pid_ctrl_parameter_t init_param; // Initial parameters
} pid_ctrl_config_t;

typedef struct pid_ctrl_block_t *pid_ctrl_block_handle_t;    /* Type of PID control block handle */
/********************************************************************************************
 *                           GLOBAL VARIABLES
 ********************************************************************************************/

/********************************************************************************************
 *                           GLOBAL FUNCTIONS
 ********************************************************************************************/
/**
 * @brief Create a new PID control session, returns the handle of control block
 *
 * @param[in] config PID control configuration
 * @param[out] ret_pid Returned PID control block handle
 * @return
 *      - ESP_OK: Created PID control block successfully
 *      - ESP_ERR_INVALID_ARG: Created PID control block failed because of invalid argument
 *      - ESP_ERR_NO_MEM: Created PID control block failed because out of memory
 */
esp_err_t pid_new_control_block(const pid_ctrl_config_t *config, pid_ctrl_block_handle_t *ret_pid);

/**
 * @brief Update PID parameters
 *
 * @param[in] pid PID control block handle, created by `pid_new_control_block()`
 * @param[in] params PID parameters
 * @return
 *      - ESP_OK: Update PID parameters successfully
 *      - ESP_ERR_INVALID_ARG: Update PID parameters failed because of invalid argument
 */
esp_err_t pid_update_parameters(pid_ctrl_block_handle_t pid, const pid_ctrl_parameter_t *params);

/**
 * @brief Input error and get PID control result
 *
 * @param[in] pid PID control block handle, created by `pid_new_control_block()`
 * @param[in] input_error error data that feed to the PID controller
 * @param[out] ret_result result after PID calculation
 * @return
 *      - ESP_OK: Run a PID compute successfully
 *      - ESP_ERR_INVALID_ARG: Run a PID compute failed because of invalid argument
 */
esp_err_t pid_compute(pid_ctrl_block_handle_t pid, float input_error, float *ret_result);


void save_pid_parameters(float fKp, float fKi, float fKd, float fAkp, float fAki, float fAkd, float fNcoff,
             float fACoff);

#ifdef __cplusplus
}
#endif

#endif //_PID_H
