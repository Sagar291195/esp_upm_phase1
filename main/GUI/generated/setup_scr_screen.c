/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 */

#include "../../lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "../custom/custom.h"


void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL, NULL);

	//Write style LV_OBJ_PART_MAIN for screen
	static lv_style_t style_screen_main;
	lv_style_reset(&style_screen_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_main
	lv_style_set_bg_color(&style_screen_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_opa(&style_screen_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen, LV_OBJ_PART_MAIN, &style_screen_main);

	//Write codes screen_label_title
	ui->screen_label_title = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_label_title, "ESPUPM");
	lv_label_set_long_mode(ui->screen_label_title, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_title, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_title
	static lv_style_t style_screen_label_title_main;
	lv_style_reset(&style_screen_label_title_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_title_main
	lv_style_set_radius(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_title_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_color(&style_screen_label_title_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_dir(&style_screen_label_title_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_title_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_label_title_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_title_main, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_24);
	lv_style_set_text_letter_space(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_title_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_title, LV_LABEL_PART_MAIN, &style_screen_label_title_main);
	lv_obj_set_pos(ui->screen_label_title, 10, 10);
	lv_obj_set_size(ui->screen_label_title, 300, 0);

	//Write codes screen_label_subtitle
	ui->screen_label_subtitle = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_label_subtitle, "Production Test");
	lv_label_set_long_mode(ui->screen_label_subtitle, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_subtitle, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_subtitle
	static lv_style_t style_screen_label_subtitle_main;
	lv_style_reset(&style_screen_label_subtitle_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_subtitle_main
	lv_style_set_radius(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_color(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_dir(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_18);
	lv_style_set_text_letter_space(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_subtitle_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_subtitle, LV_LABEL_PART_MAIN, &style_screen_label_subtitle_main);
	lv_obj_set_pos(ui->screen_label_subtitle, 10, 40);
	lv_obj_set_size(ui->screen_label_subtitle, 300, 0);

	//Write codes screen_dd_sensor
	ui->screen_dd_sensor = lv_dropdown_create(ui->screen, NULL);
	lv_dropdown_set_options(ui->screen_dd_sensor, "BME680\nBME280\nSDP32\nLED");
	lv_dropdown_set_symbol(ui->screen_dd_sensor, NULL);

	//Write style LV_DROPDOWN_PART_MAIN for screen_dd_sensor
	static lv_style_t style_screen_dd_sensor_main;
	lv_style_reset(&style_screen_dd_sensor_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_dd_sensor_main
	lv_style_set_radius(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, 1);
	lv_style_set_text_color(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);
	lv_style_set_text_line_space(&style_screen_dd_sensor_main, LV_STATE_DEFAULT, 20);
	lv_obj_add_style(ui->screen_dd_sensor, LV_DROPDOWN_PART_MAIN, &style_screen_dd_sensor_main);

	//Write style LV_DROPDOWN_PART_SELECTED for screen_dd_sensor
	static lv_style_t style_screen_dd_sensor_selected;
	lv_style_reset(&style_screen_dd_sensor_selected);

	//Write style state: LV_STATE_DEFAULT for style_screen_dd_sensor_selected
	lv_style_set_radius(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, 1);
	lv_style_set_text_color(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_dd_sensor_selected, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);
	lv_obj_add_style(ui->screen_dd_sensor, LV_DROPDOWN_PART_SELECTED, &style_screen_dd_sensor_selected);

	//Write style LV_DROPDOWN_PART_LIST for screen_dd_sensor
	static lv_style_t style_screen_dd_sensor_list;
	lv_style_reset(&style_screen_dd_sensor_list);

	//Write style state: LV_STATE_DEFAULT for style_screen_dd_sensor_list
	lv_style_set_radius(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, 1);
	lv_style_set_text_color(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_screen_dd_sensor_list, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);
	lv_obj_add_style(ui->screen_dd_sensor, LV_DROPDOWN_PART_LIST, &style_screen_dd_sensor_list);
	lv_obj_set_pos(ui->screen_dd_sensor, 160, 100);
	lv_obj_set_width(ui->screen_dd_sensor, 120);

	//Write codes screen_label_sensor
	ui->screen_label_sensor = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_label_sensor, "Sensor Type");
	lv_label_set_long_mode(ui->screen_label_sensor, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_sensor, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_sensor
	static lv_style_t style_screen_label_sensor_main;
	lv_style_reset(&style_screen_label_sensor_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_sensor_main
	lv_style_set_radius(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_label_sensor_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_color(&style_screen_label_sensor_main, LV_STATE_DEFAULT, lv_color_make(0x5d, 0x5d, 0x5d));
	lv_style_set_bg_grad_dir(&style_screen_label_sensor_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_label_sensor_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_sensor_main, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);
	lv_style_set_text_letter_space(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_right(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_top(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_bottom(&style_screen_label_sensor_main, LV_STATE_DEFAULT, 5);
	lv_obj_add_style(ui->screen_label_sensor, LV_LABEL_PART_MAIN, &style_screen_label_sensor_main);
	lv_obj_set_pos(ui->screen_label_sensor, 30, 100);
	lv_obj_set_size(ui->screen_label_sensor, 120, 0);

	//Write codes screen_btn_start
	ui->screen_btn_start = lv_btn_create(ui->screen, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn_start
	static lv_style_t style_screen_btn_start_main;
	lv_style_reset(&style_screen_btn_start_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_start_main
	lv_style_set_radius(&style_screen_btn_start_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_btn_start_main, LV_STATE_DEFAULT, lv_color_make(0x32, 0x97, 0xa4));
	lv_style_set_bg_grad_color(&style_screen_btn_start_main, LV_STATE_DEFAULT, lv_color_make(0x32, 0x97, 0xa4));
	lv_style_set_bg_grad_dir(&style_screen_btn_start_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_start_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn_start_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xaf, 0xb2));
	lv_style_set_border_width(&style_screen_btn_start_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn_start_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn_start_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn_start_main, LV_STATE_DEFAULT, 0);

	//Write style state: LV_STATE_PRESSED for style_screen_btn_start_main
	lv_style_set_radius(&style_screen_btn_start_main, LV_STATE_PRESSED, 5);
	lv_style_set_bg_color(&style_screen_btn_start_main, LV_STATE_PRESSED, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_btn_start_main, LV_STATE_PRESSED, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_btn_start_main, LV_STATE_PRESSED, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_start_main, LV_STATE_PRESSED, 0);
	lv_style_set_border_color(&style_screen_btn_start_main, LV_STATE_PRESSED, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn_start_main, LV_STATE_PRESSED, 2);
	lv_style_set_border_opa(&style_screen_btn_start_main, LV_STATE_PRESSED, 255);
	lv_style_set_outline_color(&style_screen_btn_start_main, LV_STATE_PRESSED, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn_start_main, LV_STATE_PRESSED, 0);
	lv_obj_add_style(ui->screen_btn_start, LV_BTN_PART_MAIN, &style_screen_btn_start_main);
	lv_obj_set_pos(ui->screen_btn_start, 40, 310);
	lv_obj_set_size(ui->screen_btn_start, 100, 40);
	ui->screen_btn_start_label = lv_label_create(ui->screen_btn_start, NULL);
	lv_label_set_text(ui->screen_btn_start_label, "START");
	lv_obj_set_style_local_text_color(ui->screen_btn_start_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_obj_set_style_local_text_font(ui->screen_btn_start_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);

	//Write codes screen_btn_stop
	ui->screen_btn_stop = lv_btn_create(ui->screen, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn_stop
	static lv_style_t style_screen_btn_stop_main;
	lv_style_reset(&style_screen_btn_stop_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn_stop_main
	lv_style_set_radius(&style_screen_btn_stop_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_btn_stop_main, LV_STATE_DEFAULT, lv_color_make(0xa4, 0x32, 0x5a));
	lv_style_set_bg_grad_color(&style_screen_btn_stop_main, LV_STATE_DEFAULT, lv_color_make(0xa4, 0x32, 0x5a));
	lv_style_set_bg_grad_dir(&style_screen_btn_stop_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_stop_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn_stop_main, LV_STATE_DEFAULT, lv_color_make(0xb2, 0x2d, 0x01));
	lv_style_set_border_width(&style_screen_btn_stop_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn_stop_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn_stop_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn_stop_main, LV_STATE_DEFAULT, 0);

	//Write style state: LV_STATE_PRESSED for style_screen_btn_stop_main
	lv_style_set_radius(&style_screen_btn_stop_main, LV_STATE_PRESSED, 5);
	lv_style_set_bg_color(&style_screen_btn_stop_main, LV_STATE_PRESSED, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_btn_stop_main, LV_STATE_PRESSED, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_btn_stop_main, LV_STATE_PRESSED, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn_stop_main, LV_STATE_PRESSED, 0);
	lv_style_set_border_color(&style_screen_btn_stop_main, LV_STATE_PRESSED, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn_stop_main, LV_STATE_PRESSED, 2);
	lv_style_set_border_opa(&style_screen_btn_stop_main, LV_STATE_PRESSED, 255);
	lv_style_set_outline_color(&style_screen_btn_stop_main, LV_STATE_PRESSED, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn_stop_main, LV_STATE_PRESSED, 0);
	lv_obj_add_style(ui->screen_btn_stop, LV_BTN_PART_MAIN, &style_screen_btn_stop_main);
	lv_obj_set_pos(ui->screen_btn_stop, 190, 310);
	lv_obj_set_size(ui->screen_btn_stop, 100, 40);
	ui->screen_btn_stop_label = lv_label_create(ui->screen_btn_stop, NULL);
	lv_label_set_text(ui->screen_btn_stop_label, "STOP");
	lv_obj_set_style_local_text_color(ui->screen_btn_stop_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_obj_set_style_local_text_font(ui->screen_btn_stop_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);

	//Write codes screen_label_value
	ui->screen_label_value = lv_label_create(ui->screen, NULL);
	lv_label_set_text(ui->screen_label_value, "Sensor Value = 304 HG");
	lv_label_set_long_mode(ui->screen_label_value, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_label_value, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for screen_label_value
	static lv_style_t style_screen_label_value_main;
	lv_style_reset(&style_screen_label_value_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_value_main
	lv_style_set_radius(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_label_value_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_label_value_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_label_value_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_label_value_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_value_main, LV_STATE_DEFAULT, &lv_font_Montserrat_Medium_16);
	lv_style_set_text_letter_space(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_label_value_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_label_value, LV_LABEL_PART_MAIN, &style_screen_label_value_main);
	lv_obj_set_pos(ui->screen_label_value, 10, 370);
	lv_obj_set_size(ui->screen_label_value, 300, 0);

	//Init events for screen
	events_init_screen(ui);
}