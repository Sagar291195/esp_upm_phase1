/*********************
 *      INCLUDES
 *********************/

#include "screen_includes.h"


/*********************
 *      DEFINES
 *********************/
#define TAG             "SET TIMEDATE"
#define SYMBOL_SIGNAL "\uf012"
/**********************
 *      TYPEDEFS
 **********************/
LV_FONT_DECLARE(signal_20);
/**********************
 *  STATIC PROTOTYPES
 **********************/

static void dayRoller_event_handler(lv_obj_t *obj, lv_event_t event);
static void monthRoller_event_handler(lv_obj_t *obj, lv_event_t event);
static void yearRoller_event_handler(lv_obj_t *obj, lv_event_t event);

static void HourRoller_event_handler(lv_obj_t *obj, lv_event_t event);
static void MinRoller_event_handler(lv_obj_t *obj, lv_event_t event);
static void SecRoller_event_handler(lv_obj_t *obj, lv_event_t event);

static void timeSaveBTN_event_handler(lv_obj_t *obj, lv_event_t event);
static void _pidBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
LV_IMG_DECLARE(left_arrow_icon);

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
char day_Roller_Val[5];
char month_Roller_Val[5];
char year_Roller_Val[5];

char hour_Roller_Val[5];
char min_Roller_Val[5];
char sec_Roller_Val[5];

char rollerNullVal[5] = "";

char day_Roller_Default[5] = "01";
char month_Roller_Default[5] = "01";
char year_Roller_Default[5] = "2023";

char hour_Roller_Default[5] = "00";
char min_Roller_Default[5] = "00";
char sec_Roller_Default[5] = "00";
static struct tm set_time_data;
lv_obj_t *crnt_screen;
lv_obj_t *set_timedate_battery_icon;

lv_task_t *settimedate_refresherTask;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void settimedate_refer_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(set_timedate_battery_icon) == lv_scr_act())
    {
        lv_label_set_text(set_timedate_battery_icon, get_battery_symbol());
    }
}

void screen_set_time_date(void)
{
    lv_obj_t *obj_screen_set_timedate;
    obj_screen_set_timedate = lv_obj_create(NULL, NULL);
    lv_scr_load(obj_screen_set_timedate);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

	static lv_style_t style_screen_main;
	lv_style_reset(&style_screen_main);
	lv_style_set_bg_color(&style_screen_main, LV_STATE_DEFAULT, lv_color_make(0x39, 0x89, 0xbd));
	lv_style_set_bg_opa(&style_screen_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(obj_screen_set_timedate, LV_OBJ_PART_MAIN, &style_screen_main);

    lv_obj_t *set_timedate_header;
    set_timedate_header = lv_cont_create(obj_screen_set_timedate, NULL);
    lv_obj_set_size(set_timedate_header, 320, 35);
    lv_obj_align(set_timedate_header, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(set_timedate_header, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(set_timedate_header, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Label for Battery icon
    set_timedate_battery_icon = lv_label_create(set_timedate_header, NULL);
    lv_obj_align(set_timedate_battery_icon, set_timedate_header, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(set_timedate_battery_icon, get_battery_symbol());

    static lv_style_t style_battery_label;
    lv_style_init(&style_battery_label);
    lv_style_set_text_font(&style_battery_label, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&style_battery_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(set_timedate_battery_icon, LV_LABEL_PART_MAIN, &style_battery_label);

    // Create Label for Wifi icon
    lv_obj_t *set_timedate_wifi_icon;
    set_timedate_wifi_icon = lv_label_create(set_timedate_header, NULL);
    lv_obj_align(set_timedate_wifi_icon, set_timedate_battery_icon, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(set_timedate_wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(set_timedate_wifi_icon, true);

    static lv_style_t style_wifi_label;
    lv_style_init(&style_wifi_label);
    lv_style_set_text_font(&style_wifi_label, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&style_wifi_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(set_timedate_wifi_icon, LV_LABEL_PART_MAIN, &style_wifi_label);

    // Create Label for Signal icon
    lv_obj_t *set_timedate_signal_icon;
    set_timedate_signal_icon = lv_label_create(set_timedate_header, NULL);
    lv_obj_align(set_timedate_signal_icon, set_timedate_wifi_icon, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(set_timedate_signal_icon, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(set_timedate_signal_icon, true);

    static lv_style_t style_signal_label;
    lv_style_init(&style_signal_label);
    lv_style_set_text_font(&style_signal_label, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&style_signal_label, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(set_timedate_signal_icon, LV_LABEL_PART_MAIN, &style_signal_label);

    settimedate_refresherTask = lv_task_create(settimedate_refer_func, 1000, LV_TASK_PRIO_LOW, NULL);
    // Black arrow Container
    lv_obj_t *set_timeddate_back;
    set_timeddate_back = lv_cont_create(set_timedate_header, NULL);
    lv_obj_set_size(set_timeddate_back, 50, 50);
    lv_obj_align(set_timeddate_back, set_timedate_header, LV_ALIGN_IN_TOP_LEFT, 10, 0);
    lv_obj_set_style_local_bg_color(set_timeddate_back, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(set_timeddate_back, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(set_timeddate_back, _pidBackArrow_event_handler);

    // Create Back arrow img
    lv_obj_t *back_arrow;
    back_arrow = lv_img_create(obj_screen_set_timedate, NULL);
    lv_img_set_src(back_arrow, &left_arrow_icon);
    lv_obj_align(back_arrow, set_timeddate_back, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_click(back_arrow, true);
    lv_obj_set_style_local_image_recolor_opa(back_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(back_arrow, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(back_arrow, _pidBackArrow_event_handler);

   
    lv_obj_t *screen_label;
    screen_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_label_set_text(screen_label, "Set Time & Date");
	lv_label_set_long_mode(screen_label, LV_LABEL_LONG_BREAK);
	lv_label_set_align(screen_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_pos(screen_label, 0, 40);
    lv_obj_set_size(screen_label, 320, 0);
    lv_obj_set_style_local_text_font(screen_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(screen_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    lv_obj_t *date_roller;
    date_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(date_roller,     "01\n"
                                           "02\n"
                                           "03\n"
                                           "04\n"
                                           "05\n"
                                           "06\n"
                                           "07\n"
                                           "08\n"
                                           "09\n"
                                           "10\n"
                                           "11\n"
                                           "12\n"
                                           "13\n"
                                           "14\n"
                                           "15\n"
                                           "16\n"
                                           "17\n"
                                           "18\n"
                                           "19\n"
                                           "20\n"
                                           "21\n"
                                           "22\n"
                                           "23\n"
                                           "24\n"
                                           "25\n"
                                           "26\n"
                                           "27\n"
                                           "28\n"
                                           "29\n"
                                           "30\n"
                                           "31\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(date_roller, 3);
    lv_obj_align(date_roller, set_timeddate_back, LV_ALIGN_OUT_BOTTOM_LEFT, 30, 30);
    lv_obj_set_click(date_roller, false);
    lv_obj_set_style_local_bg_color(date_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(date_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(date_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(date_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(date_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(date_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(date_roller, dayRoller_event_handler);

    lv_obj_t *date_text_label;
    date_text_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(date_text_label, date_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 12, 5);
    lv_label_set_text(date_text_label, "DD");
    lv_obj_set_style_local_text_font(date_text_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(date_text_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *month_roller;
    month_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(month_roller, "01\n"
                                           "02\n"
                                           "03\n"
                                           "04\n"
                                           "05\n"
                                           "06\n"
                                           "07\n"
                                           "08\n"
                                           "09\n"
                                           "10\n"
                                           "11\n"
                                           "12\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(month_roller, 3);
    lv_obj_align(month_roller, date_roller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_click(month_roller, false);
    lv_obj_set_style_local_bg_color(month_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(month_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(month_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(month_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(month_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(month_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(month_roller, monthRoller_event_handler);

    lv_obj_t *month_text_label;
    month_text_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(month_text_label, month_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 12, 5);
    lv_label_set_text(month_text_label, "MM");
    lv_obj_set_style_local_text_font(month_text_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(month_text_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *year_roller;
    year_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(year_roller,       "2023\n"
                                             "2024\n"
                                             "2025\n"
                                             "2026\n"
                                             "2027\n"
                                             "2028\n"
                                             "2029\n"
                                             "2030\n"
                                             "2031\n"
                                             "2032\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(year_roller, 3);
    lv_obj_align(year_roller, month_roller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_click(year_roller, false);
    lv_obj_set_style_local_bg_color(year_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(year_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(year_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(year_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(year_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(year_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(year_roller, yearRoller_event_handler);

    lv_obj_t *year_txt_label;
    year_txt_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(year_txt_label, year_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 15, 5);
    lv_label_set_text(year_txt_label, "YYYY");
    lv_obj_set_style_local_text_font(year_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(year_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *hour_roller;
    hour_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(hour_roller, "00\n"
                                         "01\n"
                                         "02\n"
                                         "03\n"
                                         "04\n"
                                         "05\n"
                                         "06\n"
                                         "07\n"
                                         "08\n"
                                         "09\n"
                                         "10\n"
                                         "11\n"
                                         "12\n"
                                         "13\n"
                                         "14\n"
                                         "15\n"
                                         "16\n"
                                         "17\n"
                                         "18\n"
                                         "19\n"
                                         "20\n"
                                         "21\n"
                                         "22\n"
                                         "23\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(hour_roller, 3);
    lv_obj_align(hour_roller, date_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 40);
    lv_obj_set_click(hour_roller, false);
    lv_obj_set_style_local_bg_color(hour_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(hour_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(hour_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(hour_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(hour_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(hour_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(hour_roller, HourRoller_event_handler);

    lv_obj_t *hour_txt_label;
    hour_txt_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(hour_txt_label, hour_roller, LV_ALIGN_OUT_BOTTOM_LEFT, -5, 5);
    lv_label_set_text(hour_txt_label, "HOUR");
    lv_obj_set_style_local_text_font(hour_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(hour_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *minute_roller;
    minute_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(minute_roller, "00\n"
                                           "01\n"
                                           "02\n"
                                           "03\n"
                                           "04\n"
                                           "05\n"
                                           "06\n"
                                           "07\n"
                                           "08\n"
                                           "09\n"
                                           "10\n"
                                           "11\n"
                                           "12\n"
                                           "13\n"
                                           "14\n"
                                           "15\n"
                                           "16\n"
                                           "17\n"
                                           "18\n"
                                           "19\n"
                                           "20\n"
                                           "21\n"
                                           "22\n"
                                           "23\n"
                                           "24\n"
                                           "25\n"
                                           "26\n"
                                           "27\n"
                                           "28\n"
                                           "29\n"
                                           "30\n"
                                           "31\n"
                                           "32\n"
                                           "33\n"
                                           "34\n"
                                           "35\n"
                                           "36\n"
                                           "37\n"
                                           "38\n"
                                           "39\n"
                                           "40\n"
                                           "41\n"
                                           "42\n"
                                           "43\n"
                                           "44\n"
                                           "45\n"
                                           "46\n"
                                           "47\n"
                                           "48\n"
                                           "49\n"
                                           "50\n"
                                           "51\n"
                                           "52\n"
                                           "53\n"
                                           "54\n"
                                           "55\n"
                                           "56\n"
                                           "57\n"
                                           "58\n"
                                           "59\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(minute_roller, 3);
    lv_obj_align(minute_roller, hour_roller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_click(minute_roller, false);
    lv_obj_set_style_local_bg_color(minute_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(minute_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(minute_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(minute_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(minute_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(minute_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(minute_roller, MinRoller_event_handler);

    lv_obj_t *minute_txt_label;
    minute_txt_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(minute_txt_label, minute_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 5);
    lv_label_set_text(minute_txt_label, "MIN");
    lv_obj_set_style_local_text_font(minute_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(minute_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t *second_roller;
    second_roller = lv_roller_create(obj_screen_set_timedate, NULL);
    lv_roller_set_options(second_roller, "00\n"
                                           "01\n"
                                           "02\n"
                                           "03\n"
                                           "04\n"
                                           "05\n"
                                           "06\n"
                                           "07\n"
                                           "08\n"
                                           "09\n"
                                           "10\n"
                                           "11\n"
                                           "12\n"
                                           "13\n"
                                           "14\n"
                                           "15\n"
                                           "16\n"
                                           "17\n"
                                           "18\n"
                                           "19\n"
                                           "20\n"
                                           "21\n"
                                           "22\n"
                                           "23\n"
                                           "24\n"
                                           "25\n"
                                           "26\n"
                                           "27\n"
                                           "28\n"
                                           "29\n"
                                           "30\n"
                                           "31\n"
                                           "32\n"
                                           "33\n"
                                           "34\n"
                                           "35\n"
                                           "36\n"
                                           "37\n"
                                           "38\n"
                                           "39\n"
                                           "40\n"
                                           "41\n"
                                           "42\n"
                                           "43\n"
                                           "44\n"
                                           "45\n"
                                           "46\n"
                                           "47\n"
                                           "48\n"
                                           "49\n"
                                           "50\n"
                                           "51\n"
                                           "52\n"
                                           "53\n"
                                           "54\n"
                                           "55\n"
                                           "56\n"
                                           "57\n"
                                           "58\n"
                                           "59\n",
                          LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(second_roller, 3);
    lv_obj_align(second_roller, minute_roller, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_click(second_roller, false);
    lv_obj_set_style_local_bg_color(second_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_obj_set_style_local_bg_color(second_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D));
    lv_obj_set_style_local_border_width(second_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(second_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, &lv_font_montserrat_22); // 22
    lv_obj_set_style_local_text_color(second_roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xAC, 0xAC, 0xAC));
    lv_obj_set_style_local_text_color(second_roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(second_roller, SecRoller_event_handler);

    lv_obj_t *second_txt_label;
    second_txt_label = lv_label_create(obj_screen_set_timedate, NULL);
    lv_obj_align(second_txt_label, second_roller, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 5);
    lv_label_set_text(second_txt_label, "SEC");
    lv_obj_set_style_local_text_font(second_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_obj_set_style_local_text_color(second_txt_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    
    static lv_style_t style_btn_default;
    lv_style_init(&style_btn_default);
    lv_style_set_text_font(&style_btn_default, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_border_width(&style_btn_default, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&style_btn_default, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_btn_default, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&style_btn_default, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_btn_default, LV_STATE_DEFAULT, 5);

    static lv_style_t style_btn_pressed;
    lv_style_init(&style_btn_pressed);
    lv_style_set_text_font(&style_btn_pressed, LV_STATE_PRESSED, &lv_font_montserrat_16);
    lv_style_set_border_width(&style_btn_pressed, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&style_btn_pressed, LV_STATE_PRESSED, LV_COLOR_BLUE);
    lv_style_set_text_color(&style_btn_pressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_btn_pressed, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&style_btn_pressed, LV_STATE_PRESSED, 0);
    lv_style_set_radius(&style_btn_pressed, LV_STATE_PRESSED, 5);

    static lv_style_t style_btn_focused;
    lv_style_init(&style_btn_focused);
    lv_style_set_text_font(&style_btn_focused, LV_STATE_FOCUSED, &lv_font_montserrat_16);
    lv_style_set_border_width(&style_btn_focused, LV_STATE_FOCUSED, 0);
    lv_style_set_text_color(&style_btn_focused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&style_btn_focused, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&style_btn_focused, LV_STATE_FOCUSED, 0);
    lv_style_set_radius(&style_btn_focused, LV_STATE_PRESSED, 5);

    lv_obj_t *set_time_btn;
    set_time_btn = lv_btn_create(obj_screen_set_timedate, NULL);
    lv_obj_align(set_time_btn, obj_screen_set_timedate, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);
    lv_obj_add_style(set_time_btn, LV_BTN_PART_MAIN, &style_btn_default);
    lv_obj_add_style(set_time_btn, LV_BTN_PART_MAIN, &style_btn_pressed);
    lv_obj_add_style(set_time_btn, LV_BTN_PART_MAIN, &style_btn_focused);
    lv_obj_set_size(set_time_btn, 260, 44);
    lv_obj_set_event_cb(set_time_btn, timeSaveBTN_event_handler);

    lv_obj_t *time_save_label;
    time_save_label = lv_label_create(set_time_btn, NULL);
    lv_obj_align(time_save_label, set_time_btn, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_label_set_text(time_save_label, "UPDATE TIME");
    lv_obj_set_style_local_text_font(time_save_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_obj_set_style_local_text_color(time_save_label, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    crnt_screen = obj_screen_set_timedate;
    screenid = SCR_SET_TIMEDATE;
 }

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void dayRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, day_Roller_Val, sizeof(day_Roller_Val));
    }
}

static void monthRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, month_Roller_Val, sizeof(month_Roller_Val));
    }
}

static void yearRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, year_Roller_Val, sizeof(year_Roller_Val));
    }
}

static void HourRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, hour_Roller_Val, sizeof(hour_Roller_Val));
    }
}

static void MinRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, min_Roller_Val, sizeof(min_Roller_Val));
    }
}

static void SecRoller_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_roller_get_selected_str(obj, sec_Roller_Val, sizeof(sec_Roller_Val));
    }
}

static void _pidBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(settimedate_refresherTask);
        settimedate_refresherTask = NULL;
        pxDashboardScreen();
    }
}

static void timeSaveBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        if (!strcmp(day_Roller_Val, rollerNullVal))
        {
            sprintf(day_Roller_Val, day_Roller_Default);
        }
        if (!strcmp(month_Roller_Val, rollerNullVal))
        {
            sprintf(month_Roller_Val, month_Roller_Default);
        }
        if (!strcmp(year_Roller_Val, rollerNullVal))
        {
            sprintf(year_Roller_Val, year_Roller_Default);
        }

        if (!strcmp(hour_Roller_Val, rollerNullVal))
        {
            sprintf(hour_Roller_Val, hour_Roller_Default);
        }
        if (!strcmp(min_Roller_Val, rollerNullVal))
        {
            sprintf(min_Roller_Val, min_Roller_Default);
        }
        if (!strcmp(sec_Roller_Val, rollerNullVal))
        {
            sprintf(sec_Roller_Val, sec_Roller_Default);
        }

        printf("Set Day Value is   = %s\n", day_Roller_Val);
        printf("Set Month Value is = %s\n", month_Roller_Val);
        printf("Set Year Value is  = %s\n", year_Roller_Val);

        printf("Set Hour Value is   = %s\n", hour_Roller_Val);
        printf("Set Minute Value is = %s\n", min_Roller_Val);
        printf("Set Secons Value is = %s\n", sec_Roller_Val);

        set_time_data.tm_year = atoi(year_Roller_Val) - 1900;   
        set_time_data.tm_mon = atoi(month_Roller_Val) - 1;  
        set_time_data.tm_mday = atoi(day_Roller_Val);
        set_time_data.tm_hour = atoi(hour_Roller_Val);
        set_time_data.tm_min = atoi(min_Roller_Val);
        set_time_data.tm_sec = atoi(sec_Roller_Val);

        set_navier_time_flag(true);
    }
}


void get_new_set_time(struct tm *time)
{
    memcpy(time, &set_time_data, sizeof(set_time_data));
}
/**********************
 *    ERROR ASSERT
 **********************/
