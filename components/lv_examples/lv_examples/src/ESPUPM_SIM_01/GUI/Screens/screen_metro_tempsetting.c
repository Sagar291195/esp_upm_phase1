/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-24
 */

/**
*  @file screen_metro_tempsetting.c
*  @brief This Screen is see the Temperature settings
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

//Declare Fonts here
LV_FONT_DECLARE(signal_20)

//Declare Images Here

LV_IMG_DECLARE(left_arrow_icon)	


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void  __mtsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  unit_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  lowerlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  higherlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  __mtsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event);
/**********************
 *  STATIC VARIABLES
 **********************/


lv_obj_t *crnt_screen;
lv_obj_t * scrTempSetting;
lv_obj_t * mtsParentCont;
lv_obj_t * _mtsContStatusBar;
lv_obj_t * __mtsTimeLabel;
lv_obj_t * __mtsBatteryLabel;
lv_obj_t * __mtsWifiLabel;
lv_obj_t * __mtsSignalLabel;
lv_obj_t * _mtsTempHeadingCont;
lv_obj_t * __mtsBackArrowLabel;
lv_obj_t * __mtsTempHeadingLbl;
lv_obj_t * _mtsTempLogo;
lv_obj_t * _mtsCurveUnitCont;


int global_CurveDegree_temp;
int metroTempUnit;    

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void callMetroTempSettingScreen(void)
{
    //Create Base container
    scrTempSetting = lv_cont_create(NULL, NULL);
    lv_scr_load(scrTempSetting);

    mtsParentCont = lv_cont_create(scrTempSetting, NULL);
    lv_obj_set_size(mtsParentCont, 320, 480);
    lv_obj_set_click(mtsParentCont, false);
    lv_obj_align(mtsParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( mtsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(mtsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(mtsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    
    _mtsContStatusBar = lv_cont_create(mtsParentCont, NULL);
    lv_obj_set_size(_mtsContStatusBar, 320, 35);
    lv_obj_align(_mtsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mtsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mtsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    
    __mtsTimeLabel = lv_label_create(_mtsContStatusBar, NULL);
    lv_obj_align(__mtsTimeLabel, _mtsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__mtsTimeLabel, guiTime);

    static lv_style_t _mhsTimeLabelStyle;
    lv_style_init(&_mhsTimeLabelStyle);
    lv_style_set_text_font(&_mhsTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mhsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtsTimeLabel, LV_LABEL_PART_MAIN, &_mhsTimeLabelStyle);

    //Create Label for Battery icon
    
    __mtsBatteryLabel = lv_label_create(_mtsContStatusBar, NULL);
    lv_obj_align(__mtsBatteryLabel, _mtsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__mtsBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t __mtsBatteryLabelStyle;
    lv_style_init(&__mtsBatteryLabelStyle);
    lv_style_set_text_font(&__mtsBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&__mtsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtsBatteryLabel, LV_LABEL_PART_MAIN, &__mtsBatteryLabelStyle);

    //Create Label for Wifi icon
    __mtsWifiLabel = lv_label_create(_mtsContStatusBar, NULL);
    lv_obj_align(__mtsWifiLabel, __mtsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__mtsWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __mtsWifiLabelStyle;
    lv_style_init(&__mtsWifiLabelStyle);
    lv_style_set_text_font(&__mtsWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__mtsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtsWifiLabel, LV_LABEL_PART_MAIN, &__mtsWifiLabelStyle);

    //Create Label for Signal icon
    
    __mtsSignalLabel = lv_label_create(_mtsContStatusBar, NULL);
    lv_obj_align(__mtsSignalLabel, __mtsWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__mtsSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __mtsSignalLabelStyle;
    lv_style_init(&__mtsSignalLabelStyle);
    lv_style_set_text_font(&__mtsSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__mtsSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtsSignalLabel, LV_LABEL_PART_MAIN, &__mtsSignalLabelStyle);

    //Crate a container to contain FLOW Header

    
    _mtsTempHeadingCont = lv_cont_create(mtsParentCont, NULL);
    lv_obj_set_size(_mtsTempHeadingCont, 300, 70);
    lv_obj_align(_mtsTempHeadingCont, _mtsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mtsTempHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_mtsTempHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    // Create Back arrow img
    
    __mtsBackArrowLabel = lv_img_create(mtsParentCont, NULL);
    lv_img_set_src(__mtsBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__mtsBackArrowLabel, _mtsTempHeadingCont, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_click(__mtsBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__mtsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__mtsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__mtsBackArrowLabel, __mtsBackArrow_event_handler);

    //Create Label for FLOW "Heading"
    
    __mtsTempHeadingLbl = lv_label_create(_mtsTempHeadingCont, NULL);
    lv_obj_align(__mtsTempHeadingLbl, _mtsTempHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -50, -35);
    lv_label_set_align(__mtsTempHeadingLbl, LV_LABEL_ALIGN_CENTER);   
    if(screenid == SCR_METROLOGY_MENU){
        screenid = SCR_EXTERNAL_TEMPERATURE_SETTINGS;
        lv_label_set_text(__mtsTempHeadingLbl, "External \nTemperature");
    }else if(screenid == SCR_EXTERNAL_HUMIDITY_ADJUST){   
        screenid = SCR_INTERNAL_TEMPERATURE_SETTINGS; 
        lv_label_set_text(__mtsTempHeadingLbl, "Internal \nTemperature");
    }

    static lv_style_t __mtsTempHeadingLblStyle;
    lv_style_init(&__mtsTempHeadingLblStyle);
    lv_style_set_text_font(&__mtsTempHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__mtsTempHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mtsTempHeadingLbl, LV_LABEL_PART_MAIN, &__mtsTempHeadingLblStyle);

    // //Create FAN Logo
    
    // _mtsTempLogo = lv_img_create(mtsParentCont, NULL);
    // lv_img_set_src(_mtsTempLogo, &fan_icon);
    // lv_obj_align(_mtsTempLogo, mtsParentCont, LV_ALIGN_IN_TOP_RIGHT, -25 , 55);
    // lv_img_set_auto_size(_mtsTempLogo, true);

    //===============================================================
    //===============================================================

    _mtsCurveUnitCont = lv_cont_create(mtsParentCont, NULL);
    lv_obj_set_size(_mtsCurveUnitCont, 300, 110);
    lv_obj_align(_mtsCurveUnitCont, _mtsTempHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mtsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mtsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mtsBlueTxtStyle;
    lv_style_init(&_mtsBlueTxtStyle);
    lv_style_set_text_font(&_mtsBlueTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mtsBlueTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35,0x9F,0xE2));

    lv_obj_t * _mtsCurveTxt;
    _mtsCurveTxt = lv_label_create(_mtsCurveUnitCont, NULL);
    lv_obj_align(_mtsCurveTxt, _mtsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, -25);
    lv_label_set_text(_mtsCurveTxt, "CURVE");
    lv_obj_add_style(_mtsCurveTxt, LV_LABEL_PART_MAIN, &_mtsBlueTxtStyle);

    lv_obj_t * _mtsUnitTxt;
    _mtsUnitTxt = lv_label_create(_mtsCurveUnitCont, NULL);
    lv_obj_align(_mtsUnitTxt, _mtsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, 15);
    lv_label_set_text(_mtsUnitTxt, "UNIT");
    lv_obj_add_style(_mtsUnitTxt, LV_LABEL_PART_MAIN, &_mtsBlueTxtStyle);

    //```````````````````````````````````````````````````````````
    //
    //```````````````````````````````````````````````````````````
    static lv_style_t _mtsDropDownStylemain;
    lv_style_reset(&_mtsDropDownStylemain);
    lv_style_set_bg_color(&_mtsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mtsDropDownStylemain, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mtsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mtsDropDownStylemain, LV_STATE_DEFAULT, 0);

    static lv_style_t _mtsDropDownStylelist;
    lv_style_reset(&_mtsDropDownStylelist);
    lv_style_set_bg_color(&_mtsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&_mtsDropDownStylelist, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mtsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mtsDropDownStylelist, LV_STATE_DEFAULT, 0);

    static lv_style_t _mtsDropDownStyleselected;
    lv_style_reset(&_mtsDropDownStyleselected);
    lv_style_set_bg_color(&_mtsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35,0x9F,0xE2));
    lv_style_set_text_font(&_mtsDropDownStyleselected, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mtsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mtsDropDownStyleselected, LV_STATE_DEFAULT, 0);

    //Create a Curve selection drop down list
    lv_obj_t * _mtsCurveDropDown = lv_dropdown_create(_mtsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mtsCurveDropDown, 
                 "Linear4\n"
                 "Linear5\n"
                 "Linear6\n"
                 "Linear7\n"
                 "Linear8"
                 );
                

    lv_obj_align(_mtsCurveDropDown, _mtsCurveTxt, LV_ALIGN_OUT_RIGHT_TOP, 100, -2);
    lv_obj_set_size(_mtsCurveDropDown, 120, 30);
    lv_obj_add_style(_mtsCurveDropDown, LV_DROPDOWN_PART_MAIN, &_mtsDropDownStylemain);
    lv_obj_add_style(_mtsCurveDropDown, LV_DROPDOWN_PART_LIST, &_mtsDropDownStylelist);
    lv_obj_add_style(_mtsCurveDropDown, LV_DROPDOWN_PART_SELECTED, &_mtsDropDownStyleselected);
    lv_obj_set_event_cb(_mtsCurveDropDown, curve_dropdown_event_handler);

    //===-------------------------->----------------->---------------------->------------->

    //Create a Unit selection drop down list
    lv_obj_t * _mtsUnitDropDown = lv_dropdown_create(_mtsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mtsUnitDropDown, "°C\n"
                                              "°F");
                

    lv_obj_align(_mtsUnitDropDown, _mtsUnitTxt, LV_ALIGN_OUT_RIGHT_TOP, 117, -2);
    lv_obj_set_size(_mtsUnitDropDown, 120, 30);
    lv_obj_add_style(_mtsUnitDropDown, LV_DROPDOWN_PART_MAIN, &_mtsDropDownStylemain);
    lv_obj_add_style(_mtsUnitDropDown, LV_DROPDOWN_PART_LIST, &_mtsDropDownStylelist);
    lv_obj_add_style(_mtsUnitDropDown, LV_DROPDOWN_PART_SELECTED, &_mtsDropDownStyleselected);
    lv_obj_set_event_cb(_mtsUnitDropDown, unit_dropdown_event_handler);

    //=================================================================
    //=================================================================

    lv_obj_t * _mtsAlarmStPtCont;
    _mtsAlarmStPtCont = lv_cont_create(mtsParentCont, NULL);
    lv_obj_set_size(_mtsAlarmStPtCont, 300, 180);
    lv_obj_align(_mtsAlarmStPtCont, _mtsCurveUnitCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mtsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mtsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mtsBigTxtStyle;
    lv_style_init(&_mtsBigTxtStyle);
    lv_style_set_text_font(&_mtsBigTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mtsBigTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    static lv_style_t _mtsSmallTxtStyle;
    lv_style_init(&_mtsSmallTxtStyle);
    lv_style_set_text_font(&_mtsSmallTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); //signal_20
    lv_style_set_text_color(&_mtsSmallTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    lv_obj_t * _mtsAlarmSetPointTxt;
    _mtsAlarmSetPointTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsAlarmSetPointTxt, _mtsAlarmStPtCont, LV_ALIGN_IN_TOP_LEFT, 10, 8);
    lv_label_set_text(_mtsAlarmSetPointTxt, "ALERT FROM SET POINT");
    lv_obj_add_style(_mtsAlarmSetPointTxt, LV_LABEL_PART_MAIN, &_mtsBigTxtStyle);

    lv_obj_t * _mtsOnOffTxt;
    _mtsOnOffTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsOnOffTxt, _mtsAlarmSetPointTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mtsOnOffTxt, "ON / OFF");
    lv_obj_add_style(_mtsOnOffTxt, LV_LABEL_PART_MAIN, &_mtsSmallTxtStyle);

    //=========================
    //=========================
    //Write style state: LV_STATE_DEFAULT for style_screen_sw_1_bg
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

	//Write style LV_SWITCH_PART_INDIC for screen_sw_1
	static lv_style_t style_switch_indic;
	lv_style_reset(&style_switch_indic);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_1_indic
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

	//Write style LV_SWITCH_PART_KNOB for screen_sw_1
	static lv_style_t style_switch_knob;
	lv_style_reset(&style_switch_knob);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_1_knob
	//lv_style_set_radius(&style_switch_knob, LV_STATE_DEFAULT, 100);
	lv_style_set_bg_color(&style_switch_knob, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_grad_color(&style_switch_knob, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_grad_dir(&style_switch_knob, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_switch_knob, LV_STATE_DEFAULT, 255);

    //Create On / OFF control switch 

    lv_obj_t *_mtsOnOffSwitch = lv_switch_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsOnOffSwitch, _mtsOnOffTxt, LV_ALIGN_OUT_RIGHT_TOP, 155, -15);
    lv_obj_add_style(_mtsOnOffSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mtsOnOffSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mtsOnOffSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mtsOnOffSwitch, 25);
    lv_obj_set_width(_mtsOnOffSwitch, 60);
    //lv_obj_set_event_cb(_mtsOnOffSwitch, Buzzer_switch_event_handler);
   

    //==============================
    //==============================


    lv_obj_t * _mtsLowerLimitTxt;
    _mtsLowerLimitTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsLowerLimitTxt, _mtsOnOffTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mtsLowerLimitTxt, "LOWER LIMIT");
    lv_obj_add_style(_mtsLowerLimitTxt, LV_LABEL_PART_MAIN, &_mtsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mtsLowerLimDropDown = lv_dropdown_create(_mtsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mtsLowerLimDropDown, "-10\n"
                "-20");
    lv_obj_align(_mtsLowerLimDropDown, _mtsLowerLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mtsLowerLimDropDown, 120, 30);
    lv_obj_add_style(_mtsLowerLimDropDown, LV_DROPDOWN_PART_MAIN, &_mtsDropDownStylemain);
    lv_obj_add_style(_mtsLowerLimDropDown, LV_DROPDOWN_PART_LIST, &_mtsDropDownStylelist);
    lv_obj_add_style(_mtsLowerLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mtsDropDownStyleselected);
    lv_obj_set_event_cb(_mtsLowerLimDropDown, lowerlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mtsHigherLimitTxt;
    _mtsHigherLimitTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsHigherLimitTxt, _mtsLowerLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(_mtsHigherLimitTxt, "HIGHER LIMIT");
    lv_obj_add_style(_mtsHigherLimitTxt, LV_LABEL_PART_MAIN, &_mtsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mtsHigherLimDropDown = lv_dropdown_create(_mtsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mtsHigherLimDropDown, "40\n"
                "50");
    lv_obj_align(_mtsHigherLimDropDown, _mtsHigherLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mtsHigherLimDropDown, 120, 30);
    lv_obj_add_style(_mtsHigherLimDropDown, LV_DROPDOWN_PART_MAIN, &_mtsDropDownStylemain);
    lv_obj_add_style(_mtsHigherLimDropDown, LV_DROPDOWN_PART_LIST, &_mtsDropDownStylelist);
    lv_obj_add_style(_mtsHigherLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mtsDropDownStyleselected);
    lv_obj_set_event_cb(_mtsHigherLimDropDown, higherlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mtsActionTxt;
    _mtsActionTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsActionTxt, _mtsHigherLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_mtsActionTxt, "ACTION");
    lv_obj_add_style(_mtsActionTxt, LV_LABEL_PART_MAIN, &_mtsBigTxtStyle);

    lv_obj_t * _mtsDisableDeviceTxt;
    _mtsDisableDeviceTxt = lv_label_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsDisableDeviceTxt, _mtsActionTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_mtsDisableDeviceTxt, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_mtsDisableDeviceTxt, LV_LABEL_PART_MAIN, &_mtsSmallTxtStyle);

    //=========================
    //=========================

    //Create On / OFF control switch 

    lv_obj_t *_mtsActionSwitch = lv_switch_create(_mtsAlarmStPtCont, NULL);
    lv_obj_align(_mtsActionSwitch, _mtsActionTxt, LV_ALIGN_OUT_RIGHT_TOP, 160, 10);
    lv_obj_add_style(_mtsActionSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mtsActionSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mtsActionSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mtsActionSwitch, 25);
    lv_obj_set_width(_mtsActionSwitch, 60);
    //lv_obj_set_event_cb(_mtsOnOffSwitch, Buzzer_switch_event_handler);

    
    //==============================
    //==============================
    


    //===================================================================
    //===================================================================
     static lv_style_t _mtsValidBtnLblStyle;
    lv_style_init(&_mtsValidBtnLblStyle);
    lv_style_set_text_font(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT  ,0); 
    lv_style_set_text_color(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&_mtsValidBtnLblStyle, LV_STATE_DEFAULT, 5);

    static lv_style_t _mtsValidBtnLblStylepressed;
    lv_style_init(&_mtsValidBtnLblStylepressed);
    lv_style_set_text_font(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_BLUE); 
    lv_style_set_text_color(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, 0);
    lv_style_set_radius(&_mtsValidBtnLblStylepressed, LV_STATE_PRESSED, 5);

    static lv_style_t _mtsValidBtnLblStylefocused;
    lv_style_init(&_mtsValidBtnLblStylefocused);
    lv_style_set_text_font(&_mtsValidBtnLblStylefocused, LV_STATE_FOCUSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mtsValidBtnLblStylefocused, LV_STATE_FOCUSED  ,0); 
    lv_style_set_text_color(&_mtsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mtsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mtsValidBtnLblStylefocused, LV_STATE_FOCUSED, 0);
    lv_style_set_radius(&_mtsValidBtnLblStylefocused, LV_STATE_PRESSED, 5);

    //Creat a stop Button
    lv_obj_t * _mtsValidBtn;
    _mtsValidBtn = lv_btn_create(mtsParentCont, NULL);
    lv_obj_align(_mtsValidBtn, mtsParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_size(_mtsValidBtn, 300, 44);
    lv_obj_add_style(_mtsValidBtn, LV_BTN_PART_MAIN, &_mtsValidBtnLblStyle);
    lv_obj_add_style(_mtsValidBtn, LV_BTN_PART_MAIN, &_mtsValidBtnLblStylepressed);
    lv_obj_add_style(_mtsValidBtn, LV_BTN_PART_MAIN, &_mtsValidBtnLblStylefocused);
    lv_obj_set_event_cb(_mtsValidBtn,  __mtsValidAdjBTN_event_handler);
   

    //Creat a stop Button Label
    lv_obj_t * _mtsValidTxt;
    _mtsValidTxt = lv_label_create(_mtsValidBtn, NULL);
    lv_obj_align(_mtsValidTxt, _mtsValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_mtsValidTxt, "VALID AND ADJUST");
    
    crnt_screen = scrTempSetting;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  __mtsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        callMetroAdjust();
    }
}

static void  __mtsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        CallMetroMenuScreen();
    }
}

static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        //printf("Option: %s\n", buf);
        //fflush(NULL);
        if(  strcmp (buf, "Linear1") == 0)
        {
            global_CurveDegree_temp = 1;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear2") == 0)
        {
            global_CurveDegree_temp = 2;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear3") == 0)
        {
            global_CurveDegree_temp = 3;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear4") == 0)
        {
            global_CurveDegree_temp = 4;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear5") == 0)
        {
            global_CurveDegree_temp = 5;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear6") == 0)
        {
            global_CurveDegree_temp = 6;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear7") == 0)
        {
            global_CurveDegree_temp = 7;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear8") == 0)
        {
            global_CurveDegree_temp = 8;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else if(  strcmp (buf, "Linear9") == 0)
        {
            global_CurveDegree_temp = 9;
            // printf("Curve Points: %d\n", global_CurveDegree_temp);
            // fflush(NULL);
        }
        else{}


            
    }
}

static void  unit_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if( ! strcmp( buf, "°C"))
        {
            metroTempUnit = 0;
            printf("Temperature Unit selected is °C = %d\n", metroTempUnit);
        }
        if( ! strcmp( buf, "°F"))
        {
            metroTempUnit = 1;
            printf("Temperature Unit selected is °F = %d\n", metroTempUnit);
        }
    }
}

static void  lowerlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
    }
}

static void  higherlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
    }
}


/**********************
 *    ERROR ASSERT
 **********************/
