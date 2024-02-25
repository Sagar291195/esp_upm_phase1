/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "../../lvgl/lvgl.h"
#include "guider_fonts/guider_fonts.h"
#include "guider_customer_fonts/guider_customer_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	lv_obj_t *screen_label_title;
	lv_obj_t *screen_label_subtitle;
	lv_obj_t *screen_dd_sensor;
	lv_obj_t *screen_label_sensor;
	lv_obj_t *screen_btn_start;
	lv_obj_t *screen_btn_start_label;
	lv_obj_t *screen_btn_stop;
	lv_obj_t *screen_btn_stop_label;
	lv_obj_t *screen_label_value;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif