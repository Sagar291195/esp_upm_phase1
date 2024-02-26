// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "../../lvgl/lvgl.h"
#include "custom.h"
#include "esp_upm.h"
#include "esp_log.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
#define TAG     "GUI"
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
sensor_type_t selectedsensor = BME680;

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    lv_dropdown_set_selected(guider_ui.screen_dd_sensor, 0);
}


void dropdown_sensor_value_changed( void )
{
    selectedsensor = lv_dropdown_get_selected(guider_ui.screen_dd_sensor);
}

void start_button_clicked(void)
{
    ESP_LOGI(TAG, "start button clicked");
    if (get_device_state() == STATE_IDLE)
    {
        set_device_state(STATE_START_TEST);
    }
}

void stop_button_clicked(void)
{
    ESP_LOGI(TAG, "stop button clicked");
    if(get_device_state() != STATE_IDLE)
    {
        lv_label_set_text(guider_ui.screen_label_value, "Sensor test stop");
        set_device_state(STATE_STOP_TEST);
    }
}


sensor_type_t get_selected_sensor(void)
{
    selectedsensor = lv_dropdown_get_selected(guider_ui.screen_dd_sensor);
    return selectedsensor;
}