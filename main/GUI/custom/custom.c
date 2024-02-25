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

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
sensor_type_t selectedsensor = NO_SENSOR;

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    lv_dropdown_set_selected(guider_ui.screen_dd_sensor, 0);
}


void dropdown_sensor_value_changed(uint8_t event)
{
    static bool dropdown_enable = false;

    if( event == LV_EVENT_RELEASED )
    {
        if( dropdown_enable == false )
        {
            dropdown_enable = true;
            lv_obj_set_click(guider_ui.screen_btn_start, false);
            lv_obj_set_click(guider_ui.screen_btn_stop, false);
        }
        else
        {
            dropdown_enable = false;
            lv_obj_set_click(guider_ui.screen_btn_start, true);
            lv_obj_set_click(guider_ui.screen_btn_stop, true);
        }
    }
    else if( event == LV_EVENT_VALUE_CHANGED )
    {
        selectedsensor = lv_dropdown_get_selected(guider_ui.screen_dd_sensor);
    }
}

void start_button_clicked(void)
{
    if (get_device_state() == STATE_IDLE)
    {
        set_device_state(STATE_START_TEST);
    }
}

void stop_button_clicked(void)
{
    if(get_device_state() != STATE_IDLE)
    {
        set_device_state(STATE_STOP_TEST);
    }
}


sensor_type_t get_selected_sensor(void)
{
    return selectedsensor;
}