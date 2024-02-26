/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 */

#include "events_init.h"
#include <stdio.h>
#include "../../lvgl/lvgl.h"


#include "../custom/custom.h"

void events_init(lv_ui *ui)
{
}

static void screen_dd_sensorevent_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		dropdown_sensor_value_changed();
	}
		break;
	default:
		break;
	}
}

static void screen_btn_startevent_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		start_button_clicked();
	}
		break;
	default:
		break;
	}
}

static void screen_btn_stopevent_handler(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		stop_button_clicked();
	}
		break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_set_event_cb(ui->screen_dd_sensor, screen_dd_sensorevent_handler);
	lv_obj_set_event_cb(ui->screen_btn_start, screen_btn_startevent_handler);
	lv_obj_set_event_cb(ui->screen_btn_stop, screen_btn_stopevent_handler);
}
