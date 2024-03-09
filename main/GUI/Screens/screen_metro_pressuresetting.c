/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-24
 */

/**
 *  @file screen_metro_pressuresetting.c
 *  @brief This Screen is see the Humidity settings
 *  @details
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_includes.h"

/*********************
 *      DEFINES
 *********************/
#define SYMBOL_SIGNAL "\uf012"

// Declare Fonts here
LV_FONT_DECLARE(signal_20)

// Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void __mpsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event);
static void curve_dropdown_event_handler(lv_obj_t *obj, lv_event_t event);
static void unit_dropdown_event_handler(lv_obj_t *obj, lv_event_t event);
static void lowerlim_dropdown_event_handler(lv_obj_t *obj, lv_event_t event);
static void higherlim_dropdown_event_handler(lv_obj_t *obj, lv_event_t event);
static void __mpsValidAdjBTN_event_handler(lv_obj_t *obj, lv_event_t event);
/**********************
 *  STATIC VARIABLES
 **********************/

lv_obj_t *crnt_screen;
lv_obj_t *scrPressureSetting;
lv_obj_t *mpsParentCont;
lv_obj_t *_mpsContStatusBar;
lv_obj_t *__mpsTimeLabel;
lv_obj_t *__mpsBatteryLabel;
lv_obj_t *__mpsWifiLabel;
lv_obj_t *__mpsSignalLabel;
lv_obj_t *_mpsPressureHeadingCont;
lv_obj_t *__mpsBackArrowLabel;
lv_obj_t *__mpsPressureHeadingLbl;
lv_obj_t *_mpsPressureLogo;
lv_obj_t *_mpsCurveUnitCont;

lv_task_t *pressure_settings_refresherTask;
int global_CurveDegree_pressure;

int metropressureUnit; // for LPH flowUnit == 0, For LPM == 1

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void pressure_settings_refer_func(lv_task_t *refresherTask)
{
    if (lv_obj_get_screen(__mpsTimeLabel) == lv_scr_act())
    {
        lv_label_set_text(__mpsTimeLabel, guiTime);
        lv_label_set_text(__mpsBatteryLabel, get_battery_symbol());
    }
}

void callMetroPressureSettingScreen(void)
{
    // Create Base container
    scrPressureSetting = lv_obj_create(NULL, NULL);
    lv_scr_load(scrPressureSetting);
    if (crnt_screen != NULL)
    {
        lv_obj_del(crnt_screen);
        crnt_screen = NULL;
    }

    mpsParentCont = lv_cont_create(scrPressureSetting, NULL);
    lv_obj_set_size(mpsParentCont, 320, 480);
    lv_obj_set_click(mpsParentCont, false);
    lv_obj_align(mpsParentCont, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(mpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 3989BD
    lv_obj_set_style_local_border_opa(mpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(mpsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create a Satus BAR Container to contain Watch , Signal, wifi & battery status
    _mpsContStatusBar = lv_cont_create(mpsParentCont, NULL);
    lv_obj_set_size(_mpsContStatusBar, 320, 35);
    lv_obj_align(_mpsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_mpsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mpsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    // Create Watch upper left corner
    __mpsTimeLabel = lv_label_create(_mpsContStatusBar, NULL);
    lv_obj_align(__mpsTimeLabel, _mpsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12, 5);
    lv_label_set_text(__mpsTimeLabel, guiTime);

    static lv_style_t _mhsTimeLabelStyle;
    lv_style_init(&_mhsTimeLabelStyle);
    lv_style_set_text_font(&_mhsTimeLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&_mhsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mpsTimeLabel, LV_LABEL_PART_MAIN, &_mhsTimeLabelStyle);

    // Create Label for Battery icon
    __mpsBatteryLabel = lv_label_create(_mpsContStatusBar, NULL);
    lv_obj_align(__mpsBatteryLabel, _mpsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__mpsBatteryLabel, get_battery_symbol());

    static lv_style_t __mpsBatteryLabelStyle;
    lv_style_init(&__mpsBatteryLabelStyle);
    lv_style_set_text_font(&__mpsBatteryLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
    lv_style_set_text_color(&__mpsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mpsBatteryLabel, LV_LABEL_PART_MAIN, &__mpsBatteryLabelStyle);

    // Create Label for Wifi icon
    __mpsWifiLabel = lv_label_create(_mpsContStatusBar, NULL);
    lv_obj_align(__mpsWifiLabel, __mpsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__mpsWifiLabel, LV_SYMBOL_WIFI);
    lv_obj_set_hidden(__mpsWifiLabel, true);

    static lv_style_t __mpsWifiLabelStyle;
    lv_style_init(&__mpsWifiLabelStyle);
    lv_style_set_text_font(&__mpsWifiLabelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&__mpsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mpsWifiLabel, LV_LABEL_PART_MAIN, &__mpsWifiLabelStyle);

    // Create Label for Signal icon
    __mpsSignalLabel = lv_label_create(_mpsContStatusBar, NULL);
    lv_obj_align(__mpsSignalLabel, __mpsWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__mpsSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"
    lv_obj_set_hidden(__mpsSignalLabel, true);

    static lv_style_t __mpsSignalLabelStyle;
    lv_style_init(&__mpsSignalLabelStyle);
    lv_style_set_text_font(&__mpsSignalLabelStyle, LV_STATE_DEFAULT, &signal_20); // signal_20
    lv_style_set_text_color(&__mpsSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mpsSignalLabel, LV_LABEL_PART_MAIN, &__mpsSignalLabelStyle);

    pressure_settings_refresherTask = lv_task_create(pressure_settings_refer_func, 1000, LV_TASK_PRIO_LOW, NULL);

    // Crate a container to contain FLOW Header
    _mpsPressureHeadingCont = lv_cont_create(mpsParentCont, NULL);
    lv_obj_set_size(_mpsPressureHeadingCont, 300, 70);
    lv_obj_align(_mpsPressureHeadingCont, _mpsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_local_bg_color(_mpsPressureHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD)); // 5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_mpsPressureHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    // Create Back arrow img
    __mpsBackArrowLabel = lv_img_create(mpsParentCont, NULL);
    lv_img_set_src(__mpsBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__mpsBackArrowLabel, _mpsPressureHeadingCont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_obj_set_click(__mpsBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__mpsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__mpsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__mpsBackArrowLabel, __mpsBackArrow_event_handler);

    // Create Label for FLOW "Heading"
    __mpsPressureHeadingLbl = lv_label_create(_mpsPressureHeadingCont, NULL);
    lv_obj_align(__mpsPressureHeadingLbl, _mpsPressureHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
    lv_label_set_align(__mpsPressureHeadingLbl, LV_LABEL_ALIGN_CENTER);
    if (screenid == SCR_EXTERNAL_TEMPERATURE_ADJUST)
    {
        lv_label_set_text(__mpsPressureHeadingLbl, "External \nPressure");
        screenid = SCR_EXTERNAL_PRESSURE_SETTINGS;
    }
    else if (screenid == SCR_INTERNAL_TEMPERATURE_ADJUST)
    {
        lv_label_set_text(__mpsPressureHeadingLbl, "Internal \nPressure");
        screenid = SCR_INTERNAL_PRESSURE_SETTINGS;
    }

    static lv_style_t __mpsPressureHeadingLblStyle;
    lv_style_init(&__mpsPressureHeadingLblStyle);
    lv_style_set_text_font(&__mpsPressureHeadingLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_22); // signal_20
    lv_style_set_text_color(&__mpsPressureHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mpsPressureHeadingLbl, LV_LABEL_PART_MAIN, &__mpsPressureHeadingLblStyle);

    // //Create FAN Logo
    // _mpsPressureLogo = lv_img_create(mpsParentCont, NULL);
    // lv_img_set_src(_mpsPressureLogo, &fan_icon);
    // lv_obj_align(_mpsPressureLogo, mpsParentCont, LV_ALIGN_IN_TOP_RIGHT, -25 , 55);
    // lv_img_set_auto_size(_mpsPressureLogo, true);

    //===============================================================
    //===============================================================

    _mpsCurveUnitCont = lv_cont_create(mpsParentCont, NULL);
    lv_obj_set_size(_mpsCurveUnitCont, 300, 110);
    lv_obj_align(_mpsCurveUnitCont, _mpsPressureHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(_mpsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mpsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    static lv_style_t _mpsBlueTxtStyle;
    lv_style_init(&_mpsBlueTxtStyle);
    lv_style_set_text_font(&_mpsBlueTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_mpsBlueTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));

    lv_obj_t *_mpsCurveTxt;
    _mpsCurveTxt = lv_label_create(_mpsCurveUnitCont, NULL);
    lv_obj_align(_mpsCurveTxt, _mpsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, -25);
    lv_label_set_text(_mpsCurveTxt, "CURVE");
    lv_obj_add_style(_mpsCurveTxt, LV_LABEL_PART_MAIN, &_mpsBlueTxtStyle);

    lv_obj_t *_mpsUnitTxt;
    _mpsUnitTxt = lv_label_create(_mpsCurveUnitCont, NULL);
    lv_obj_align(_mpsUnitTxt, _mpsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, 15);
    lv_label_set_text(_mpsUnitTxt, "UNIT");
    lv_obj_add_style(_mpsUnitTxt, LV_LABEL_PART_MAIN, &_mpsBlueTxtStyle);

    static lv_style_t _mpsDropDownStylemain;
    lv_style_reset(&_mpsDropDownStylemain);
    lv_style_set_bg_color(&_mpsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mpsDropDownStylemain, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mpsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mpsDropDownStylemain, LV_STATE_DEFAULT, 0);

    static lv_style_t _mpsDropDownStylelist;
    lv_style_reset(&_mpsDropDownStylelist);
    lv_style_set_bg_color(&_mpsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&_mpsDropDownStylelist, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mpsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mpsDropDownStylelist, LV_STATE_DEFAULT, 0);

    static lv_style_t _mpsDropDownStyleselected;
    lv_style_reset(&_mpsDropDownStyleselected);
    lv_style_set_bg_color(&_mpsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35, 0x9F, 0xE2));
    lv_style_set_text_font(&_mpsDropDownStyleselected, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mpsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mpsDropDownStyleselected, LV_STATE_DEFAULT, 0);

    // Create a Curve selection drop down list
    lv_obj_t *_mpsCurveDropDown = lv_dropdown_create(_mpsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mpsCurveDropDown,
                            "Linear4\n"
                            "Linear5\n"
                            "Linear6\n"
                            "Linear7\n"
                            "Linear8");

    lv_obj_align(_mpsCurveDropDown, _mpsCurveTxt, LV_ALIGN_OUT_RIGHT_TOP, 100, -2);
    lv_obj_set_size(_mpsCurveDropDown, 120, 30);
    lv_obj_add_style(_mpsCurveDropDown, LV_DROPDOWN_PART_MAIN, &_mpsDropDownStylemain);
    lv_obj_add_style(_mpsCurveDropDown, LV_DROPDOWN_PART_LIST, &_mpsDropDownStylelist);
    lv_obj_add_style(_mpsCurveDropDown, LV_DROPDOWN_PART_SELECTED, &_mpsDropDownStyleselected);
    lv_obj_set_event_cb(_mpsCurveDropDown, curve_dropdown_event_handler);

    // Create a Unit selection drop down list
    lv_obj_t *_mpsUnitDropDown = lv_dropdown_create(_mpsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mpsUnitDropDown, "hPA");

    lv_obj_align(_mpsUnitDropDown, _mpsUnitTxt, LV_ALIGN_OUT_RIGHT_TOP, 117, -2);
    lv_obj_set_size(_mpsUnitDropDown, 120, 30);
    lv_obj_add_style(_mpsUnitDropDown, LV_DROPDOWN_PART_MAIN, &_mpsDropDownStylemain);
    lv_obj_add_style(_mpsUnitDropDown, LV_DROPDOWN_PART_LIST, &_mpsDropDownStylelist);
    lv_obj_add_style(_mpsUnitDropDown, LV_DROPDOWN_PART_SELECTED, &_mpsDropDownStyleselected);
    lv_obj_set_event_cb(_mpsUnitDropDown, unit_dropdown_event_handler);

    lv_obj_t *_mpsAlarmStPtCont;
    _mpsAlarmStPtCont = lv_cont_create(mpsParentCont, NULL);
    lv_obj_set_size(_mpsAlarmStPtCont, 300, 180);
    lv_obj_align(_mpsAlarmStPtCont, _mpsCurveUnitCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_local_bg_color(_mpsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D)); // LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mpsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN);

    static lv_style_t _mpsBigTxtStyle;
    lv_style_init(&_mpsBigTxtStyle);
    lv_style_set_text_font(&_mpsBigTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14); // signal_20
    lv_style_set_text_color(&_mpsBigTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    static lv_style_t _mpsSmallTxtStyle;
    lv_style_init(&_mpsSmallTxtStyle);
    lv_style_set_text_font(&_mpsSmallTxtStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12); // signal_20
    lv_style_set_text_color(&_mpsSmallTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    lv_obj_t *_mpsAlarmSetPointTxt;
    _mpsAlarmSetPointTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsAlarmSetPointTxt, _mpsAlarmStPtCont, LV_ALIGN_IN_TOP_LEFT, 10, 8);
    lv_label_set_text(_mpsAlarmSetPointTxt, "ALERT FROM SET POINT");
    lv_obj_add_style(_mpsAlarmSetPointTxt, LV_LABEL_PART_MAIN, &_mpsBigTxtStyle);

    lv_obj_t *_mpsOnOffTxt;
    _mpsOnOffTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsOnOffTxt, _mpsAlarmSetPointTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mpsOnOffTxt, "ON / OFF");
    lv_obj_add_style(_mpsOnOffTxt, LV_LABEL_PART_MAIN, &_mpsSmallTxtStyle);

    // Write style state: LV_STATE_DEFAULT for style_screen_sw_1_bg
    static lv_style_t style_switch_bg;
    lv_style_reset(&style_switch_bg);
    lv_style_set_radius(&style_switch_bg, LV_STATE_DEFAULT, 20);
    lv_style_set_bg_color(&style_switch_bg, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_grad_color(&style_switch_bg, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_bg_grad_dir(&style_switch_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_switch_bg, LV_STATE_DEFAULT, 255);
    lv_style_set_border_opa(&style_switch_bg, LV_STATE_DEFAULT, 255);
    lv_style_set_border_color(&style_switch_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_switch_bg, LV_STATE_DEFAULT, 2);
    lv_style_set_outline_width(&style_switch_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_switch_bg, LV_STATE_FOCUSED, 0);
    lv_style_set_border_opa(&style_switch_bg, LV_STATE_CHECKED, 255);
    lv_style_set_border_color(&style_switch_bg, LV_STATE_CHECKED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_switch_bg, LV_STATE_CHECKED, 2);
    lv_style_set_outline_width(&style_switch_bg, LV_STATE_CHECKED, 0);

    // Write style LV_SWITCH_PART_INDIC for screen_sw_1
    static lv_style_t style_switch_indic;
    lv_style_reset(&style_switch_indic);

    // Write style state: LV_STATE_DEFAULT for style_screen_sw_1_indic
    lv_style_set_radius(&style_switch_indic, LV_STATE_DEFAULT, 20);
    lv_style_set_bg_color(&style_switch_indic, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_grad_color(&style_switch_indic, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_grad_dir(&style_switch_indic, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_switch_indic, LV_STATE_DEFAULT, 255);
    lv_style_set_bg_color(&style_switch_indic, LV_STATE_CHECKED, LV_COLOR_GREEN);
    lv_style_set_bg_grad_color(&style_switch_indic, LV_STATE_CHECKED, LV_COLOR_GREEN);
    lv_style_set_bg_grad_dir(&style_switch_indic, LV_STATE_CHECKED, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_switch_indic, LV_STATE_CHECKED, 255);
    lv_style_set_border_opa(&style_switch_indic, LV_STATE_DEFAULT, 255);
    lv_style_set_border_color(&style_switch_indic, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_switch_indic, LV_STATE_DEFAULT, 2);
    lv_style_set_outline_width(&style_switch_indic, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_switch_indic, LV_STATE_FOCUSED, 0);
    lv_style_set_border_opa(&style_switch_indic, LV_STATE_CHECKED, 255);
    lv_style_set_border_color(&style_switch_indic, LV_STATE_CHECKED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_switch_indic, LV_STATE_CHECKED, 2);
    lv_style_set_outline_width(&style_switch_indic, LV_STATE_CHECKED, 0);

    // Write style LV_SWITCH_PART_KNOB for screen_sw_1
    static lv_style_t style_switch_knob;
    lv_style_reset(&style_switch_knob);

    // Write style state: LV_STATE_DEFAULT for style_screen_sw_1_knob
    // lv_style_set_radius(&style_switch_knob, LV_STATE_DEFAULT, 100);
    lv_style_set_bg_color(&style_switch_knob, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_grad_color(&style_switch_knob, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_grad_dir(&style_switch_knob, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_switch_knob, LV_STATE_DEFAULT, 255);

    // Create On / OFF control switch
    lv_obj_t *_mpsOnOffSwitch = lv_switch_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsOnOffSwitch, _mpsOnOffTxt, LV_ALIGN_OUT_RIGHT_TOP, 155, -15);
    lv_obj_add_style(_mpsOnOffSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mpsOnOffSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mpsOnOffSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mpsOnOffSwitch, 25);
    lv_obj_set_width(_mpsOnOffSwitch, 60);
    // lv_obj_set_event_cb(_mpsOnOffSwitch, Buzzer_switch_event_handler);

    lv_obj_t *_mpsLowerLimitTxt;
    _mpsLowerLimitTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsLowerLimitTxt, _mpsOnOffTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mpsLowerLimitTxt, "LOWER LIMIT");
    lv_obj_add_style(_mpsLowerLimitTxt, LV_LABEL_PART_MAIN, &_mpsBlueTxtStyle);

    // Create a Curve selection drop down list
    lv_obj_t *_mpsLowerLimDropDown = lv_dropdown_create(_mpsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mpsLowerLimDropDown, "-300\n"
                                                  "-400");
    lv_obj_align(_mpsLowerLimDropDown, _mpsLowerLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mpsLowerLimDropDown, 120, 30);
    lv_obj_add_style(_mpsLowerLimDropDown, LV_DROPDOWN_PART_MAIN, &_mpsDropDownStylemain);
    lv_obj_add_style(_mpsLowerLimDropDown, LV_DROPDOWN_PART_LIST, &_mpsDropDownStylelist);
    lv_obj_add_style(_mpsLowerLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mpsDropDownStyleselected);
    lv_obj_set_event_cb(_mpsLowerLimDropDown, lowerlim_dropdown_event_handler);

    lv_obj_t *_mpsHigherLimitTxt;
    _mpsHigherLimitTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsHigherLimitTxt, _mpsLowerLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(_mpsHigherLimitTxt, "HIGHER LIMIT");
    lv_obj_add_style(_mpsHigherLimitTxt, LV_LABEL_PART_MAIN, &_mpsBlueTxtStyle);

    // Create a Curve selection drop down list
    lv_obj_t *_mpsHigherLimDropDown = lv_dropdown_create(_mpsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mpsHigherLimDropDown, "700\n"
                                                   "800");
    lv_obj_align(_mpsHigherLimDropDown, _mpsHigherLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mpsHigherLimDropDown, 120, 30);
    lv_obj_add_style(_mpsHigherLimDropDown, LV_DROPDOWN_PART_MAIN, &_mpsDropDownStylemain);
    lv_obj_add_style(_mpsHigherLimDropDown, LV_DROPDOWN_PART_LIST, &_mpsDropDownStylelist);
    lv_obj_add_style(_mpsHigherLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mpsDropDownStyleselected);
    lv_obj_set_event_cb(_mpsHigherLimDropDown, higherlim_dropdown_event_handler);

    lv_obj_t *_mpsActionTxt;
    _mpsActionTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsActionTxt, _mpsHigherLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_mpsActionTxt, "ACTION");
    lv_obj_add_style(_mpsActionTxt, LV_LABEL_PART_MAIN, &_mpsBigTxtStyle);

    lv_obj_t *_mpsDisableDeviceTxt;
    _mpsDisableDeviceTxt = lv_label_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsDisableDeviceTxt, _mpsActionTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_mpsDisableDeviceTxt, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_mpsDisableDeviceTxt, LV_LABEL_PART_MAIN, &_mpsSmallTxtStyle);

    // Create On / OFF control switch
    lv_obj_t *_mpsActionSwitch = lv_switch_create(_mpsAlarmStPtCont, NULL);
    lv_obj_align(_mpsActionSwitch, _mpsActionTxt, LV_ALIGN_OUT_RIGHT_TOP, 160, 10);
    lv_obj_add_style(_mpsActionSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mpsActionSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mpsActionSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mpsActionSwitch, 25);
    lv_obj_set_width(_mpsActionSwitch, 60);
    // lv_obj_set_event_cb(_mpsOnOffSwitch, Buzzer_switch_event_handler);

    static lv_style_t _mpsValidBtnLblStyle;
    lv_style_init(&_mpsValidBtnLblStyle);
    lv_style_set_text_font(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_border_width(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&_mpsValidBtnLblStyle, LV_STATE_DEFAULT, 5);

    static lv_style_t _mpsValidBtnLblStylepressed;
    lv_style_init(&_mpsValidBtnLblStylepressed);
    lv_style_set_text_font(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, &lv_font_montserrat_16);
    lv_style_set_border_width(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_BLUE);
    lv_style_set_text_color(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, 0);
    lv_style_set_radius(&_mpsValidBtnLblStylepressed, LV_STATE_PRESSED, 5);

    static lv_style_t _mpsValidBtnLblStylefocused;
    lv_style_init(&_mpsValidBtnLblStylefocused);
    lv_style_set_text_font(&_mpsValidBtnLblStylefocused, LV_STATE_FOCUSED, &lv_font_montserrat_16);
    lv_style_set_border_width(&_mpsValidBtnLblStylefocused, LV_STATE_FOCUSED, 0);
    lv_style_set_text_color(&_mpsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mpsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D, 0xAF, 0x48));
    lv_style_set_outline_width(&_mpsValidBtnLblStylefocused, LV_STATE_FOCUSED, 0);
    lv_style_set_radius(&_mpsValidBtnLblStylefocused, LV_STATE_PRESSED, 5);

    // Creat a stop Button
    lv_obj_t *_mpsValidBtn;
    _mpsValidBtn = lv_btn_create(mpsParentCont, NULL);
    lv_obj_align(_mpsValidBtn, mpsParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_size(_mpsValidBtn, 300, 44);
    lv_obj_add_style(_mpsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStyle);
    lv_obj_add_style(_mpsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStylepressed);
    lv_obj_add_style(_mpsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStylefocused);
    lv_obj_set_event_cb(_mpsValidBtn, __mpsValidAdjBTN_event_handler);

    // Creat a stop Button Label
    lv_obj_t *_mpsValidTxt;
    _mpsValidTxt = lv_label_create(_mpsValidBtn, NULL);
    lv_obj_align(_mpsValidTxt, _mpsValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_mpsValidTxt, "VALID & ADJUST");

    crnt_screen = scrPressureSetting;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void __mpsValidAdjBTN_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(pressure_settings_refresherTask);
        pressure_settings_refresherTask = NULL;
        callMetroAdjust();
    }
}

static void __mpsBackArrow_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_task_del(pressure_settings_refresherTask);
        pressure_settings_refresherTask = NULL;
        CallMetroMenuScreen();
    }
}

static void curve_dropdown_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if (strcmp(buf, "Linear1") == 0)
        {
            global_CurveDegree_pressure = 1;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear2") == 0)
        {
            global_CurveDegree_pressure = 2;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear3") == 0)
        {
            global_CurveDegree_pressure = 3;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear4") == 0)
        {
            global_CurveDegree_pressure = 4;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear5") == 0)
        {
            global_CurveDegree_pressure = 5;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear6") == 0)
        {
            global_CurveDegree_pressure = 6;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear7") == 0)
        {
            global_CurveDegree_pressure = 7;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear8") == 0)
        {
            global_CurveDegree_pressure = 8;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else if (strcmp(buf, "Linear9") == 0)
        {
            global_CurveDegree_pressure = 9;
            printf("Curve Points: %d\n", global_CurveDegree_pressure);
        }
        else
        {
        }
    }
}

static void unit_dropdown_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if (!strcmp(buf, "hPA"))
        {
            metropressureUnit = 0;
            printf("Pressure Unit selected is hPA = %d\n", metropressureUnit);
        }
    }
}

static void lowerlim_dropdown_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
    }
}

static void higherlim_dropdown_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
    }
}

/**********************
 *    ERROR ASSERT
 **********************/
