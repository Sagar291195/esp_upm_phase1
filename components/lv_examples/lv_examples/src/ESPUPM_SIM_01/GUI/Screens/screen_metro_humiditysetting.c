/**
 *  @copyright "License Name" described in the LICENSE file.
 *  @author    Abhai Tiwari
 *  @date      2021-03-24
 */

/**
*  @file screen_metro_humiditysetting.c
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
static void  __mhsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event);
static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  unit_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  lowerlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  higherlim_dropdown_event_handler(lv_obj_t * obj, lv_event_t event);
static void  __mhsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event);
/**********************
 *  STATIC VARIABLES
 **********************/


lv_obj_t *crnt_screen;
lv_obj_t * scrHumiditySetting;
lv_obj_t * mhsParentCont;
lv_obj_t * _mhsContStatusBar;
lv_obj_t * __mhsTimeLabel;
lv_obj_t * __mhsBatteryLabel;
lv_obj_t * __mhsWifiLabel;
lv_obj_t * __mhsSignalLabel;
lv_obj_t * _mhsHumidityHeadingCont;
lv_obj_t * __mhsBackArrowLabel;
lv_obj_t * __mhsHumidityHeadingLbl;
lv_obj_t * _mhsHumidityLogo;
lv_obj_t * _mhsCurveUnitCont;


int global_CurveDegree_humidity;

int metrohumidityUnit;    // for LPH flowUnit == 0, For LPM == 1

/**********************
 *      MACROS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void callMetroHumiditySettingScreen(void)
{
    //Create Base container
    
    
    scrHumiditySetting = lv_cont_create(NULL, NULL);
    lv_scr_load(scrHumiditySetting);

    mhsParentCont = lv_cont_create(scrHumiditySetting, NULL);
    lv_obj_set_size(mhsParentCont, 320, 480);
    lv_obj_set_click(mhsParentCont, false);
    lv_obj_align(mhsParentCont, NULL, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_style_local_bg_color( mhsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //3989BD
    lv_obj_set_style_local_border_opa(mhsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_radius(mhsParentCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);

    //Create a Satus BAR Container to contain Watch , Signal, wifi & battery status

    
    _mhsContStatusBar = lv_cont_create(mhsParentCont, NULL);
    lv_obj_set_size(_mhsContStatusBar, 320, 35);
    lv_obj_align(_mhsContStatusBar, NULL, LV_ALIGN_IN_TOP_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mhsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mhsContStatusBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    //Create Watch upper left corner
    
    __mhsTimeLabel = lv_label_create(_mhsContStatusBar, NULL);
    lv_obj_align(__mhsTimeLabel, _mhsContStatusBar, LV_ALIGN_IN_TOP_LEFT, 12,5);
    lv_label_set_text(__mhsTimeLabel, guiTime);

    static lv_style_t _mhsTimeLabelStyle;
    lv_style_init(&_mhsTimeLabelStyle);
    lv_style_set_text_font(&_mhsTimeLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_20); 
    lv_style_set_text_color(&_mhsTimeLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mhsTimeLabel, LV_LABEL_PART_MAIN, &_mhsTimeLabelStyle);

    //Create Label for Battery icon
    
    __mhsBatteryLabel = lv_label_create(_mhsContStatusBar, NULL);
    lv_obj_align(__mhsBatteryLabel, _mhsContStatusBar, LV_ALIGN_IN_TOP_RIGHT, -10, 5);
    lv_label_set_text(__mhsBatteryLabel, LV_SYMBOL_BATTERY_FULL); //LV_SYMBOL_BATTERY_FULL

    static lv_style_t __mhsBatteryLabelStyle;
    lv_style_init(&__mhsBatteryLabelStyle);
    lv_style_set_text_font(&__mhsBatteryLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_24); 
    lv_style_set_text_color(&__mhsBatteryLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mhsBatteryLabel, LV_LABEL_PART_MAIN, &__mhsBatteryLabelStyle);

    //Create Label for Wifi icon
    __mhsWifiLabel = lv_label_create(_mhsContStatusBar, NULL);
    lv_obj_align(__mhsWifiLabel, __mhsBatteryLabel, LV_ALIGN_OUT_LEFT_TOP, -7, 2);
    lv_label_set_text(__mhsWifiLabel, LV_SYMBOL_WIFI);

    static lv_style_t __mhsWifiLabelStyle;
    lv_style_init(&__mhsWifiLabelStyle);
    lv_style_set_text_font(&__mhsWifiLabelStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); 
    lv_style_set_text_color(&__mhsWifiLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mhsWifiLabel, LV_LABEL_PART_MAIN, &__mhsWifiLabelStyle);

    //Create Label for Signal icon
    
    __mhsSignalLabel = lv_label_create(_mhsContStatusBar, NULL);
    lv_obj_align(__mhsSignalLabel, __mhsWifiLabel, LV_ALIGN_OUT_LEFT_TOP, -5, 1);
    lv_label_set_text(__mhsSignalLabel, SYMBOL_SIGNAL); //"\uf012" #define SYMBOL_SIGNAL "\uf012"

    static lv_style_t __mhsSignalLabelStyle;
    lv_style_init(&__mhsSignalLabelStyle);
    lv_style_set_text_font(&__mhsSignalLabelStyle, LV_STATE_DEFAULT  ,&signal_20); //signal_20
    lv_style_set_text_color(&__mhsSignalLabelStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mhsSignalLabel, LV_LABEL_PART_MAIN, &__mhsSignalLabelStyle);

    //Crate a container to contain FLOW Header

    
    _mhsHumidityHeadingCont = lv_cont_create(mhsParentCont, NULL);
    lv_obj_set_size(_mhsHumidityHeadingCont, 300, 70);
    lv_obj_align(_mhsHumidityHeadingCont, _mhsContStatusBar, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_set_style_local_bg_color(_mhsHumidityHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x39, 0x89, 0xBD) ); //5f615f , LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_width(_mhsHumidityHeadingCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0 );

    // Create Back arrow img
    
    __mhsBackArrowLabel = lv_img_create(mhsParentCont, NULL);
    lv_img_set_src(__mhsBackArrowLabel, &left_arrow_icon);
    lv_obj_align(__mhsBackArrowLabel, _mhsHumidityHeadingCont, LV_ALIGN_IN_LEFT_MID, 5 , 0);
    lv_obj_set_click(__mhsBackArrowLabel, true);
    lv_obj_set_style_local_image_recolor_opa(__mhsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_image_recolor(__mhsBackArrowLabel, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(__mhsBackArrowLabel, __mhsBackArrow_event_handler);

    //Create Label for FLOW "Heading"
    
    __mhsHumidityHeadingLbl = lv_label_create(_mhsHumidityHeadingCont, NULL);
    lv_obj_align(__mhsHumidityHeadingLbl, _mhsHumidityHeadingCont, LV_ALIGN_IN_BOTTOM_MID, -30, -35);
    lv_label_set_align(__mhsHumidityHeadingLbl, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(__mhsHumidityHeadingLbl, "External \nHumidity");

    static lv_style_t __fasFlowHeadingLblStyle;
    lv_style_init(&__fasFlowHeadingLblStyle);
    lv_style_set_text_font(&__fasFlowHeadingLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_22); //signal_20
    lv_style_set_text_color(&__fasFlowHeadingLblStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);
    lv_obj_add_style(__mhsHumidityHeadingLbl, LV_LABEL_PART_MAIN, &__fasFlowHeadingLblStyle);

    // //Create FAN Logo
    
    // _mhsHumidityLogo = lv_img_create(mhsParentCont, NULL);
    // lv_img_set_src(_mhsHumidityLogo, &fan_icon);
    // lv_obj_align(_mhsHumidityLogo, mhsParentCont, LV_ALIGN_IN_TOP_RIGHT, -25 , 55);
    // lv_img_set_auto_size(_mhsHumidityLogo, true);

    //===============================================================
    //===============================================================

    _mhsCurveUnitCont = lv_cont_create(mhsParentCont, NULL);
    lv_obj_set_size(_mhsCurveUnitCont, 300, 110);
    lv_obj_align(_mhsCurveUnitCont, _mhsHumidityHeadingCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mhsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mhsCurveUnitCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mfsBlueTxtStyle;
    lv_style_init(&_mfsBlueTxtStyle);
    lv_style_set_text_font(&_mfsBlueTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mfsBlueTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_MAKE(0x35,0x9F,0xE2));

    lv_obj_t * _mhsCurveTxt;
    _mhsCurveTxt = lv_label_create(_mhsCurveUnitCont, NULL);
    lv_obj_align(_mhsCurveTxt, _mhsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, -25);
    lv_label_set_text(_mhsCurveTxt, "CURVE");
    lv_obj_add_style(_mhsCurveTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    lv_obj_t * _mhsUnitTxt;
    _mhsUnitTxt = lv_label_create(_mhsCurveUnitCont, NULL);
    lv_obj_align(_mhsUnitTxt, _mhsCurveUnitCont, LV_ALIGN_IN_LEFT_MID, 15, 15);
    lv_label_set_text(_mhsUnitTxt, "UNIT");
    lv_obj_add_style(_mhsUnitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //```````````````````````````````````````````````````````````
    //
    //```````````````````````````````````````````````````````````
     static lv_style_t _mhsDropDownStylemain;
    lv_style_reset(&_mhsDropDownStylemain);
    lv_style_set_bg_color(&_mhsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D, 0x5D, 0x5D));
    lv_style_set_text_font(&_mhsDropDownStylemain, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mhsDropDownStylemain, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&_mhsDropDownStylemain, LV_STATE_DEFAULT, 0);

    static lv_style_t _mhsDropDownStylelist;
    lv_style_reset(&_mhsDropDownStylelist);
    lv_style_set_bg_color(&_mhsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&_mhsDropDownStylelist, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mhsDropDownStylelist, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mhsDropDownStylelist, LV_STATE_DEFAULT, 0);

    static lv_style_t _mhsDropDownStyleselected;
    lv_style_reset(&_mhsDropDownStyleselected);
    lv_style_set_bg_color(&_mhsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x35,0x9F,0xE2));
    lv_style_set_text_font(&_mhsDropDownStyleselected, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_style_set_text_color(&_mhsDropDownStyleselected, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&_mhsDropDownStyleselected, LV_STATE_DEFAULT, 0);

    //Create a Curve selection drop down list
    lv_obj_t * _mhsCurveDropDown = lv_dropdown_create(_mhsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mhsCurveDropDown, 
                 "Linear4\n"
                 "Linear5\n"
                 "Linear6\n"
                 "Linear7\n"
                 "Linear8"
                 );
                

    lv_obj_align(_mhsCurveDropDown, _mhsCurveTxt, LV_ALIGN_OUT_RIGHT_TOP, 100, -2);
    lv_obj_set_size(_mhsCurveDropDown, 120, 30);
    lv_obj_add_style(_mhsCurveDropDown, LV_DROPDOWN_PART_MAIN, &_mhsDropDownStylemain);
    lv_obj_add_style(_mhsCurveDropDown, LV_DROPDOWN_PART_LIST, &_mhsDropDownStylelist);
    lv_obj_add_style(_mhsCurveDropDown, LV_DROPDOWN_PART_SELECTED, &_mhsDropDownStyleselected);
    lv_obj_set_event_cb(_mhsCurveDropDown, curve_dropdown_event_handler);

    //===-------------------------->----------------->---------------------->------------->

    //Create a Unit selection drop down list
    lv_obj_t * _mhsUnitDropDown = lv_dropdown_create(_mhsCurveUnitCont, NULL);
    lv_dropdown_set_options(_mhsUnitDropDown, "LPM\n"
                                              "LPH");
                

    lv_obj_align(_mhsUnitDropDown, _mhsUnitTxt, LV_ALIGN_OUT_RIGHT_TOP, 117, -2);
    lv_obj_set_size(_mhsUnitDropDown, 120, 30);
    lv_obj_add_style(_mhsUnitDropDown, LV_DROPDOWN_PART_MAIN, &_mhsDropDownStylemain);
    lv_obj_add_style(_mhsUnitDropDown, LV_DROPDOWN_PART_LIST, &_mhsDropDownStylelist);
    lv_obj_add_style(_mhsUnitDropDown, LV_DROPDOWN_PART_SELECTED, &_mhsDropDownStyleselected);
    lv_obj_set_event_cb(_mhsUnitDropDown, unit_dropdown_event_handler);

    //=================================================================
    //=================================================================

    lv_obj_t * _mhsAlarmStPtCont;
    _mhsAlarmStPtCont = lv_cont_create(mhsParentCont, NULL);
    lv_obj_set_size(_mhsAlarmStPtCont, 300, 180);
    lv_obj_align(_mhsAlarmStPtCont, _mhsCurveUnitCont, LV_ALIGN_OUT_BOTTOM_LEFT, 0,10);
    lv_obj_set_style_local_bg_color(_mhsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3D, 0x3D, 0x3D) ); //LV_COLOR_MAKE(0x5D, 0x5D, 0x5D)
    lv_obj_set_style_local_border_opa(_mhsAlarmStPtCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_MIN );

    static lv_style_t _mhsBigTxtStyle;
    lv_style_init(&_mhsBigTxtStyle);
    lv_style_set_text_font(&_mhsBigTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_14); //signal_20
    lv_style_set_text_color(&_mhsBigTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    static lv_style_t _mhsSmallTxtStyle;
    lv_style_init(&_mhsSmallTxtStyle);
    lv_style_set_text_font(&_mhsSmallTxtStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_12); //signal_20
    lv_style_set_text_color(&_mhsSmallTxtStyle, LV_LABEL_PART_MAIN, LV_COLOR_WHITE);

    lv_obj_t * _mhsAlarmSetPointTxt;
    _mhsAlarmSetPointTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsAlarmSetPointTxt, _mhsAlarmStPtCont, LV_ALIGN_IN_TOP_LEFT, 10, 8);
    lv_label_set_text(_mhsAlarmSetPointTxt, "ALERT FROM SET POINT");
    lv_obj_add_style(_mhsAlarmSetPointTxt, LV_LABEL_PART_MAIN, &_mhsBigTxtStyle);

    lv_obj_t * _mhsOnOffTxt;
    _mhsOnOffTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsOnOffTxt, _mhsAlarmSetPointTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mhsOnOffTxt, "ON / OFF");
    lv_obj_add_style(_mhsOnOffTxt, LV_LABEL_PART_MAIN, &_mhsSmallTxtStyle);

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

    lv_obj_t *_mhsOnOffSwitch = lv_switch_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsOnOffSwitch, _mhsOnOffTxt, LV_ALIGN_OUT_RIGHT_TOP, 155, -15);
    lv_obj_add_style(_mhsOnOffSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mhsOnOffSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mhsOnOffSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mhsOnOffSwitch, 25);
    lv_obj_set_width(_mhsOnOffSwitch, 60);
    //lv_obj_set_event_cb(_mhsOnOffSwitch, Buzzer_switch_event_handler);
   

    //==============================
    //==============================


    lv_obj_t * _mhsLowerLimitTxt;
    _mhsLowerLimitTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsLowerLimitTxt, _mhsOnOffTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text(_mhsLowerLimitTxt, "LOWER LIMIT");
    lv_obj_add_style(_mhsLowerLimitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mhsLowerLimDropDown = lv_dropdown_create(_mhsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mhsLowerLimDropDown, "-10%\n"
                "-20%");
    lv_obj_align(_mhsLowerLimDropDown, _mhsLowerLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mhsLowerLimDropDown, 120, 30);
    lv_obj_add_style(_mhsLowerLimDropDown, LV_DROPDOWN_PART_MAIN, &_mhsDropDownStylemain);
    lv_obj_add_style(_mhsLowerLimDropDown, LV_DROPDOWN_PART_LIST, &_mhsDropDownStylelist);
    lv_obj_add_style(_mhsLowerLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mhsDropDownStyleselected);
    lv_obj_set_event_cb(_mhsLowerLimDropDown, lowerlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mhsHigherLimitTxt;
    _mhsHigherLimitTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsHigherLimitTxt, _mhsLowerLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(_mhsHigherLimitTxt, "HIGHER LIMIT");
    lv_obj_add_style(_mhsHigherLimitTxt, LV_LABEL_PART_MAIN, &_mfsBlueTxtStyle);

    //============================
    //============================

    //Create a Curve selection drop down list
    lv_obj_t * _mhsHigherLimDropDown = lv_dropdown_create(_mhsAlarmStPtCont, NULL);
    lv_dropdown_set_options(_mhsHigherLimDropDown, "10%\n"
                "20%");
    lv_obj_align(_mhsHigherLimDropDown, _mhsHigherLimitTxt, LV_ALIGN_OUT_RIGHT_TOP, 60, -4);
    lv_obj_set_size(_mhsHigherLimDropDown, 120, 30);
    lv_obj_add_style(_mhsHigherLimDropDown, LV_DROPDOWN_PART_MAIN, &_mhsDropDownStylemain);
    lv_obj_add_style(_mhsHigherLimDropDown, LV_DROPDOWN_PART_LIST, &_mhsDropDownStylelist);
    lv_obj_add_style(_mhsHigherLimDropDown, LV_DROPDOWN_PART_SELECTED, &_mhsDropDownStyleselected);
    lv_obj_set_event_cb(_mhsHigherLimDropDown, higherlim_dropdown_event_handler);

    //============================
    //============================

    lv_obj_t * _mhsActionTxt;
    _mhsActionTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsActionTxt, _mhsHigherLimitTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_label_set_text(_mhsActionTxt, "ACTION");
    lv_obj_add_style(_mhsActionTxt, LV_LABEL_PART_MAIN, &_mhsBigTxtStyle);

    lv_obj_t * _mhsDisableDeviceTxt;
    _mhsDisableDeviceTxt = lv_label_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsDisableDeviceTxt, _mhsActionTxt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    lv_label_set_text(_mhsDisableDeviceTxt, "DISABLE DEVICE AFTER ALERT");
    lv_obj_add_style(_mhsDisableDeviceTxt, LV_LABEL_PART_MAIN, &_mhsSmallTxtStyle);

    //=========================
    //=========================

    //Create On / OFF control switch 

    lv_obj_t *_mhsActionSwitch = lv_switch_create(_mhsAlarmStPtCont, NULL);
    lv_obj_align(_mhsActionSwitch, _mhsActionTxt, LV_ALIGN_OUT_RIGHT_TOP, 160, 10);
    lv_obj_add_style(_mhsActionSwitch, LV_SWITCH_PART_BG, &style_switch_bg);
    lv_obj_add_style(_mhsActionSwitch, LV_SWITCH_PART_INDIC, &style_switch_indic);
    lv_obj_add_style(_mhsActionSwitch, LV_SWITCH_PART_KNOB, &style_switch_knob);
    lv_obj_set_height(_mhsActionSwitch, 25);
    lv_obj_set_width(_mhsActionSwitch, 60);
    //lv_obj_set_event_cb(_mhsOnOffSwitch, Buzzer_switch_event_handler);

    
    //==============================
    //==============================
    


    //===================================================================
    //===================================================================
    static lv_style_t _mhsValidBtnLblStyle;
    lv_style_init(&_mhsValidBtnLblStyle);
    lv_style_set_text_font(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT  ,0); 
    lv_style_set_text_color(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&_mhsValidBtnLblStyle, LV_STATE_DEFAULT, 5);

    static lv_style_t _mhsValidBtnLblStylepressed;
    lv_style_init(&_mhsValidBtnLblStylepressed);
    lv_style_set_text_font(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_BLUE); 
    lv_style_set_text_color(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, 0);
    lv_style_set_radius(&_mhsValidBtnLblStylepressed, LV_STATE_PRESSED, 5);

    static lv_style_t _mhsValidBtnLblStylefocused;
    lv_style_init(&_mhsValidBtnLblStylefocused);
    lv_style_set_text_font(&_mhsValidBtnLblStylefocused, LV_STATE_FOCUSED  ,&lv_font_montserrat_16); 
    lv_style_set_border_width(&_mhsValidBtnLblStylefocused, LV_STATE_FOCUSED  ,0); 
    lv_style_set_text_color(&_mhsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_bg_color(&_mhsValidBtnLblStylefocused, LV_STATE_FOCUSED, LV_COLOR_MAKE(0x5D,0xAF,0x48));
    lv_style_set_outline_width(&_mhsValidBtnLblStylefocused, LV_STATE_FOCUSED, 0);
    lv_style_set_radius(&_mhsValidBtnLblStylefocused, LV_STATE_PRESSED, 5);
    //Creat a stop Button
    lv_obj_t * _mhsValidBtn;
    _mhsValidBtn = lv_btn_create(mhsParentCont, NULL);
    lv_obj_align(_mhsValidBtn, mhsParentCont, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_size(_mhsValidBtn, 300, 44);
    lv_obj_add_style(_mhsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStyle);
    lv_obj_add_style(_mhsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStylepressed);
    lv_obj_add_style(_mhsValidBtn, LV_BTN_PART_MAIN, &_mpsValidBtnLblStylefocused);
    lv_obj_set_event_cb(_mhsValidBtn,  __mhsValidAdjBTN_event_handler);
   

    //Creat a stop Button Label
    lv_obj_t * _mhsValidTxt;
    _mhsValidTxt = lv_label_create(_mhsValidBtn, NULL);
    lv_obj_align(_mhsValidTxt, _mhsValidBtn, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_text(_mhsValidTxt, "VALID & ADJUST");
    lv_obj_set_style_local_text_font(_mhsValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_obj_set_style_local_text_color(_mhsValidTxt, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    
    crnt_screen = scrHumiditySetting;

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  __mhsValidAdjBTN_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {

    }
}

static void  __mhsBackArrow_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        printf("Back to Metrology Menu Screen\n");
        CallMetroMenuScreen();
    }
}

static void  curve_dropdown_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        fflush(NULL);
        if(  strcmp (buf, "Linear1") == 0)
        {
            global_CurveDegree_humidity = 1;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear2") == 0)
        {
            global_CurveDegree_humidity = 2;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear3") == 0)
        {
            global_CurveDegree_humidity = 3;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear4") == 0)
        {
            global_CurveDegree_humidity = 4;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear5") == 0)
        {
            global_CurveDegree_humidity = 5;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear6") == 0)
        {
            global_CurveDegree_humidity = 6;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear7") == 0)
        {
            global_CurveDegree_humidity = 7;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear8") == 0)
        {
            global_CurveDegree_humidity = 8;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
        }
        else if(  strcmp (buf, "Linear9") == 0)
        {
            global_CurveDegree_humidity = 9;
            printf("Curve Points: %d\n", global_CurveDegree_humidity);
            fflush(NULL);
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
        if( ! strcmp( buf, "LPH"))
        {
            metrohumidityUnit = 0;
            printf("Flow Unit selected is LPH = %d\n", metrohumidityUnit);
        }
        if( ! strcmp( buf, "LPM"))
        {
            metrohumidityUnit = 1;
            printf("Flow Unit selected is LPM = %d\n", metrohumidityUnit);
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
