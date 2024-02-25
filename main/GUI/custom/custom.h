// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_

#include "../generated/gui_guider.h"

typedef enum{
    BME680,
    BME280,
    SDP32,
    LED,
    NO_SENSOR,
}sensor_type_t;


void custom_init(lv_ui *ui);

void dropdown_sensor_value_changed(uint8_t event);
void start_button_clicked(void);
void stop_button_clicked(void);

sensor_type_t get_selected_sensor(void);

#endif /* EVENT_CB_H_ */
